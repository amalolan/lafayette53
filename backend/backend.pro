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
    ../lib/nlohmann/json.hpp



macx|win32: LIBS += -L$$PWD/../lib/cpprestsdk@2.10.15/lib/ -lcpprest.2.10

INCLUDEPATH += $$PWD/../lib/cpprestsdk@2.10.15/include
DEPENDPATH += $$PWD/../lib/cpprestsdk@2.10.15/include

macx|win32: LIBS += -L$$PWD/../lib/openssl@1.0.2t/lib/ -lssl.1.0.0

INCLUDEPATH += $$PWD/../lib/openssl@1.0.2t/include
DEPENDPATH += $$PWD/../lib/openssl@1.0.2t/include

macx|win32: LIBS += -L$$PWD/../lib/openssl@1.0.2t/lib/ -lcrypto.1.0.0

macx|win32: LIBS += -L$$PWD/../lib/boost@1.60/lib/ -lboost_system-mt

INCLUDEPATH += $$PWD/../lib/boost@1.60/include
DEPENDPATH += $$PWD/../lib/boost@1.60/include

macx|win32: LIBS += -L$$PWD/../lib/boost@1.60/lib/ -lboost_chrono-mt

macx|win32: LIBS += -L$$PWD/../lib/boost@1.60/lib/ -lboost_thread-mt
