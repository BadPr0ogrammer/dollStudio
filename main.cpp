#include <QQuickVTKItem.h>

#include "app.h"
#include "factory.h"
#include "init.h"

int main(int argc, char* argv[])
{
	QQuickVTKItem::setGraphicsApi();
	f3d::factory::instance()->autoload();
	f3d::detail::init::initialize();

	DS::App app(argc, argv);
	if (app._engine->rootObjects().isEmpty())
		return 1;

	app.setup();
	return app._application->exec();
}
