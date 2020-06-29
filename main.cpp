#include "app.h"

#include "influxdbfactory.h"

int main(int argc, char *argv[])
{
    InfluxDbFactory::createFactory();

    App a(argc, argv);

    return a.exec();
}
