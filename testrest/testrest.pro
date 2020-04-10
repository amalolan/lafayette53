TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        handler.cpp \
        main.cpp \
        server.cpp

QMAKE_CXXFLAGS += -std=gnu++11

LIBS += -lcpprest -lssl -lcrypto -lboost_system -pthread

HEADERS += \
    handler.h \
    server.h


