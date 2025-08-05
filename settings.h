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
        Q_PROPERTY(bool showAxis READ showAxis WRITE setShowAxis NOTIFY showAxisChanged)
        Q_PROPERTY(bool showGrid READ showGrid WRITE setShowGrid NOTIFY showGridChanged)
        QML_ELEMENT
public:
    Settings(QObject* parent = nullptr);

    bool showAxis() const;
    void setShowAxis(bool show);
    bool defaultShowAxis() const;

    bool showGrid() const;
    void setShowGrid(bool show);
    bool defaultShowGrid() const;

    Q_INVOKABLE void storeIt();
signals:
    void showAxisChanged();
    void showGridChanged();
};
}