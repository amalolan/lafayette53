TEMPLATE = app
CONFIG += console c++11
QT += sql

SOURCES += \
        handler.cpp \
        main.cpp \
        ../model/modelclass.cpp \
        util.cpp

QMAKE_CXXFLAGS += -std=gnu++11

LIBS += -lcpprest -lssl -lcrypto -lboost_system -pthread

HEADERS += \
    handler.h \
    ../model/modelclass.h \
    ../model/ModelException.h \
    util.h


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
