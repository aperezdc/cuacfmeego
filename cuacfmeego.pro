# vim: filetype=make expandtab

TEMPLATE   = app
TARGET     = cuacfmeego
QT        += declarative \
             network \
             xml
CONFIG    += link_pkgconfig \
             meegotouch \
             qt-boostable \
             qdeclarative-boostable
PKGCONFIG += gstreamer-net-0.10 \
             gstreamer-base-0.10 \
             gstreamer-audio-0.10 \
             conic

# Do not clutter the source directory
#
OBJECTS_DIR = ,build
MOC_DIR     = ,build

# Needed for the booster to work
#
QMAKE_CXXFLAGS += -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
QMAKE_LFLAGS   += -pie -rdynamic -Wl,--as-needed

# Application sources
#
SOURCES    += cfm-main.cc \
              cfm-controller.cc
HEADERS    += cfm-controller.h
RESOURCES  += cuacfmeego.qrc

