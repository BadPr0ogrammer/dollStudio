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

#include "types.h"

namespace DS {
class Settings : public QSettings
{
	Q_OBJECT
		Q_PROPERTY(bool showAxes READ showAxes WRITE setShowAxes NOTIFY showAxesChanged)
		Q_PROPERTY(bool showGrid READ showGrid WRITE setShowGrid NOTIFY showGridChanged)
		Q_PROPERTY(int upDirection READ upDirection WRITE setUpDirection NOTIFY upDirectionChanged)
		Q_PROPERTY(int rightDirection READ rightDirection WRITE setRightDirection NOTIFY rightDirectionChanged)
		QML_ELEMENT
public:
	Settings(QObject* parent = nullptr);

	bool showAxes() const;
	void setShowAxes(bool show);
	bool defaultShowAxes() const;

	bool showGrid() const;
	void setShowGrid(bool show);
	bool defaultShowGrid() const;

	int  upDirection() const;
	void setUpDirection(int idx);
	int  defaultUpDirection() const;
	int  rightDirection() const;
	void setRightDirection(int idx);
	int  defaultRightDirection() const;

	Q_INVOKABLE void storeIt();
signals:
	void showAxesChanged();
	void showGridChanged();
	void upDirectionChanged();
	void rightDirectionChanged();
public:
	f3d::direction_t getDirectionFromIdx(int idx);
};
}