TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

LIBS += -pthread
LIBS += -lboost_system -lboost_thread
LIBS += -lOVR -ldl

SOURCES += \
    main.cpp \
    TcpClient.cpp \
    vendor/pp-ne-oculus-server/TcpMessage.cpp \
    vendor/pp-ne-oculus-server/EdvsEventsCollection.cpp \
    vendor/pp-ne-oculus-server/Message.cpp \
    vendor/pp-ne-oculus-server/Message_EventCollection.cpp \
    EdvsImage.cpp \
    EdvsSphericImage.cpp \
    EdvsEventHandler.cpp

HEADERS += \
    TcpClient.hpp \
    vendor/pp-ne-oculus-server/TcpMessage.hpp \
    vendor/pp-ne-oculus-server/EdvsEventsCollection.h \
    vendor/pp-ne-oculus-server/EdvsEventsCollection.hpp \
    vendor/pp-ne-oculus-server/Message.hpp \
    vendor/pp-ne-oculus-server/Message_EventCollection.hpp \
    EdvsImage.hpp \
    EdvsSphericImage.hpp \
    EdvsEventHandler.hpp

