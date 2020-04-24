QT += sql
TEMPLATE = app
CONFIG += console c++11

SOURCES += \
        artifact.cpp \
        collection.cpp \
        modelclass.cpp \
        main.cpp

HEADERS += \
    ModelException.h \
    artifact.h \
    collection.h \
    museum.h \
    modelclass.h \
    user.h \
    ../nlohmann/json.hpp

LIBS += -pthread

