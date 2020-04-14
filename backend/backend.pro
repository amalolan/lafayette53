TEMPLATE = app
CONFIG += console c++11
QT += sql

SOURCES += \
        handler.cpp \
        main.cpp \
        ../model/modelclass.cpp

QMAKE_CXXFLAGS += -std=gnu++11

LIBS += -lcpprest -lssl -lcrypto -lboost_system -pthread

HEADERS += \
    handler.h \
    ../model/modelclass.h


