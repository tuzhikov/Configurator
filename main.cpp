#include "src/mainwindow.h"
#include "src/CONTROLLER/controller.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(Resources);
    QTranslator trn;
    trn.load(QString(":/TS/language_ru"));
    a.installTranslator(&trn);
    //setting parametr
    QApplication::setOrganizationName("Software Ltd.");
    QApplication::setApplicationName("Configurator");
    Controller controller;
    //create main windows
    MainWindow w(nullptr,&controller);
    w.setWindowIcon(QPixmap(":/ico/icotraffic.ico"));
    w.setWindowTitle(QObject::tr("Configurator"));
    w.move((QApplication::desktop()->width()-w.width())/ 2,
               (QApplication::desktop()->height()-w.height())/ 2
               );
    w.show();

    return a.exec();
}
