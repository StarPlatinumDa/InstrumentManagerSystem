QT       += core gui charts

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

SOURCES += \
    addinstrument.cpp \
    departmentmanager.cpp \
    detail.cpp \
    global.cpp \
    gzsystem.cpp \
    hander.cpp \
    individual.cpp \
    instrumentinfomation.cpp \
    instrumentmanager.cpp \
    logging.cpp \
    main.cpp \
    modify.cpp \
    userinformation.cpp \
    usermanager.cpp \
    depart.cpp \
    dialog.cpp \
    sub1.cpp

HEADERS += \
    addinstrument.h \
    departmentmanager.h \
    detail.h \
    global.h \
    gzsystem.h \
    hander.h \
    individual.h \
    instrumentinfomation.h \
    instrumentmanager.h \
    logging.h \
    modify.h \
    userinformation.h \
    usermanager.h \
    progresswater.h \
    progresspercent.h \
    navprogress.h \
    navbar.h \
    lunarcalendaritem.h \
    depart.h \
    dialog.h \
    sub1.h

FORMS += \
    addinstrument.ui \
    departmentmanager.ui \
    detail.ui \
    gzsystem.ui \
    individual.ui \
    instrumentinfomation.ui \
    instrumentmanager.ui \
    logging.ui \
    modify.ui \
    userinformation.ui \
    usermanager.ui \
    depart.ui \
    dialog.ui \
    sub1.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
INCLUDEPATH += $$PWD

CONFIG(debug, debug|release){
LIBS += -L$$PWD/ -lqucd
} else {
LIBS += -L$$PWD/ -lquc
}

RESOURCES += \
    source.qrc
RC_FILE = icon.rc
