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

int Settings::upDirection() const
{
	return contains("upDirection") ? value("upDirection").toInt() : defaultUpDirection();
}

void Settings::setUpDirection(int idx)
{
	const int existingValue = value("upDirection", defaultUpDirection()).toInt();
	if (idx == existingValue)
		return;
	setValue("upDirection", idx);
	emit upDirectionChanged();
}

int Settings::defaultUpDirection() const
{
	return 1;
}

int Settings::rightDirection() const
{
	return contains("rightDirection") ? value("rightDirection").toInt() : defaultRightDirection();
}

void Settings::setRightDirection(int idx)
{
	const int existingValue = value("rightDirection", defaultRightDirection()).toInt();
	if (idx == existingValue)
		return;
	setValue("rightDirection", idx);
	emit rightDirectionChanged();
}

int Settings::defaultRightDirection() const
{
	return 0;
}

void Settings::storeIt()
{
	sync();
}

f3d::direction_t Settings::getDirectionFromIdx(int idx)
{
	f3d::direction_t direction = f3d::direction_t{ 1, 0, 0 };
	switch (idx) {
	case 1:	direction = f3d::direction_t{ 0, 1, 0 };
		  break;
	case 2:	direction = f3d::direction_t{ 0, 0, 1 };
		  break;
	case 3:	direction = f3d::direction_t{ -1, 0, 0 };
		  break;
	case 4:	direction = f3d::direction_t{ 0, -1, 0 };
		  break;
	case 5:	direction = f3d::direction_t{ 0, 0, -1 };
		  break;
	}
	return direction;
}

}
