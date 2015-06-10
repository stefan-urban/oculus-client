TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

LIBS += -pthread
LIBS += -lboost_system -lboost_thread

SOURCES += \
    main.cpp \
    TcpClient.cpp

HEADERS += \
    TcpClient.hpp

