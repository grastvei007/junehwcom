#ifndef SYSTEMTRAYUI_H
#define SYSTEMTRAYUI_H

#include <QString>
#include <QObject>
#include <memory>

#include "menu.h"

class QSystemTrayIcon;

class SystemTrayUI : public QObject
{
    Q_OBJECT
public:
    SystemTrayUI();
    ~SystemTrayUI();

signals:
    void messageClicked(QString); ///< message clicked, emit device name.

public slots:
    void onDeviceAvailable(QString aDeviceName);

private slots:
    void onMessageClicked();

private:
    std::unique_ptr<QSystemTrayIcon> mSystemTrayIcon;
    std::unique_ptr<Menu> mContestMenu;

    QString mDeviceName;
};

#endif // SYSTEMTRAYUI_H
