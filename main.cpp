#include "Gui/MainWidget.h"
#include <QSurfaceFormat>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{

	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	format.setStencilBufferSize(8);
	format.setVersion(4, 3);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);

    QApplication a(argc, argv);
    MainWidget w;
    w.show();
    return a.exec();
}
