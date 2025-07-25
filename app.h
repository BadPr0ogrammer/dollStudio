#ifndef APP_H
#define APP_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QStandardItemModel>

#include <memory>

namespace DS {
class Manager;
class TreeModel;
class App
{
public:
    App(int &argc, char **argv);
    ~App();

    std::unique_ptr<QGuiApplication> _application;
    std::unique_ptr<QQmlApplicationEngine> _qmlEng;
    std::unique_ptr<Manager> _manager;
    std::unique_ptr<QStandardItemModel> _treemodel;
};
}
#endif
