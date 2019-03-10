#include "console.h"
#include <QPalette>
#include <QScrollBar>
#include <QCloseEvent>

#include <device/device.h>

Console::Console(QWidget *parent) :
    mDevice(nullptr)
{
    document()->setMaximumBlockCount(100);
    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::green);
    setPalette(p);
}

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
    if(mDevice)
        mDevice->setDataListenFlag(false); // release serial stream.
}


void Console::setData(QString aInfo, QByteArray aData)
{
    QByteArray d;
    d.append(aInfo);
    d.append(aData);
    putData(d);
}


void Console::putData(QByteArray aData)
{
    insertPlainText(QString(aData).toLatin1());
    insertPlainText("\n");
    QScrollBar *bar = verticalScrollBar();
    bar->setValue(bar->maximum());

}


void Console::closeEvent(QCloseEvent *event)
{
    emit closed();
}


void Console::setInData(QByteArray aData)
{
    setData("In:  ", aData);
}


void Console::setOutData(QByteArray aData)
{
    setData("Out: ", aData);
}
