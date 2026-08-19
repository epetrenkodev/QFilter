APP_VERSION     = 1.1.0
APP_NAME        = "QFilter"
APP_COMPANY     = "EPetrenko"
APP_DESCRIPTION = "Приложение для контроля сроков замены фильтров вентиляции пассажирских вагонов"
APP_EXEC        = "qfilter"

VERSION = $$APP_VERSION

DEFINES += APP_VERSION="\\\"$$APP_VERSION\\\""
DEFINES += APP_NAME="\\\"$$APP_NAME\\\""
DEFINES += APP_COMPANY="\\\"$$APP_COMPANY\\\""

QT += core gui sql charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    dialogs/mainwindow.cpp \
    dialogs/editdialog.cpp \
    dialogs/settingdialog.cpp \
    dialogs/aboutdialog.cpp \
    dialogs/manualdialog.cpp \
    models/wagonmodel.cpp \
    data/abstractrepository.cpp \
    data/sqliterepository.cpp \
    data/wagon.cpp

HEADERS += \
    dialogs/mainwindow.h \
    dialogs/editdialog.h \
    dialogs/settingdialog.h \
    dialogs/aboutdialog.h \
    dialogs/manualdialog.h \
    models/wagonmodel.h \
    data/abstractrepository.h \
    data/sqliterepository.h \
    data/wagon.h

FORMS += \
    dialogs/aboutdialog.ui \
    dialogs/editdialog.ui \
    dialogs/mainwindow.ui \
    dialogs/manualdialog.ui \
    dialogs/settingdialog.ui

RESOURCES += \
    qfilter.qrc

OTHER_FILES += \
    ../.gitignore \
    ../README.md \
    qfilter.rc

# Windows
win32 {
    RC_ICONS = resources/qfilter.ico
    RC_FILE = qfilter.rc

    COMMA = ,
    VERSION_LIST = $$split(APP_VERSION, .)
    WIN_VERSION = $$join(VERSION_LIST, $$COMMA)

    RC_DEFINES += \
        RC_VERSION=\\\"$$WIN_VERSION\\\" \
        RC_VERSION_STR=\\\"$$APP_VERSION\\\" \
        RC_NAME=\\\"$$APP_NAME\\\"
}

# Linux
DESKTOP_FILE_PATH = $$_PRO_FILE_PWD_/qfilter.desktop
DESKTOP_CONTENT = \
    "[Desktop Entry]" \
    "Type=Application" \
    "Name=$$APP_NAME" \
    "Comment=$$APP_DESCRIPTION" \
    "Exec=$$APP_EXEC" \
    "Icon=$$APP_EXEC" \
    "Terminal=false" \
    "Categories=Utility;"
write_file($$DESKTOP_FILE_PATH, DESKTOP_CONTENT)

nix:!macx {
    target.path = /usr/bin
    desktop.path = /usr/share/applications
    desktop.files = $$DESKTOP_FILE_PATH
    icon.path = /usr/share/pixmaps
    icon.files = resources/qfilter.png

    INSTALLS += target desktop icon
}
