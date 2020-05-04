TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
#CONFIG -= qt
QT += sql
QMAKE_CXXFLAGS += -std=gnu++11

QMAKE_LFLAGS += -g -fprofile-arcs -ftest-coverage  -O0
QMAKE_CXXFLAGS += -g -fprofile-arcs -ftest-coverage -O0


SOURCES += \
        devtests.cpp \
        main.cpp

HEADERS += \
    devtests.h


SOURCES += \
    ../../model/modelclass.cpp \
    ../../model/artifact.cpp \
    ../../model/collection.cpp


HEADERS += \
    ../../model/modelclass.h \
    ../../model/artifact.h \
    ../../model/collection.h \
    ../../model/museum.h \
    ../../model/user.h \
    ../../model/ModelException.h

LIBS += -pthread

SOURCES += ../../gtest/googletest/src/gtest-all.cc \
           ../../gtest/googlemock/src/gmock-all.cc


INCLUDEPATH +=  ../../gtest                                   \
                ../../gtest/googletest                        \
                ../../gtest/googletest/include                \
                ../../gtest/googletest/include/gtest          \
                ../../gtest/googlemock                        \
                ../../gtest/googlemock/include                \
                ../../gtest/googlemock/include/gmock

