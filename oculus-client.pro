TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
QMAKE_CFLAGS += -std=gnu11

LIBS += -pthread
LIBS += -lboost_system -lboost_thread
LIBS += -lOVR -ldl -lglfw -lGLU -lGL

SOURCES += \
    main.cpp \
    TcpClient.cpp \
    vendor/pp-ne-oculus-server/TcpMessage.cpp \
    vendor/pp-ne-oculus-server/EdvsEventsCollection.cpp \
    vendor/pp-ne-oculus-server/Message.cpp \
    vendor/pp-ne-oculus-server/Message_EventCollection.cpp \
    EdvsImage.cpp \
    EdvsSphericImage.cpp \
    EdvsEventHandler.cpp \
    vendor/edvstools/Edvs/EventIO.cpp \
    vendor/edvstools/Edvs/EventStream.cpp \
    vendor/edvstools/Edvs/edvs.c

HEADERS += \
    TcpClient.hpp \
    vendor/pp-ne-oculus-server/TcpMessage.hpp \
    vendor/pp-ne-oculus-server/EdvsEventsCollection.h \
    vendor/pp-ne-oculus-server/EdvsEventsCollection.hpp \
    vendor/pp-ne-oculus-server/Message.hpp \
    vendor/pp-ne-oculus-server/Message_EventCollection.hpp \
    EdvsImage.hpp \
    EdvsSphericImage.hpp \
    EdvsEventHandler.hpp \
    vendor/edvstools/Edvs/edvs.h \
    vendor/edvstools/Edvs/edvs_impl.h \
    vendor/edvstools/Edvs/event.h \
    vendor/edvstools/Edvs/Event.hpp \
    vendor/edvstools/Edvs/EventIO.hpp \
    vendor/edvstools/Edvs/EventStream.hpp
