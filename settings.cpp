#include "settings.h"
#include <QSettings>

Settings& Settings::sGetInstance()
{
    static Settings sSettings;
    return sSettings;
}

Settings::Settings()
{
    loadSettings();
}


void Settings::loadSettings()
{
    QSettings settings("settings", QSettings::Format::NativeFormat);

    mAutoConnectAtmegaDevices = settings.value("load/atmega", false).toBool();
    mAutoConnectVicronDevices = settings.value("load/victron", false).toBool();
}

void Settings::saveSettings()
{
    QSettings settings("settings", QSettings::Format::NativeFormat);
    settings.setValue("load/atmega", mAutoConnectAtmegaDevices);
    settings.setValue("load/victron", mAutoConnectVicronDevices);
    settings.sync();
}

void Settings::setAutoConnectAtmegaDevices(bool aLoad)
{
    if(mAutoConnectAtmegaDevices == aLoad)
        return;

    mAutoConnectAtmegaDevices = aLoad;
    emit autoConncectAtmegaUnoDevicesChanged(mAutoConnectAtmegaDevices);
    saveSettings();
}

void Settings::setAutoConnectVictronDevices(bool aLoad)
{
    if(mAutoConnectVicronDevices == aLoad)
        return;

    mAutoConnectVicronDevices = aLoad;
    emit autoConnectVictronDevicesChanged(mAutoConnectVicronDevices);
    saveSettings();
}


bool Settings::getAutoConnectAtmega() const
{
    return mAutoConnectAtmegaDevices;
}


bool Settings::getAutoConnectVictron() const
{
    return mAutoConnectVicronDevices;
}
