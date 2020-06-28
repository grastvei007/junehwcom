#ifndef FACTORYBASE_H
#define FACTORYBASE_H

#include <string>

class FactoryBase
{
public:
    FactoryBase(){}
    virtual std::string id(){return "unknown";}
   // virtual ~FactoryBase(){;}
};



#endif // FACTORYBASE_H
