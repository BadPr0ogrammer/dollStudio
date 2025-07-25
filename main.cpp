#include <QQuickVTKItem.h>

#include "app.h"
#include "factory.h"

int main(int argc, char *argv[])
{
    QQuickVTKItem::setGraphicsApi();
    f3d::factory::instance()->autoload();

    DS::App app(argc, argv);
    if (app._qmlEng->rootObjects().isEmpty())
        return 1;
    return app._application.get()->exec();
}
