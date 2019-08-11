#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

class Settings : public QObject
{
    Q_OBJECT
public:
    static Settings& sGetInstance();

    bool getAutoConnectAtmega() const;
    bool getAutoConnectVictron() const;

public slots:
    void setAutoConnectVictronDevices(bool aLoad);
    void setAutoConnectAtmegaDevices(bool aLoad);
signals:
    void autoConnectVictronDevicesChanged(bool);
    void autoConncectAtmegaUnoDevicesChanged(bool);


private:
    Settings();
    void saveSettings();
    void loadSettings();

private:
    bool mAutoConnectVicronDevices;
    bool mAutoConnectAtmegaDevices;

};

#endif // SETTINGS_H
