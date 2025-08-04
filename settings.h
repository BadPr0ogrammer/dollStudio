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
        Q_PROPERTY(bool showAxis READ showAxis WRITE setShowAxis NOTIFY showAxisChanged)
        QML_ELEMENT
public:
    Settings(QObject* parent = nullptr);

    bool showAxis() const;
    void setShowAxis(bool show);
    bool defaultShowAxis() const;

    Q_INVOKABLE void storeIt();
signals:
    void showAxisChanged();
};
}