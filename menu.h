#ifndef MENU_H
#define MENU_H

#include <QObject>
#include <QMenu>
#include <QMap>

#include <memory>

class QAction;

class Menu : public QObject
{
    Q_OBJECT
public:
    explicit Menu(QObject *parent = nullptr);
    ~Menu();

    QMenu* getMenu();
signals:

public slots:
    void onNewDeviceAvailable(QString aName);
    void onConnectedDevices(QString aName);

private:
    void setupSerialportSettingsMenu();
    void setupViewMenu();

private slots:
    void onConnectedDevicesActionTriggered(QAction *aAction);
    void onMenuActionTriggered(QAction *aAction);

    void onDeviceDisconnected(QString aDeviceName);
    void onShowTagSocketListActionTriggered(bool aChecked);
    void onSpy(QString aDeviceName);

private:
    std::unique_ptr<QMenu> mMenu;
    std::unique_ptr<QMenu> mAvailableDevicesMenu;
    std::unique_ptr<QMenu> mConnectedDevicesMenu;
    std::unique_ptr<QMenu> mSerialportSettings;
    std::unique_ptr<QMenu> mViewMenu;

    std::unique_ptr<QWidget> mTagSocketListViewWidget;

    QMap<QString, QMenu*> mAvailableDeviceMenuMap;
    QMap<QString, QMenu*> mConnectedDeviceMenuMap;

};

#endif // MENU_H
