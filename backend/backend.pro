TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp\
    museum.cpp \
        server.cpp\
        url.cpp\

HEADERS += \
        museum.h \
        server.h\
        router.h\
        url.h

OTHER_FILES += \
        index.html\
        museum-list.json

