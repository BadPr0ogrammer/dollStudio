#include "vtkF3DAssimpImporter.h"

vtkStandardNewMacro(vtkF3DAssimpImporter);


//----------------------------------------------------------------------------
vtkF3DAssimpImporter::vtkF3DAssimpImporter()
  : Internals(new vtkF3DAssimpImporter::vtkInternals(this))
{
}

//----------------------------------------------------------------------------
vtkF3DAssimpImporter::~vtkF3DAssimpImporter() = default;

//----------------------------------------------------------------------------
int vtkF3DAssimpImporter::ImportBegin()
{
  return this->Internals->ReadScene(this->FileName);
}

//----------------------------------------------------------------------------
void vtkF3DAssimpImporter::ImportActors(vtkRenderer* renderer)
{
  this->Internals->ImportRoot(renderer);
#if VTK_VERSION_NUMBER >= VTK_VERSION_CHECK(9, 3, 20240707)
  // Record all actors imported from internals to importer itself
  for (auto& pair : this->Internals->NodeActors)
  {
    vtkCollectionSimpleIterator ait;
    pair.second->InitTraversal(ait);
    while (auto* actor = pair.second->GetNextActor(ait))
    {
      this->ActorCollection->AddItem(actor);
    }
  }
#endif
}

//----------------------------------------------------------------------------
std::string vtkF3DAssimpImporter::GetOutputsDescription()
{
  return this->Internals->Description;
}

//----------------------------------------------------------------------------
bool vtkF3DAssimpImporter::UpdateAtTimeValue(double timeValue)
{
  assert(this->Internals->ActiveAnimation < this->GetNumberOfAnimations());
  if (this->Internals->ActiveAnimation == -1)
  {
    return true;
  }

  // get the animation tick
  double fps =
    this->Internals->Scene->mAnimations[this->Internals->ActiveAnimation]->mTicksPerSecond;
  if (fps == 0.0)
  {
    fps = 1.0;
  }

  aiAnimation* anim = this->Internals->Scene->mAnimations[this->Internals->ActiveAnimation];
  double tick = timeValue * fps;

  Assimp::Interpolator<aiVectorKey> vectorInterpolator;
  Assimp::Interpolator<aiQuatKey> quaternionInterpolator;

  for (unsigned int nodeChannelId = 0; nodeChannelId < anim->mNumChannels; nodeChannelId++)
  {
    aiNodeAnim* nodeAnim = anim->mChannels[nodeChannelId];

    aiVector3D translation;
    aiVector3D scaling;
    aiQuaternion quaternion;

    aiVectorKey* positionKey = std::lower_bound(nodeAnim->mPositionKeys,
      nodeAnim->mPositionKeys + nodeAnim->mNumPositionKeys, tick,
      [](const aiVectorKey& key, const double& time) { return key.mTime < time; });

    if (positionKey == nodeAnim->mPositionKeys)
    {
      translation = positionKey->mValue;
    }
    else if (positionKey == nodeAnim->mPositionKeys + nodeAnim->mNumPositionKeys)
    {
      translation = (positionKey - 1)->mValue;
    }
    else
    {
      aiVectorKey* prev = positionKey - 1;
      ai_real d = (tick - prev->mTime) / (positionKey->mTime - prev->mTime);
      vectorInterpolator(translation, *prev, *positionKey, d);
    }

    aiQuatKey* rotationKey = std::lower_bound(nodeAnim->mRotationKeys,
      nodeAnim->mRotationKeys + nodeAnim->mNumRotationKeys, tick,
      [](const aiQuatKey& key, const double& time) { return key.mTime < time; });

    if (rotationKey == nodeAnim->mRotationKeys)
    {
      quaternion = rotationKey->mValue;
    }
    else if (rotationKey == nodeAnim->mRotationKeys + nodeAnim->mNumRotationKeys)
    {
      quaternion = (rotationKey - 1)->mValue;
    }
    else
    {
      aiQuatKey* prev = rotationKey - 1;
      ai_real d = (tick - prev->mTime) / (rotationKey->mTime - prev->mTime);
      quaternionInterpolator(quaternion, *prev, *rotationKey, d);
    }

    aiVectorKey* scalingKey =
      std::lower_bound(nodeAnim->mScalingKeys, nodeAnim->mScalingKeys + nodeAnim->mNumScalingKeys,
        tick, [](const aiVectorKey& key, const double& time) { return key.mTime < time; });

    if (scalingKey == nodeAnim->mScalingKeys)
    {
      scaling = scalingKey->mValue;
    }
    else if (scalingKey == nodeAnim->mScalingKeys + nodeAnim->mNumScalingKeys)
    {
      scaling = (scalingKey - 1)->mValue;
    }
    else
    {
      aiVectorKey* prev = scalingKey - 1;
      ai_real d = (tick - prev->mTime) / (scalingKey->mTime - prev->mTime);
      vectorInterpolator(scaling, *prev, *scalingKey, d);
    }

    vtkMatrix4x4* transform = this->Internals->NodeLocalMatrix[nodeAnim->mNodeName.data];

    if (transform)
    {
      // Initialize quaternion
      vtkQuaternion<double> rotation;
      rotation.Set(quaternion.w, quaternion.x, quaternion.y, quaternion.z);
      rotation.Normalize();

      double rotationMatrix[3][3];
      rotation.ToMatrix3x3(rotationMatrix);

      // Apply transformations
      for (int i = 0; i < 3; i++)
      {
        for (int j = 0; j < 3; j++)
        {
          transform->SetElement(i, j, scaling[j] * rotationMatrix[i][j]);
        }
        transform->SetElement(i, 3, translation[i]);
      }
    }
  }

  vtkNew<vtkMatrix4x4> identity;
  this->Internals->UpdateNodeTransform(this->Internals->Scene->mRootNode, identity);

  this->Internals->UpdateBones();
  this->Internals->UpdateCameras();
  this->Internals->UpdateLights();
  return true;
}

