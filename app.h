#ifndef APP_H
#define APP_H

#include <QCoreApplication>
#include <QApplication>
#include <memory>

class SystemTrayUI;
class InputDeviceManager;
class FactoryBase;

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
    std::vector<std::unique_ptr<FactoryBase>> mInfluxDbDevices;
};

#endif // APP_H
