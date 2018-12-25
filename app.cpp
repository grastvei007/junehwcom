#include "app.h"

#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QStringList>
#include <QDebug>

#include <device/inputdevicemanager.h>

#include "gui/systemtrayui.h"
#include "gui/console.h"

#include <device/msg/message.h>
#include <device/msg/messagehandlermanager.h>

#include <tagsystem/taglist.h>

App::App(int argc, char *argv[]) : QApplication(argc, argv)
{
    setApplicationName("June hw communicator");
    setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("Brings serial port devices to june. \n Default serial port values:\n baudrate 9600 \n databits 8 \n no parity \n one stop bit \n no flow control\n");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption gui(QStringList() << "s" << "system_bar", "System bar");
    parser.addOption(gui);


    parser.process(*this);


    InputDeviceManager::sGetInstance().setUseDefaultSerialSettingFlag(true);
    connect(&InputDeviceManager::sGetInstance(), &InputDeviceManager::inputDeviceAvailable, this, &App::onDeviceAvailable);
    connect(&InputDeviceManager::sGetInstance(), &InputDeviceManager::inputDeviceConnected, this, &App::onDeviceConnected);

    if(parser.isSet(gui))
    {
        mSystemTrayUI.reset(new SystemTrayUI());
        connect(&InputDeviceManager::sGetInstance(), &InputDeviceManager::inputDeviceAvailable, mSystemTrayUI.get(), &SystemTrayUI::onDeviceAvailable);
        //connect(mSystemTrayUI.get(), &SystemTrayUI::messageClicked, this, &App::onMessageClicked);
    }

    InputDeviceManager::sGetInstance().setDetectInputDevicesInterval(1000);

    TagList::sGetInstance().connectToServer("localhost", 5000);

}


App::~App()
{

}


void App::onDeviceAvailable(QString aDeviceNAme)
{
    //InputDeviceManager::sGetInstance().connectInputDevice(aDeviceNAme);
}


void App::onMessageClicked(QString aDeviceNAme)
{
    qDebug() << __FUNCTION__;
    Device *device = InputDeviceManager::sGetInstance().getInputDevice(aDeviceNAme);
    if(!device)
    {
        qDebug() << "Device not connected, " << aDeviceNAme;
        return;
    }
    device->setOverideDataRead(true);
}


void App::onDeviceConnected(QString aDeviceName)
{
    qDebug() << __FUNCTION__;
    Device *device = InputDeviceManager::sGetInstance().getInputDevice(aDeviceName);
    if(device->getPid() == 42) // default atmega device
    {
        MessageHandlerManager::sGetInstance().createMessageHandlerForDevice(device);
    }
}
