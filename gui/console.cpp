#include "console.h"
#include <QPalette>
#include <QScrollBar>
#include <QCloseEvent>

#include <device/device.h>

Console::Console(Device *aDevice, QWidget *parent) : QPlainTextEdit(parent),
    mDevice(aDevice)
{
    document()->setMaximumBlockCount(100);
    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::green);
    setPalette(p);

    mDevice->setDataListenFlag(true);
    connect(mDevice, &Device::dataRecieved, this, &Console::putData);
}

Console::~Console()
{
    mDevice->setDataListenFlag(false); // release serial stream.
}

void Console::putData(QByteArray aData)
{
    insertPlainText(QString(aData).toLatin1());

    QScrollBar *bar = verticalScrollBar();
    bar->setValue(bar->maximum());

}


void Console::closeEvent(QCloseEvent *event)
{
    emit closed();
}
