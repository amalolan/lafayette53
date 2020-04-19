QT += sql
TEMPLATE = app
CONFIG += console c++11

SOURCES += \
        artifact.cpp \
        collection.cpp \
        modelclass.cpp \
        main.cpp

HEADERS += \
<<<<<<< HEAD
    LafException.h \
=======
    ModelException.h \
>>>>>>> 612d083e490d99c04e164c457c12f666e91e1230
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
