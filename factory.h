#ifndef FACTORY_H
#define FACTORY_H

#include <map>
#include <string>
#include <iostream>

//#include "factorybase.h"

class FactoryBase;

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

    FactoryBase* createInstance(const std::string &aKey)
    {
        if(mConstructors.find(aKey) == mConstructors.end())
            return nullptr;
        Creator createor = mConstructors[aKey];
        return createor();
    }


    template<typename Type>
     void addFactory(const std::string &aCreateKey);

private:
     Factory(){}

protected:
    typedef FactoryBase* (*Creator)();

    std::map<std::string, Creator> mConstructors;
};


template<typename Type>
void Factory::addFactory(const std::string &aCreateKey)
{
    Creator creator = &createType<Type>;
    mConstructors.insert(std::make_pair(aCreateKey, creator));
}


#endif // FACTORY_H
