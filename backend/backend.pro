TEMPLATE = app
CONFIG += console c++11
QT += sql

SOURCES += \
        handler.cpp \
        main.cpp \
        ../model/modelclass.cpp \
        ../model/collection.cpp


QMAKE_CXXFLAGS += -std=gnu++11
QMAKE_CXXFLAGS_WARN_ON = -w
LIBS += -lcpprest -lssl -lcrypto -lboost_system -pthread

HEADERS += \
    controller.h \
    handler.h \
    ../model/modelclass.h \
    ../model/collection.h \
    ../model/user.h \
    ../model/museum.h \
    ../model/ModelException.h \
    util.h \
    ../nlohmann/json.hpp


macx: LIBS += -L$$PWD/../../../../../../../../usr/local/Cellar/boost@1.60/1.60.0/lib/ -lboost_system

INCLUDEPATH += $$PWD/../../../../../../../../usr/local/Cellar/boost@1.60/1.60.0/include
DEPENDPATH += $$PWD/../../../../../../../../usr/local/Cellar/boost@1.60/1.60.0/include


macx: LIBS += -L$$PWD/../../../../../../../../usr/local/Cellar/cpprestsdk/2.10.15/lib/ -lcpprest.2.10

INCLUDEPATH += $$PWD/../../../../../../../../usr/local/Cellar/cpprestsdk/2.10.15/include
DEPENDPATH += $$PWD/../../../../../../../../usr/local/Cellar/cpprestsdk/2.10.15/include

macx: LIBS += -L$$PWD/../../../../../../../../usr/local/Cellar/openssl/1.0.2t/lib/ -lssl.1.0.0

INCLUDEPATH += $$PWD/../../../../../../../../usr/local/Cellar/openssl/1.0.2t/include
DEPENDPATH += $$PWD/../../../../../../../../usr/local/Cellar/openssl/1.0.2t/include

macx: LIBS += -L$$PWD/../../../../../../../../usr/local/Cellar/openssl/1.0.2t/lib/ -lcrypto.1.0.0

INCLUDEPATH += $$PWD/../../../../../../../../usr/local/Cellar/openssl/1.0.2t/include
DEPENDPATH += $$PWD/../../../../../../../../usr/local/Cellar/openssl/1.0.2t/include

macx: LIBS += -L$$PWD/../../../../../../../../usr/local/Cellar/boost@1.60/1.60.0/lib/ -lboost_thread-mt

INCLUDEPATH += $$PWD/../../../../../../../../usr/local/Cellar/boost@1.60/1.60.0/include
DEPENDPATH += $$PWD/../../../../../../../../usr/local/Cellar/boost@1.60/1.60.0/include

macx: LIBS += -L$$PWD/../../../../../../../../usr/local/Cellar/boost@1.60/1.60.0/lib/ -lboost_chrono-mt

INCLUDEPATH += $$PWD/../../../../../../../../usr/local/Cellar/boost@1.60/1.60.0/include
DEPENDPATH += $$PWD/../../../../../../../../usr/local/Cellar/boost@1.60/1.60.0/include

