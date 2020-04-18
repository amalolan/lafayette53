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
    ../model/LafException.h \
    util.h \
    helper.h


osx: LIBS += -L$$PWD/../../../../../../../usr/local/Cellar/cpprestsdk/2.10.15/lib/ -lcpprest.2.10

osx: INCLUDEPATH += $$PWD/../../../../../../../usr/local/Cellar/cpprestsdk/2.10.15/include
osx: DEPENDPATH += $$PWD/../../../../../../../usr/local/Cellar/cpprestsdk/2.10.15/include

osx: LIBS += -L$$PWD/../../../../../../../usr/local/Cellar/boost@1.60/1.60.0/lib/ -lboost_system

osx: INCLUDEPATH += $$PWD/../../../../../../../usr/local/Cellar/boost@1.60/1.60.0/include
osx: DEPENDPATH += $$PWD/../../../../../../../usr/local/Cellar/boost@1.60/1.60.0/include

osx: LIBS += -L$$PWD/../../../../../../../usr/local/Cellar/boost@1.60/1.60.0/lib/ -lboost_thread-mt

osx: INCLUDEPATH += $$PWD/../../../../../../../usr/local/Cellar/boost@1.60/1.60.0/include
osx: DEPENDPATH += $$PWD/../../../../../../../usr/local/Cellar/boost@1.60/1.60.0/include

osx: LIBS += -L$$PWD/../../../../../../../usr/local/Cellar/boost@1.60/1.60.0/lib/ -lboost_chrono-mt

osx: INCLUDEPATH += $$PWD/../../../../../../../usr/local/Cellar/boost@1.60/1.60.0/include
osx: DEPENDPATH += $$PWD/../../../../../../../usr/local/Cellar/boost@1.60/1.60.0/include

osx: LIBS += -L$$PWD/../../../../../../../usr/local/Cellar/openssl/1.0.2t/lib/ -lssl.1.0.0

osx: INCLUDEPATH += $$PWD/../../../../../../../usr/local/Cellar/openssl/1.0.2t/include
osx: DEPENDPATH += $$PWD/../../../../../../../usr/local/Cellar/openssl/1.0.2t/include

osx: LIBS += -L$$PWD/../../../../../../../usr/local/Cellar/openssl/1.0.2t/lib/ -lcrypto.1.0.0

osx: INCLUDEPATH += $$PWD/../../../../../../../usr/local/Cellar/openssl/1.0.2t/include
osx: DEPENDPATH += $$PWD/../../../../../../../usr/local/Cellar/openssl/1.0.2t/include
