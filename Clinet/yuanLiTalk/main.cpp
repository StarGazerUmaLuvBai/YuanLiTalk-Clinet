#include "dialog.h"
#include "register.h"
#include <QApplication>
#include "tcpnetwork.h"

tcpnetwork *socket;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    socket=new tcpnetwork();
    Dialog w;
    w.show();
    return a.exec();
}
