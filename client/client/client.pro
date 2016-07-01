TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS+=-pthread -Werror
QMAKE_LFLAGS+=-pthread -lGL -lglut -lGLU -lopenal -lX11
SOURCES += \
    ../base/console.cpp \
    ../base/daemon.cpp \
    ../base/file_index.cpp \
    ../base/GL_model.cpp \
    ../base/gra_tools.cpp \
    ../base/interfaces.cpp \
    ../base/main.cpp \
    ../base/paint_board.cpp \
    ../base/states.cpp \
    ../base/voice.cpp \
    ../core/character_controller.cpp \
    ../core/Com.cpp \
    ../core/graphic.cpp \
    ../core/input_method.cpp \
    ../net/gra_world_link.cpp \
    ../../pub/inte_link.cpp \
    ../../pub/spinlock.cpp \
    ../../pub/thread_control.cpp \
    ../../pub/time_control.cpp

HEADERS += \
    ../base/console.h \
    ../base/daemon.h \
    ../base/file_index.h \
    ../base/GL_model.h \
    ../base/gra_structs.h \
    ../base/gra_tools.h \
    ../base/interfaces.h \
    ../base/paint_board.h \
    ../base/states.h \
    ../base/voice.h \
    ../core/character_controller.h \
    ../core/Com.h \
    ../core/graphic.h \
    ../core/input_method.h \
    ../net/gra_world_link.h \
    ../../pub/inte_link.h \
    ../../pub/spinlock.h \
    ../../pub/STDHF.h \
    ../../pub/thread_control.h \
    ../../pub/time_control.h \
    ../../pub/world_pub_structs.h

