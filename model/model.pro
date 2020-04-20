QT += sql
TEMPLATE = app
CONFIG += console c++11

SOURCES += \
        artifact.cpp \
        collection.cpp \
        modelclass.cpp \
        main.cpp

HEADERS += \
    LafException.h \
    artifact.h \
    collection.h \
    museum.h \
    modelclass.h \
    user.h

LIBS += -pthread

SOURCES += ../gtest/googletest/src/gtest-all.cc

INCLUDEPATH +=  ../gtest                                   \
                ../gtest/googletest                        \
                ../gtest/googletest/include                \
                ../gtest/googletest/include/gtest
