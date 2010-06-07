#include <QtGui/QApplication>
#include "startpagewidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Tadaaki Matsumoto");
    QCoreApplication::setOrganizationDomain("tadaaki.matsumoto");
    QCoreApplication::setApplicationName("SmartFmListEditor");

    StartPageWidget w;
    w.show();

    return a.exec();
}
