QT -= gui
QT += widgets serialport

DEFINES += NO_GUI

CONFIG += c++14 console
CONFIG -= app_bundle

release: TARGET = junehwcom
debug: TARGET = junehwcomd

DESTDIR = $$(DEV_BIN)

release: BUILDDIR = build/release
debug:   BUILDDIR = build/debug

OBJECTS_DIR = $$BUILDDIR/.obj
MOC_DIR = $$BUILDDIR/.moc
RCC_DIR = $$BUILDDIR/.qrc
UI_DIR = $$BUILDDIR/.ui

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

if(debug){
LIBS += -L$$(DEV_LIBS) -ldeviced -ltagsystemd


}
else{
LIBS += -L$$(DEV_LIBS) -ldevice -ltagsystem
}

INCLUDEPATH += $$(JUNE_ROOT)



SOURCES += main.cpp \
    app.cpp \
    gui/systemtrayui.cpp \
    gui/console.cpp \
    influxdb/bmv712smart.cpp \
    influxdbfactory.cpp \
    menu.cpp

HEADERS += \
    app.h \
    factory.h \
    factorybase.h \
    gui/systemtrayui.h \
    gui/console.h \
    influxdb/bmv712smart.h \
    influxdbfactory.h \
    menu.h

RESOURCES += \
    resources.qrc

