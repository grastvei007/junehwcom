#include "systemtrayui.h"

#include <QSystemTrayIcon>
#include <QDebug>

SystemTrayUI::SystemTrayUI()
{
    mSystemTrayIcon.reset(new QSystemTrayIcon());
    mSystemTrayIcon->setIcon(QIcon(":/icon"));
    mSystemTrayIcon->setVisible(true);
    mSystemTrayIcon->setToolTip("June serialport com");
    connect(mSystemTrayIcon.get(), &QSystemTrayIcon::messageClicked, this, &SystemTrayUI::onMessageClicked);

    mContestMenu.reset(new Menu);
    mSystemTrayIcon->setContextMenu(mContestMenu->getMenu());
}

SystemTrayUI::~SystemTrayUI()
{

}


void SystemTrayUI::onDeviceAvailable(QString aDeviceName)
{
    qDebug() << __FUNCTION__ << aDeviceName;
    mSystemTrayIcon->showMessage("June", aDeviceName);
    mDeviceName = aDeviceName;
    mContestMenu->onNewDeviceAvailable(aDeviceName);
}


void SystemTrayUI::onMessageClicked()
{
    emit messageClicked(mDeviceName);
}
