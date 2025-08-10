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

bool Settings::loadOnStartup() const
{
	return contains("loadOnStartup") ? value("loadOnStartup").toBool() : defaultLoadOnStartup();
}

void Settings::setLoadOnStartup(bool loadLastOnStartup)
{
	const bool existingValue = value("loadOnStartup", defaultLoadOnStartup()).toBool();
	if (loadLastOnStartup == existingValue)
		return;

	setValue("loadOnStartup", loadLastOnStartup);
	emit loadOnStartupChanged();
}

bool Settings::defaultLoadOnStartup() const
{
	return false;
}

QStringList Settings::recentFiles() const
{
	return contains("recentFiles") ? value("recentFiles").toStringList() : QStringList();
}

void Settings::addRecentFile(const QString& filePath)
{
	QStringList files = recentFiles();
	const int existingIndex = files.indexOf(filePath);
	// If it already exists, remove it and move it to the top.
	if (existingIndex != -1)
		files.removeAt(existingIndex);

	// Add the file to the top of the list.
	files.prepend(filePath);

	// Respect the file limit.
	if (files.size() > 20)
		files.removeLast();

	setValue("recentFiles", files);
	emit recentFilesChanged();
}

void Settings::clearRecentFiles()
{
	if (recentFiles().isEmpty())
		return;

	setValue("recentFiles", QStringList());
	emit recentFilesChanged();
}

void Settings::removeInvalidRecentFiles()
{
	if (!contains("recentFiles"))
		return;

	bool changed = false;
	QStringList files = value("recentFiles").toStringList();
	for (int i = 0; i < files.size(); ) {
		const QString filePath = files.at(i);
		if (filePath.isEmpty() || !QFile::exists(QUrl(filePath).toLocalFile())) {
			files.removeAt(i);
			changed = true;
		}
		else {
			++i;
		}
	}

	setValue("recentFiles", files);

	if (changed)
		emit recentFilesChanged();
}

QString Settings::displayableFilePath(const QString& filePath) const
{
	return QUrl(filePath).toLocalFile();
}

}
