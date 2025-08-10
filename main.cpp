#include <QQuickVTKItem.h>

#include "app.h"
#include "init.h"

int main(int argc, char* argv[])
{
	QQuickVTKItem::setGraphicsApi();
	f3d::detail::init::initialize();

	DS::App app(argc, argv);
	if (app._engine->rootObjects().isEmpty())
		return 1;
	app.setConnect();
	return app._application->exec();
}
