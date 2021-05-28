#include "DWindow.h"
#include <QtWidgets/QApplication>


int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	DWindow w;
	w.show();
	return a.exec();
}
