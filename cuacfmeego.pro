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
              cfm-controller.cc \
              qml-file.cc
HEADERS    += cfm-controller.h \
              qml-file.h
RESOURCES  += cuacfmeego.qrc

# l10n stuff
#
CODECFORTR    = UTF-8
TRANSLATIONS += l10n/en.ts \
                l10n/es.ts

# Rule for regenerating .qm files for translations (this is missing
# from the default qmake rule set).
#
updateqm.input = TRANSLATIONS
updateqm.output = ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.qm
updateqm.commands = lrelease ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_OUT}
updateqm.CONFIG += no_link
QMAKE_EXTRA_COMPILERS += updateqm
PRE_TARGETDEPS = compiler_updateqm_make_all

