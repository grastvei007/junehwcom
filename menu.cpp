#include "menu.h"

#include <QDebug>

#include "gui/console.h"
#include "gui/settingsmenu.h"

#include <device/inputdevicemanager.h>
#include <tagsystem/tagsocketlistview.h>

#include <device/msg/messagehandlermanager.h>
#include <device/msg/messagehandler.h>

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

    setupViewMenu();

    mMenu->addMenu(new SettingsMenu("Settings"));
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
    QAction *spy = menu->addAction("Spy");
    spy->setData(aName);

}


void Menu::setupSerialportSettingsMenu()
{
    mSerialportSettings.reset(new QMenu("Serialport Settings"));
    mMenu->addMenu(mSerialportSettings.get());

    QMenu *baudMenu = mSerialportSettings->addMenu("BaudRate");
    QAction *action9600 = baudMenu->addAction("9600");
    action9600->setCheckable(true);
    action9600->setChecked(true);

    QAction *action19200 = baudMenu->addAction("19200");
    action19200->setCheckable(true);
    action19200->setChecked(false);

    QAction *action38400 = baudMenu->addAction("38400");
    action38400->setCheckable(true);
    action38400->setChecked(false);

    QAction *action115200 = baudMenu->addAction("115200");
    action115200->setCheckable(true);
    action115200->setChecked(false);

}

void Menu::onConnectedDevicesActionTriggered(QAction *aAction)
{
    if(aAction->text() == "Show console")
    {
        QString deviceName = aAction->data().toString();
        Device *device = InputDeviceManager::sGetInstance().getInputDevice(deviceName);
        Console *console = new Console(device);
        console->setAttribute(Qt::WA_DeleteOnClose, true);
        console->setAttribute(Qt::WA_QuitOnClose, false);
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
    else if(aAction->text() == "Spy")
    {
        onSpy(aAction->data().toString());
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


void Menu::setupViewMenu()
{
    mViewMenu.reset(new QMenu("View"));
    QAction *action = mViewMenu->addAction("Show TagSocket List");
    connect(action, &QAction::triggered, this, &Menu::onShowTagSocketListActionTriggered);

    mMenu->addMenu(mViewMenu.get());
}


void Menu::onShowTagSocketListActionTriggered(bool aChecked)
{
    mTagSocketListViewWidget.release();
    mTagSocketListViewWidget.reset(new TagSocketListView);

    mTagSocketListViewWidget->setAttribute(Qt::WA_DeleteOnClose, true);
    mTagSocketListViewWidget->setAttribute(Qt::WA_QuitOnClose, false);
    mTagSocketListViewWidget->setVisible(true);
    mTagSocketListViewWidget->show();
}


void Menu::onSpy(QString aDeviceName)
{
    Device *device = InputDeviceManager::sGetInstance().getInputDevice(aDeviceName);
    if(!device)
        return;
    MessageHandler *mh = MessageHandlerManager::sGetInstance().getMessageHandlerForDevice(device);
    if(!mh)
        return;

    Console *console = new Console();
    console->setAttribute(Qt::WA_DeleteOnClose, true);
    console->setAttribute(Qt::WA_QuitOnClose, false);
    console->setVisible(true);
    console->raise();

    //connect(mh, &MessageHandler::debugMessageRead, console, &Console::setInData);
    //connect(mh, &MessageHandler::debugMessageWrite, console, &Console::setOutData);

}
