QT += sql
TEMPLATE = app
CONFIG += console c++11

SOURCES += \
        artifact.cpp \
        collection.cpp \
        main.cpp \
        modelclass.cpp \
        museum.cpp

HEADERS += \
    LafException.h \
    artifact.h \
    collection.h \
    modelclass.h \
    museum.h

LIBS += -pthread

SOURCES += ../gtest/googletest/src/gtest-all.cc

INCLUDEPATH +=  ../gtest                                   \
                ../gtest/googletest                        \
                ../gtest/googletest/include                \
                ../gtest/googletest/include/gtest
