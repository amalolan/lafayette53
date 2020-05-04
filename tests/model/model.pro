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

SOURCES += ../../lib/gtest/googletest/src/gtest-all.cc \
           ../../lib/gtest/googlemock/src/gmock-all.cc


INCLUDEPATH +=  ../../lib/gtest                                   \
                ../../lib/gtest/googletest                        \
                ../../lib/gtest/googletest/include                \
                ../../lib/gtest/googletest/include/gtest          \
                ../../lib/gtest/googlemock                        \
                ../../lib/gtest/googlemock/include                \
                ../../lib/gtest/googlemock/include/gmock

