#include "app.h"
#include <QtDebug>
#include <QFile>
#include <QTextStream>


void myMessageHandler(QtMsgType type, const QMessageLogContext &, const QString & msg)
{
    QString text;
    switch (type)
    {
    case QtInfoMsg:
        text = QString("Info: %1").arg(msg);
        break;
    case QtDebugMsg:
        text = QString("Debug: %1").arg(msg);
        break;
    case QtWarningMsg:
        text = QString("Warning: %1").arg(msg);
        break;
    case QtCriticalMsg:
        text = QString("Critical: %1").arg(msg);
        break;
    case QtFatalMsg:
        text = QString("Fatal: %1").arg(msg);
        break;
    }
    QFile outFile("log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << text << Qt::endl;

    // fatal message should quit
    if(type == QtFatalMsg)
        abort();
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageHandler);
    App a(argc, argv);

    return a.exec();
}
