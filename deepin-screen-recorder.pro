######################################################################
# Automatically generated by qmake (3.0) ?? 2? 4 17:49:37 2017
######################################################################

TEMPLATE = app
TARGET = deepin-screen-recorder
INCLUDEPATH += .

CONFIG += link_pkgconfig
CONFIG += c++11
PKGCONFIG += xcb xcb-util dtkwidget dtkwm
RESOURCES = deepin-screen-recorder.qrc

# Input
HEADERS += src/main_window.h src/record_process.h src/settings.h src/utils.h src/record_button.h src/record_option_panel.h src/countdown_tooltip.h src/constant.h src/event_monitor.h src/start_tooltip.h src/button_feedback.h src/process_tree.h
SOURCES += src/main.cpp src/main_window.cpp src/record_process.cpp src/settings.cpp src/utils.cpp src/record_button.cpp src/record_option_panel.cpp src/countdown_tooltip.cpp src/constant.cpp src/event_monitor.cpp src/start_tooltip.cpp src/button_feedback.cpp src/process_tree.cpp

QT += core
QT += widgets
QT += gui
QT += network
QT += x11extras
QT += dbus
LIBS += -lX11 -lXext -lXtst -ldtkwm
LIBS += -L"libprocps" -lprocps

QMAKE_CXXFLAGS += -g

isEmpty(PREFIX){
    PREFIX = /usr
}

isEmpty(BINDIR):BINDIR=/usr/bin
isEmpty(ICONDIR):ICONDIR=/usr/share/icons/hicolor/scalable/apps
isEmpty(APPDIR):APPDIR=/usr/share/applications
isEmpty(DSRDIR):DSRDIR=/usr/share/deepin-screen-recorder
isEmpty(DOCDIR):DOCDIR=/usr/share/dman/deepin-screen-recorder

target.path = $$INSTROOT$$BINDIR
icon.path = $$INSTROOT$$ICONDIR
desktop.path = $$INSTROOT$$APPDIR
translations.path = $$INSTROOT$$DSRDIR/translations
manual.path = $$INSTROOT$$DOCDIR

icon.files = image/deepin-screen-recorder.svg
desktop.files = deepin-screen-recorder.desktop
manual.files = manual/*

dbus_service.files = $$PWD/com.deepin.ScreenRecorder.service
dbus_service.path = $$PREFIX/share/dbus-1/services

INSTALLS += target icon desktop manual dbus_service

isEmpty(TRANSLATIONS) {
     include(translations.pri)
}

TRANSLATIONS_COMPILED = $$TRANSLATIONS
TRANSLATIONS_COMPILED ~= s/\.ts/.qm/g

translations.files = $$TRANSLATIONS_COMPILED
INSTALLS += translations
CONFIG *= update_translations release_translations

CONFIG(update_translations) {
    isEmpty(lupdate):lupdate=lupdate
    system($$lupdate -no-obsolete -locations none $$_PRO_FILE_)
}
CONFIG(release_translations) {
    isEmpty(lrelease):lrelease=lrelease
    system($$lrelease $$_PRO_FILE_)
}

DSR_LANG_PATH += $$DSRDIR/translations
DEFINES += "DSR_LANG_PATH=\\\"$$DSR_LANG_PATH\\\""
