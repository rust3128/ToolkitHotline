QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
win32: RC_ICONS = $$PWD/Images/logo.ico

SOURCES += \
    Clients/addclientdialog.cpp \
    Clients/clientinfodialog.cpp \
    Clients/clientslistdialog.cpp \
    ConnectionSettingDialog/connectionsettingdialog.cpp \
    DataBase/database.cpp \
    DynamicButton/dynamiicbutton.cpp \
    GlobalSettings/globalsettings.cpp \
    LoggingCategories/loggingcategories.cpp \
    OptionsDialog/optionsdialog.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Clients/addclientdialog.h \
    Clients/clientinfodialog.h \
    Clients/clientslistdialog.h \
    ConnectionSettingDialog/connectionsettingdialog.h \
    DataBase/database.h \
    DynamicButton/dynamiicbutton.h \
    GlobalSettings/globalsettings.h \
    LoggingCategories/loggingcategories.h \
    OptionsDialog/optionsdialog.h \
    mainwindow.h

FORMS += \
    Clients/addclientdialog.ui \
    Clients/clientinfodialog.ui \
    Clients/clientslistdialog.ui \
    ConnectionSettingDialog/connectionsettingdialog.ui \
    OptionsDialog/optionsdialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resourse.qrc
