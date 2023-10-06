TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    solution.cpp \
    router.cpp \
    global.cpp \
    parameters.cpp \
    packet.cpp \
    routetableclause.cpp

HEADERS += \
    solution.h \
    router.h \
    global.h \
    parameters.h \
    packet.h \
    routetableclause.h
