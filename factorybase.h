#ifndef FACTORYBASE_H
#define FACTORYBASE_H

#include <string>

#include <QObject>

class Device;

class FactoryBase : public QObject
{
    Q_OBJECT
public:
    FactoryBase(){mDevice = nullptr;}
    virtual ~FactoryBase(){;}

    void setDevice(Device *aDevice)
    {
        mDevice = aDevice;
        emit deviceSet();
    }
    Device *getDevice() const {return  mDevice;}

signals:
    void deviceSet();

private:
    Device *mDevice;
};



#endif // FACTORYBASE_H
