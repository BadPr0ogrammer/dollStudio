#ifndef MANAGER_H
#define MANAGER_H

#include <memory>

#include <QObject>
#include <QUrl>
#include <QtQmlIntegration/qqmlintegration.h>

#include "vtkitem.h"

namespace DS {
class TreeModel;
class Manager : public QObject
{
    Q_OBJECT

    QML_ELEMENT
    QML_UNCREATABLE("")
    Q_MOC_INCLUDE("app.h")

public:
    Q_INVOKABLE void openSource(const QUrl &url);
    Q_INVOKABLE void playFlag();

    VtkItem *_vtk = nullptr;
};
}
#endif
