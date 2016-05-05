TEMPLATE = app

QT += gui widgets sql
CONFIG += c++11 silent

VERSION = 0.0.1
DEFINES += STURDY_VERSION=\\\"$${VERSION}\\\"

SOURCES += \
    main.cpp

include(core/core.pri)
include(mvc/mvc.pri)
include(ui/ui.pri)

RESOURCES += \
    resources/sturdy.qrc

DESTDIR = $$BUILD_ROOT/bin

unix:!macx {
    TARGET = sturdy
    isEmpty(PREFIX): PREFIX = /usr
    target.path = $$PREFIX/bin
    INSTALLS = target

    # Install icons and stuff 
}

win32 {
    TARGET = sturdy

    # Install icons and stuff
}

macx {
    TARGET = sturdy

    # Install icons and stuff
}

# Keeping build directory organisez
MOC_DIR = $$BUILD_ROOT/.moc
OBJECTS_DIR = $$BUILD_ROOT/.obj
RCC_DIR = $$BUILD_ROOT/.rcc
UI_DIR = $$BUILD_ROOT/.ui
