#include <QStandardItemModel>
#include <QStringList>

#include "app.h"
#include "manager.h"
#include "vtkitem.h"

namespace DS
{
Manager::Manager(App* app) : _app(app)
{
    _treemodel = new QStandardItemModel(_app->_engine);
    _listmodel = new QStringListModel(_app->_engine);
}

void Manager::setConnect()
{
    _vtk->setupOpt();
}

void Manager::treeSelChanged(const QModelIndex& idx)
{
    if (!idx.isValid() || !idx.data(Qt::UserRole + 1).isValid()) {
        qDebug() << "Tree model index is invalid!";
        return;
    }
    const aiNode* node = idx.data(Qt::UserRole + 1).value<const aiNode*>();
    if (!node) {
        qDebug() << "Tree model data is invalid!";
        return;
    }
    QStringList list; 
    list << "Node name: " + QString(node->mName.C_Str());
    if (node->mNumMeshes) {
        for (int i = 0; i < node->mNumMeshes; ++i) {
            int m = node->mMeshes[i];
            const aiMesh* mesh = _aiscene->mMeshes[m];
            list << QString("Mesh idx: ") + QString::number(i);
            list << QString("AnimMeshes: ") + QString::number(mesh->mNumAnimMeshes);
            list << QString("Bones: ") + QString::number(mesh->mNumBones);
            list << QString("Faces: ") + QString::number(mesh->mNumFaces);
            list << QString("Vertices: ") + QString::number(mesh->mNumVertices);
        }
    }
    else {
        for (int i = 0; i < node->mMetaData->mNumProperties; i++) {
            const char* key = node->mMetaData->mKeys[i].C_Str();
            const aiMetadataEntry& entry = node->mMetaData->mValues[i];
            if (entry.mType == AI_AISTRING) {
                list << QString("Key: ") + QString(key);
                list << QString("Value: ") + QString(static_cast<aiString*>(entry.mData)->C_Str());
            }
            if (entry.mType == AI_BOOL) {
                if (std::string(key) == std::string("IsBone")) {
                    bool isBone = false;
                    node->mMetaData->Get(key, isBone);
                    if (isBone)
                        list << QString("IsBone: true");
                }
            }
        }
    }
    _listmodel->setStringList(list);
}

void Manager::openSource(const QUrl& url, bool clear)
{
    _vtk->_fname = url.toLocalFile();
    _vtk->openSource(clear);
}

void Manager::playFlag()
{
    _vtk->play();
}

}