//----------------------------------------------------------------------------
vtkIdType vtkF3DAssimpImporter::GetNumberOfAnimations()
{
  return this->Internals->Scene ? this->Internals->Scene->mNumAnimations : 0;
}

//----------------------------------------------------------------------------
std::string vtkF3DAssimpImporter::GetAnimationName(vtkIdType animationIndex)
{
  assert(animationIndex < this->GetNumberOfAnimations());
  assert(animationIndex >= 0);
  return this->Internals->Scene->mAnimations[animationIndex]->mName.C_Str();
}

//----------------------------------------------------------------------------
void vtkF3DAssimpImporter::EnableAnimation(vtkIdType animationIndex)
{
  assert(animationIndex < this->GetNumberOfAnimations());
  assert(animationIndex >= 0);
  this->Internals->ActiveAnimation = animationIndex;
}

//----------------------------------------------------------------------------
void vtkF3DAssimpImporter::DisableAnimation(vtkIdType vtkNotUsed(animationIndex))
{
  this->Internals->ActiveAnimation = -1;
}

//----------------------------------------------------------------------------
bool vtkF3DAssimpImporter::IsAnimationEnabled(vtkIdType animationIndex)
{
  assert(animationIndex < this->GetNumberOfAnimations());
  assert(animationIndex >= 0);
  return this->Internals->ActiveAnimation == animationIndex;
}

//----------------------------------------------------------------------------
bool vtkF3DAssimpImporter::GetTemporalInformation(vtkIdType animationIndex,
  double vtkNotUsed(frameRate), int& vtkNotUsed(nbTimeSteps), double timeRange[2],
  vtkDoubleArray* vtkNotUsed(timeSteps))
{
  assert(animationIndex < this->GetNumberOfAnimations());
  assert(animationIndex >= 0);

  double duration = this->Internals->Scene->mAnimations[animationIndex]->mDuration;
  double fps = this->Internals->Scene->mAnimations[animationIndex]->mTicksPerSecond;
  if (fps == 0.0)
  {
    fps = 1.0;
  }

  this->Internals->Description += "Animation \"";
  this->Internals->Description += this->GetAnimationName(animationIndex);
  this->Internals->Description += "\": ";
  this->Internals->Description += std::to_string(duration);
  this->Internals->Description += " ticks, ";
  this->Internals->Description += std::to_string(fps);
  this->Internals->Description += " fps.\n";

  // F3D do not care about timesteps, only set time range
  timeRange[0] = 0.0;
  timeRange[1] = duration / fps;
  return true;
}

//----------------------------------------------------------------------------
vtkIdType vtkF3DAssimpImporter::GetNumberOfCameras()
{
  return this->Internals->Cameras.size();
}

//----------------------------------------------------------------------------
std::string vtkF3DAssimpImporter::GetCameraName(vtkIdType camIndex)
{
  return this->Internals->Cameras[camIndex].first;
}

//----------------------------------------------------------------------------
void vtkF3DAssimpImporter::SetCamera(vtkIdType camIndex)
{
  this->Internals->ActiveCameraIndex = camIndex;
}

//----------------------------------------------------------------------------
void vtkF3DAssimpImporter::ImportCameras(vtkRenderer* renderer)
{
  this->Internals->ImportCameras(renderer);
}

//----------------------------------------------------------------------------
void vtkF3DAssimpImporter::ImportLights(vtkRenderer* renderer)
{
  this->Internals->ImportLights(renderer);
}

//----------------------------------------------------------------------------
void vtkF3DAssimpImporter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "FileName: " << this->FileName << "\n";
}
