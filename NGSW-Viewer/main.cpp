#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <QApplication>

#include "MainWindow.h"

using namespace std;

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	MainWindow mainWin;
	mainWin.show();

	return app.exec();
}