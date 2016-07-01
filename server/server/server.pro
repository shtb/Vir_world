TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS+=-pthread -Werror
QMAKE_LFLAGS+=-pthread
SOURCES += \
    ../base/coll_model.cpp \
    ../base/daemon.cpp \
    ../base/file_index.cpp \
    ../base/interfaces.cpp \
    ../base/lives.cpp \
    ../base/main.cpp \
    ../base/virw_time.cpp \
    ../core/collision_check.cpp \
    ../core/cubes.cpp \
    ../core/fly_eats.cpp \
    ../core/grass_eats.cpp \
    ../core/meat_eats.cpp \
    ../core/trees.cpp \
    ../core/virw_grains.cpp \
    ../core/virw_weather.cpp \
    ../core/world.cpp \
    ../core/world_switch.cpp \
    ../net/gra_link.cpp \
    ../../pub/console.cpp \
    ../../pub/inte_link.cpp \
    ../../pub/spinlock.cpp \
    ../../pub/thread_control.cpp \
    ../../pub/time_control.cpp

HEADERS += \
    ../base/coll_model.h \
    ../base/daemon.h \
    ../base/file_index.h \
    ../base/GL_task.h \
    ../base/interfaces.h \
    ../base/lives.h \
    ../base/virw_time.h \
    ../base/world_structs.h \
    ../core/collision_check.h \
    ../core/cubes.h \
    ../core/fly_eats.h \
    ../core/grass_eats.h \
    ../core/meat_eats.h \
    ../core/trees.h \
    ../core/virw_grains.h \
    ../core/virw_weather.h \
    ../core/world.h \
    ../core/world_switch.h \
    ../net/gra_link.h \
    ../../pub/console.h \
    ../../pub/inte_link.h \
    ../../pub/spinlock.h \
    ../../pub/STDHF.h \
    ../../pub/thread_control.h \
    ../../pub/time_control.h \
    ../../pub/world_pub_structs.h

