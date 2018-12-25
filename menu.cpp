#include "menu.h"

#include <QDebug>

#include "gui/console.h"

#include <device/inputdevicemanager.h>

Menu::Menu(QObject *parent) : QObject(parent)
{
    mMenu.reset(new QMenu("Serial devices"));

    mAvailableDevicesMenu.reset(new QMenu("Available Devices"));
    mMenu->addMenu(mAvailableDevicesMenu.get());

    mConnectedDevicesMenu.reset(new QMenu("Connected Devices"));
    mMenu->addMenu(mConnectedDevicesMenu.get());
    connect(mConnectedDevicesMenu.get(), &QMenu::triggered, this, &Menu::onConnectedDevicesActionTriggered);

    setupSerialportSettingsMenu();

    connect(&InputDeviceManager::sGetInstance(), &InputDeviceManager::inputDeviceDisconnected, this, &Menu::onDeviceDisconnected);
    connect(&InputDeviceManager::sGetInstance(), &InputDeviceManager::inputDeviceConnected, this, &Menu::onConnectedDevices);
}

Menu::~Menu()
{

}

QMenu* Menu::getMenu()
{
    return mMenu.get();
}


void Menu::onNewDeviceAvailable(QString aName)
{
    QMenu *menu = mAvailableDevicesMenu->addMenu(aName);
    mAvailableDeviceMenuMap[aName] = menu;
    connect(menu, &QMenu::triggered, this, &Menu::onMenuActionTriggered);
    QAction *action = menu->addAction("Connect");
    action->setData(aName);
}


void Menu::onConnectedDevices(QString aName)
{
    qDebug() << __FUNCTION__;
    QMenu *menu = mConnectedDevicesMenu->addMenu(aName);
    mConnectedDeviceMenuMap[aName] = menu;

    if(mAvailableDeviceMenuMap.contains(aName))
    {
        QMenu *menu = mAvailableDeviceMenuMap[aName];
        QAction *remove = menu->menuAction();
        mAvailableDevicesMenu->removeAction(remove);
        menu->deleteLater();
        mAvailableDeviceMenuMap.remove(aName);
    }

    QAction *dis = menu->addAction("Disconnect");
    dis->setData(aName);
    QAction *con = menu->addAction("Show console");
    con->setData(aName);

}


void Menu::setupSerialportSettingsMenu()
{
    mSerialportSettings.reset(new QMenu("Serialport Settings"));
    mMenu->addMenu(mSerialportSettings.get());

}

void Menu::onConnectedDevicesActionTriggered(QAction *aAction)
{
    if(aAction->text() == "Show console")
    {
        QString deviceName = aAction->data().toString();
        Device *device = InputDeviceManager::sGetInstance().getInputDevice(deviceName);
        Console *console = new Console(device);
        console->setVisible(true);
        console->raise();
        connect(console, &Console::closed, console, &Console::deleteLater);
    }
    else if(aAction->text() == "Disconnect")
    {
        QString deviceName = aAction->data().toString();
        qDebug() << "Disconnect " << deviceName;
        InputDeviceManager::sGetInstance().disconnectInputDevice(deviceName);
    }
}

void Menu::onMenuActionTriggered(QAction *aAction)
{
    qDebug() << __FUNCTION__;
    if(aAction->text() == "Connect")
    {
        QString deviceName = aAction->data().toString();
        qDebug() << "Connect " << deviceName;
        InputDeviceManager::sGetInstance().connectInputDevice(deviceName);
    }
}

void Menu::onDeviceDisconnected(QString aDeviceName)
{
    qDebug() << __FUNCTION__;
    if(mAvailableDeviceMenuMap.contains(aDeviceName))
    {
        QMenu *menu = mAvailableDeviceMenuMap[aDeviceName];
        QAction *remove = menu->menuAction();
        mAvailableDevicesMenu->removeAction(remove);
        mAvailableDeviceMenuMap[aDeviceName]->deleteLater();
        mAvailableDeviceMenuMap.remove(aDeviceName);
    }
    if(mConnectedDeviceMenuMap.contains(aDeviceName))
    {
        QMenu *menu = mConnectedDeviceMenuMap[aDeviceName];
        QAction *remove = menu->menuAction();
        mConnectedDevicesMenu->removeAction(remove);
        mConnectedDeviceMenuMap[aDeviceName]->deleteLater();
        mConnectedDeviceMenuMap.remove(aDeviceName);
    }
}
