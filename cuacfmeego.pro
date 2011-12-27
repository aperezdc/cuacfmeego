# vim: filetype=make expandtab

TEMPLATE = app
TARGET   = cuacfmeego
QT      += declarative \
           network \
           xml
CONFIG  += meegotouch \
           qt-boostable \
           qdeclarative-boostable

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

