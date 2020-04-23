QT += sql
TEMPLATE = app
CONFIG += console c++11

SOURCES += \
        artifact.cpp \
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

LIBS += -pthread \
         -lsqlite3

SOURCES += ../gtest/googletest/src/gtest-all.cc

INCLUDEPATH +=  ../gtest                                   \
                ../gtest/googletest                        \
                ../gtest/googletest/include                \
                ../gtest/googletest/include/gtest
