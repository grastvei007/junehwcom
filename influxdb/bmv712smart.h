#ifndef BMV712SMART_H
#define BMV712SMART_H

#include <string>

#include <QObject>
#include "factorybase.h"

class Bmv712Smart : public FactoryBase
{
public:
    Bmv712Smart();
    virtual ~Bmv712Smart();

    std::string id();


private slots:
    void onDeviceSet();


};

#endif // BMV712SMART_H
