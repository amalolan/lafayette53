QT += sql
TEMPLATE = app
CONFIG += console c++11

SOURCES += \
        main.cpp\
        museum.cpp\
        museum.cpp \
        server.cpp\
        url.cpp\
        ../model/modelclass.cpp\

HEADERS += \
        museum.h \
        server.h\
        router.h\
        url.h
        ../model/modelclass.h\

OTHER_FILES += \
        index.html\
        museum-list.json

