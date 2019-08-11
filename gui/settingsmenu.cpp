#include "settingsmenu.h"

#include "settings.h"

SettingsMenu::SettingsMenu(QString aName, QWidget *parent) : QMenu(aName, parent)
{
    QAction *atmega = addAction("Auto connect atmega");
    atmega->setCheckable(true);
    atmega->setVisible(true);
    atmega->setChecked(Settings::sGetInstance().getAutoConnectAtmega());
    connect(&Settings::sGetInstance(), &Settings::autoConncectAtmegaUnoDevicesChanged, atmega, &QAction::setChecked);
    connect(atmega, &QAction::toggled, &Settings::sGetInstance(), &Settings::setAutoConnectAtmegaDevices);

    QAction *victron = addAction("Auto connect victron");
    victron->setCheckable(true);
    victron->setVisible(true);
    victron->setChecked(Settings::sGetInstance().getAutoConnectVictron());
    connect(&Settings::sGetInstance(), &Settings::autoConnectVictronDevicesChanged, victron, &QAction::setChecked);
    connect(victron, &QAction::toggled, &Settings::sGetInstance(), &Settings::setAutoConnectVictronDevices);


    setVisible(true);
}
