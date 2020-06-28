#ifndef FACTORY_H
#define FACTORY_H

#include <map>
#include <string>
#include <iostream>

#include "factorybase.h"


template<typename Type>
FactoryBase* createType() { return new Type; }

class Factory
{
public:
    static Factory& sGetFactory()
    {
        static Factory sFactory;
        return sFactory;
    }

    FactoryBase* create(const std::string &aKey);

    template<typename Type>
     void addFactory(const std::string &aCreateKey);



private:
     Factory(){}

protected:
    typedef FactoryBase* (*Creator)();

    std::map<std::string, Creator> mConstructors;
};

FactoryBase* Factory::create(const std::string &aKey)
{
    Creator createor = mConstructors[aKey];
    return createor();
}


template<typename Type>
void Factory::addFactory(const std::string &aCreateKey)
{
    Creator creator = &createType<Type>;
    mConstructors.insert(std::make_pair(aCreateKey, creator));
}


#endif // FACTORY_H
