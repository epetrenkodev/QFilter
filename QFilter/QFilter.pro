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

win32: RC_FILE = qfilter.rc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

