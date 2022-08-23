#include "dialog.h"
#include "register.h"
#include <QApplication>
#include "tcpnetwork.h"
#include <QProcess>
tcpnetwork *socket;
QString token;
QString uuid;
QJsonObject remembered_user;
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    socket=new tcpnetwork();

    QString cmdstr = "wmic csproduct get uuid";
    QProcess myp;
    myp.start(cmdstr);
    myp.waitForFinished();

    uuid = myp.readAllStandardOutput();
    myp.kill();
    myp.close();

    QStringList ulist = uuid.split(" ");

    uuid.remove(ulist.first(),Qt::CaseInsensitive);
    uuid.remove(ulist.last());
    uuid.replace("\r", "");
    uuid.replace("\n", "");
    uuid.remove(" ");

    QDir dataDir;
    if(!dataDir.exists("userData")){
        dataDir.mkdir("userData");
    }
    dataDir.cd("userData");
    if(!dataDir.exists("rememberedUser")){
        QFile file(dataDir.filePath("rememberedUser"));
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.write("{}");
        file.close();
    }
    else{

    }
    QFile file(dataDir.filePath("rememberedUser"));
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray arr = file.readAll();
    file.close();
    remembered_user = QJsonDocument::fromJson(arr).object();


    Dialog w;
    w.show();
    return a.exec();
}
