#pragma once

#include <vector>
#include <iostream>
#include <filesystem>

#include <QSettings>
#include <QColor>
#include <QUrl>
#include <QApplication>
#include <QQmlEngine>
#include <QtQmlIntegration/qqmlintegration.h>

namespace DS {
class Settings : public QSettings
{
	Q_OBJECT
		Q_PROPERTY(bool showAxes READ showAxes WRITE setShowAxes NOTIFY showAxesChanged)
		Q_PROPERTY(bool showGrid READ showGrid WRITE setShowGrid NOTIFY showGridChanged)
		QML_ELEMENT
public:
	Settings(QObject* parent = nullptr);

	bool showAxes() const;
	void setShowAxes(bool show);
	bool defaultShowAxes() const;

	bool showGrid() const;
	void setShowGrid(bool show);
	bool defaultShowGrid() const;

	Q_INVOKABLE void storeIt();
signals:
	void showAxesChanged();
	void showGridChanged();
};
}