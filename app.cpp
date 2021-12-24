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

#include "factorybase.h"
#include "factory.h"

App::App(int argc, char *argv[]) : QApplication(argc, argv)
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

    TagList::sGetInstance().setClientName(applicationName());
    TagList::sGetInstance().connectToServer("localhost", 5000);
    //TagList::sGetInstance().connectToServer("192.168.39.117", 5000);

}


App::~App()
{

}


void App::onDeviceAvailable(QString aDeviceNAme)
{
    qDebug() << "App::Dvice";
    //InputDeviceManager::sGetInstance().connectInputDevice(aDeviceNAme);
    QString name = InputDeviceManager::sGetInstance().getDeviceManufacturer(aDeviceNAme);
    qDebug() << name;
    if(name == "1a86")
        InputDeviceManager::sGetInstance().connectInputDevice(aDeviceNAme);
    else if(name.contains("VictronEnergy"))
    {
        InputDeviceManager::sGetInstance().connectInputDevice(aDeviceNAme);
    }
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
    qDebug() << device->getManufacturer();
    if(device->getPid() == 42) // default atmega device
    {
        MessageHandlerManager::sGetInstance().createMessageHandlerForDevice(device);
    }
    else if(device->getManufacturer() == "VictronEnergy")
    {
        const std::string name = device->getDeviceName().toStdString();
        std::unique_ptr<FactoryBase> fb;
        fb.reset(Factory::sGetFactory().createInstance(name));
        if(fb)
        {
            fb->setDevice(device);
            mInfluxDbDevices.push_back(std::move(fb));
        }
    }
}
