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

#ifdef NO_GUI
App::App(int argc, char *argv[]) : QCoreApplication(argc, argv), inputDeviceManager_()
#else
App::App(int argc, char *argv[]) : QApplication(argc, argv), inputDeviceManager_()
#endif
{
    setOrganizationName("MySoft");
    setOrganizationDomain("mysoft.com");
    setApplicationName("JuneHwCom");
    setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("Brings serial port devices to june. \n Default serial port values:\n baudrate 9600 \n databits 8 \n no parity \n one stop bit \n no flow control\n");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption gui(QStringList() << "s" << "system_bar", "System bar");
    parser.addOption(gui);

    QCommandLineOption serverIp("ip", "server-ip", "Connect to server");
    serverIp.setDefaultValue("localhost");
    parser.addOption(serverIp);


    parser.process(*this);

    inputDeviceManager_.setUseDefaultSerialSettingFlag(true);
    connect(&inputDeviceManager_, &InputDeviceManager::inputDeviceAvailable, this, &App::onDeviceAvailable);
    connect(&inputDeviceManager_, &InputDeviceManager::inputDeviceConnected, this, &App::onDeviceConnected);
#ifndef NO_GUI
    if(parser.isSet(gui))
    {
        mSystemTrayUI.reset(new SystemTrayUI(inputDeviceManager_));
        connect(&inputDeviceManager_, &InputDeviceManager::inputDeviceAvailable, mSystemTrayUI.get(), &SystemTrayUI::onDeviceAvailable);
        //connect(mSystemTrayUI.get(), &SystemTrayUI::messageClicked, this, &App::onMessageClicked);
    }
#endif
    TagList::sGetInstance().setClientName(applicationName());
    TagList::sGetInstance().connectToServer(parser.value(serverIp), 5000);


    inputDeviceManager_.setDetectInputDevicesInterval(1000);
}


App::~App()
{

}


void App::onDeviceAvailable(QString aDeviceNAme)
{
    qDebug() << "App::Dvice";
    //InputDeviceManager::sGetInstance().connectInputDevice(aDeviceNAme);
    QString name = inputDeviceManager_.getDeviceManufacturer(aDeviceNAme);
    qDebug() << name;
    if(name == "1a86")
        inputDeviceManager_.connectInputDevice(aDeviceNAme);
    else if(name.contains("VictronEnergy"))
    {
        inputDeviceManager_.connectInputDevice(aDeviceNAme);
    }
}


void App::onMessageClicked(QString aDeviceNAme)
{
    qDebug() << __FUNCTION__;
    Device *device = inputDeviceManager_.getInputDevice(aDeviceNAme);
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
    Device *device = inputDeviceManager_.getInputDevice(aDeviceName);
    qDebug() << device->getManufacturer();
    if(device->getPid() == 42) // default atmega device
    {
        MessageHandlerManager::sGetInstance().createMessageHandlerForDevice(device);
    }
    else if(device->getManufacturer() == "VictronEnergy")
    {
        const std::string name = device->getDeviceName().toStdString();
    }
}
