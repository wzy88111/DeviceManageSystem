#include "DeviceManageSystem.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.init();
    w.show();
    return a.exec();
}
