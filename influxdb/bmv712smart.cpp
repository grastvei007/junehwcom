#include "bmv712smart.h"

Bmv712Smart::Bmv712Smart() : FactoryBase()
{
    connect(this, &Bmv712Smart::deviceSet, this, &Bmv712Smart::onDeviceSet);
}


Bmv712Smart::~Bmv712Smart()
{

}


std::string Bmv712Smart::id()
{
    return std::string("BMV712_Smart");
}


void Bmv712Smart::onDeviceSet()
{

}
