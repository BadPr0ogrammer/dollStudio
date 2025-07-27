#include <QQuickVTKItem.h>

#include "app.h"
#include "factory.h"

int main(int argc, char* argv[])
{
	QQuickVTKItem::setGraphicsApi();
	f3d::factory::instance()->autoload();

	DS::App app(argc, argv);
	if (app._engine->rootObjects().isEmpty())
		return 1;

	app.setup();
	return app._application->exec();
}
