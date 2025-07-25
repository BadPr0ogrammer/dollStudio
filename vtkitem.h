#pragma once

#include <QQuickVTKItem.h>

#include <QString>
#include <QTimer>
#include <QStandardItem>

#include <vtkNew.h>
#include <vtkObject.h>
#include <vtkObjectFactory.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCallbackCommand.h>
#include <vtkRenderTimerLog.h>
#include <vtkProgressBarWidget.h>

#include "vtkF3DMetaImporter.h"
#include "vtkF3DRenderer.h"
#include "vtkF3DAssimpImporter.h"

#include "animationManager.h"
#include "window_impl.h"
#include "scene_impl.h"
#include "options.h"

class aiNode;
namespace DS {
class App;
struct VtkItem : QQuickVTKItem
{
    Q_OBJECT
public:
    struct Data : vtkObject
    {
        static Data* New();
        vtkTypeMacro(Data, vtkObject);
        struct VtkItem*                     _vtkItem = nullptr;

        f3d::detail::window_impl*           _win = nullptr;
        f3d::detail::scene_impl*            _scene = nullptr;

        vtkSmartPointer<vtkCallbackCommand> _timer1;
    };    
    QString _fname;
    f3d::options _options;    
    QTimer _timer2;
    App* _app = nullptr;

    vtkUserData initializeVTK(vtkRenderWindow* renderWindow) override;
    void destroyingVTK(vtkRenderWindow* renderWindow, vtkUserData userData) override;

    bool openSource();
    void close();
    void play();
    void setupOpt();
    void setTreeView(Data* vtk);
    void traversTree(QStandardItem *parent, const aiNode *node);
public slots:
    void timerSlot();
};
}
