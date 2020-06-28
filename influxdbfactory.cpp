#include "influxdbfactory.h"

#include "influxdb/bmv712smart.h"

void InfluxDbFactory::createFactory()
{
    Factory::sGetFactory().addFactory<Bmv712Smart>("BMV712_Smart");
}
