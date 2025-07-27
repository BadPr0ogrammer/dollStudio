#pragma once

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTreeView>
#include <QStandardItemModel>

namespace DS
{
class Manager;
class App
{
public:
	App(int& argc, char** argv);
	~App();
	void setup();

	QGuiApplication*		_application = nullptr;
	QQmlApplicationEngine*	_engine = nullptr;
	Manager*				_manager = nullptr;
};
}
