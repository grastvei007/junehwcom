#ifndef INFLUXDBFACTORY_H
#define INFLUXDBFACTORY_H

#include "factory.h"

class InfluxDbFactory : public Factory
{
public:
    static void createFactory();
};

#endif // INFLUXDBFACTORY_H
