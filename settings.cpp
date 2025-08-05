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

bool Settings::showAxis() const
{
	return contains("showAxis") ? value("showAxis").toBool() : defaultShowAxis();
}

void Settings::setShowAxis(bool show)
{
	const bool existingValue = value("showAxis", defaultShowAxis()).toBool();
	if (show == existingValue)
		return;
	setValue("showAxis", show);
	emit showAxisChanged();
}

bool Settings::defaultShowAxis() const
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
