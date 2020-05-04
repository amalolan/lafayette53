TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
#CONFIG -= qt
QT += sql
QMAKE_CXXFLAGS += -std=gnu++11
QMAKE_CXXFLAGS_WARN_ON = -w

QMAKE_LFLAGS += -g -fprofile-arcs -ftest-coverage  -O0
QMAKE_CXXFLAGS += -g -fprofile-arcs -ftest-coverage -O0

#lcov --capture --directory build-lafayette53-Desktop_Qt_5_14_1_clang_64bit-Debug/tests/backend --output-file lcov/coverage.info
#genhtml lcov/coverage.info --output-directory lcov


SOURCES += \
        main.cpp \
        handlertest.cpp \
        utiltest.cpp

HEADERS += \
        handlertest.h \
        mockmodelclass.h \
        utiltest.h

SOURCES += \
    ../../model/modelclass.cpp \
    ../../model/artifact.cpp \
    ../../model/collection.cpp \
    ../../backend/handler.cpp


HEADERS += \
    ../../backend/handler.h \
    ../../backend/util.h \
    ../../backend/controller.h \
    ../../model/modelclass.h \
    ../../model/artifact.h \
    ../../model/collection.h \
    ../../model/museum.h \
    ../../model/user.h \
    ../../model/ModelException.h



SOURCES += ../../lib/gtest/googletest/src/gtest-all.cc \
           ../../lib/gtest/googlemock/src/gmock-all.cc \
           ../../lib/nlohmann/json.hpp


INCLUDEPATH +=  ../../lib/gtest                                   \
                ../../lib/gtest/googletest                        \
                ../../lib/gtest/googletest/include                \
                ../../lib/gtest/googletest/include/gtest          \
                ../../lib/gtest/googlemock                        \
                ../../lib/gtest/googlemock/include                \
                ../../lib/gtest/googlemock/include/gmock

#LIBS += -lgtest -L$$PWD/../../
#LIBS += -lgtest -L$$PWD/../../
#LIBS += -lgcov

LIBS += -lcpprest -lssl -lcrypto -lboost_system -pthread



macx|win32: LIBS += -L$$PWD/../../lib/cpprestsdk@2.10.15/lib/ -lcpprest.2.10

INCLUDEPATH += $$PWD/../../lib/cpprestsdk@2.10.15/include
DEPENDPATH += $$PWD/../../lib/cpprestsdk@2.10.15/include

macx|win32: LIBS += -L$$PWD/../../lib/openssl@1.0.2t/lib/ -lssl.1.0.0

INCLUDEPATH += $$PWD/../../lib/openssl@1.0.2t/include
DEPENDPATH += $$PWD/../../lib/openssl@1.0.2t/include

macx|win32: LIBS += -L$$PWD/../../lib/openssl@1.0.2t/lib/ -lcrypto.1.0.0

macx|win32: LIBS += -L$$PWD/../../lib/boost@1.60/lib/ -lboost_system-mt

INCLUDEPATH += $$PWD/../../lib/boost@1.60/include
DEPENDPATH += $$PWD/../../lib/boost@1.60/include

macx|win32: LIBS += -L$$PWD/../../lib/boost@1.60/lib/ -lboost_chrono-mt

macx|win32: LIBS += -L$$PWD/../../lib/boost@1.60/lib/ -lboost_thread-mt

