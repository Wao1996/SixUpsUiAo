#include "SixUpsUiAo.h"
#include <QtWidgets/QApplication>
#include <QStyleFactory>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	//QApplication::setStyle(QStyleFactory::create("fusion"));
    SixUpsUiAo w;
    w.show();
	w.setWindowTitle("六自由度调姿平台控制程序");
    return a.exec();
}
