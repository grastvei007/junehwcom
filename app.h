#ifndef APP_H
#define APP_H

#include <QApplication>
#include <memory>

class SystemTrayUI;
class InputDeviceManager;

class App : public QApplication
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
    std::unique_ptr<SystemTrayUI> mSystemTrayUI;
};

#endif // APP_H
