#include "settings.h"

#include <QDebug>
#include <QFile>
#include <QKeySequence>
#include <QLoggingCategory>
#include <QVector>

namespace DS {
Settings::Settings(QObject* parent) :
	QSettings(QSettings::NativeFormat, QSettings::UserScope,
		QApplication::organizationName(), QApplication::applicationName(), parent)
{
}

bool Settings::showAxes() const
{
	return contains("showAxes") ? value("showAxes").toBool() : defaultShowAxes();
}

void Settings::setShowAxes(bool show)
{
	const bool existingValue = value("showAxes", defaultShowAxes()).toBool();
	if (show == existingValue)
		return;
	setValue("showAxes", show);
	emit showAxesChanged();
}

bool Settings::defaultShowAxes() const
{
	return false;
}

bool Settings::showGrid() const
{
	return contains("showGrid") ? value("showGrid").toBool() : defaultShowGrid();
}

void Settings::setShowGrid(bool show)
{
	const bool existingValue = value("showGrid", defaultShowGrid()).toBool();
	if (show == existingValue)
		return;
	setValue("showGrid", show);
	emit showGridChanged();
}

bool Settings::defaultShowGrid() const
{
	return false;
}

void Settings::storeIt()
{
	sync();
}

}
