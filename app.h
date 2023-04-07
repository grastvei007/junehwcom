#ifndef APP_H
#define APP_H

#include <QCoreApplication>
#include <QApplication>
#include <memory>

#include <device/inputdevicemanager.h>

class SystemTrayUI;
class InputDeviceManager;

#ifdef NO_GUI
class App : public QCoreApplication
#else
class App : public QApplication
#endif
{
    Q_OBJECT
public:
    App(int argc, char *argv[]);
    ~App();

signals:

public slots:

private slots:
    void onDeviceAvailable(QString aDeviceNAme);
    void onMessageClicked(QString aDeviceName);
    void onDeviceConnected(QString aDeviceName);

private:
#ifndef NO_GUI
    std::unique_ptr<SystemTrayUI> mSystemTrayUI;
#endif

    InputDeviceManager inputDeviceManager_;
    bool writeToConsole_ = false;
};

#endif // APP_H
