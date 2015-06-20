TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
QMAKE_CFLAGS += -std=gnu11

LIBS += -pthread
LIBS += -lboost_system -lboost_thread
LIBS += -lOVR -ldl -lglfw -lGL -lGLEW -lGLU
LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc
LIBS += -lOpenCTM -L/usr/local/lib/


install_it.path = $$OUT_PWD/resources/
install_it.files  = resources/sphere.ctm
install_it.files += resources/Textured.fs
install_it.files += resources/Textured.vs

INSTALLS += \
    install_it

SOURCES += \
    main.cpp \
    TcpClient.cpp \
    EdvsImage.cpp \
    EdvsSphericImage.cpp \
    EdvsEventHandler.cpp \
    vendor/edvstools/Edvs/EventIO.cpp \
    vendor/edvstools/Edvs/EventStream.cpp \
    vendor/edvstools/Edvs/edvs.c \
    glfw/GlfwApp.cpp \
    glfw/GlfwUtils.cpp \
    opengl/Font.cpp \
    opengl/GlUtils.cpp \
    opengl/Shaders.cpp \
    opengl/Textures.cpp \
    ovr/OvrUtils.cpp \
    ovr/RiftApp.cpp \
    ovr/RiftGlfwApp.cpp \
    ovr/RiftManagerApp.cpp \
    ovr/RiftRenderingApp.cpp \
    resources/ResourceEnums.cpp \
    resources/Resources.cpp \
    PhotoSphereExample.cpp \
    Platform.cpp \
    Statics.cpp \
    Utils.cpp \
    EdvsRiftApp.cpp \
    vendor/oculus-server/EdvsEventsCollection.cpp \
    vendor/oculus-server/Message.cpp \
    vendor/oculus-server/Message_EventCollection.cpp \
    vendor/oculus-server/TcpMessage.cpp

HEADERS += \
    TcpClient.hpp \
    EdvsImage.hpp \
    EdvsSphericImage.hpp \
    EdvsEventHandler.hpp \
    vendor/edvstools/Edvs/edvs.h \
    vendor/edvstools/Edvs/edvs_impl.h \
    vendor/edvstools/Edvs/event.h \
    vendor/edvstools/Edvs/Event.hpp \
    vendor/edvstools/Edvs/EventIO.hpp \
    vendor/edvstools/Edvs/EventStream.hpp \
    glfw/GlfwApp.h \
    glfw/GlfwUtils.h \
    opengl/Constants.h \
    opengl/Font.h \
    opengl/Framebuffer.h \
    opengl/GlUtils.h \
    opengl/Shaders.h \
    opengl/Textures.h \
    ovr/OvrUtils.h \
    ovr/RiftApp.h \
    ovr/RiftGlfwApp.h \
    ovr/RiftManagerApp.h \
    ovr/RiftRenderingApp.h \
    rendering/Colors.h \
    rendering/Lights.h \
    rendering/MatrixStack.h \
    rendering/State.h \
    rendering/Vectors.h \
    resources/ResourceEnums.h \
    resources/Resources.h \
    resources/Resources_Export.h \
    Common.h \
    IO.h \
    PhotoSphereExample.h \
    Platform.h \
    Types.h \
    Utils.h \
    EdvsRiftApp.h \
    vendor/oculus-server/EdvsEventsCollection.hpp \
    vendor/oculus-server/Message.hpp \
    vendor/oculus-server/Message_EventCollection.hpp \
    vendor/oculus-server/TcpMessage.hpp

DISTFILES += \
    resources/sphere.ctm \
    resources/Textured.fs \
    resources/Textured.vs
