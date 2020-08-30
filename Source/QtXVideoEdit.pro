QT += quick quickwidgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/QtLib/ImgButton.cpp \
    src/QtLib/winfrmtitle.cpp \
        src/globalutinityobject.cpp \
    src/mainwin.cpp \
        src/qmltypesregister.cpp \
        src\framelrender.cpp \
        src\frameplayview.cpp \
        main.cpp \
    src/EUMltCtl.cpp \
    src/EUPlaylist.cpp \
    src/EUTractor.cpp

RESOURCES += qml.qrc \
    shaders.qrc

TRANSLATIONS += \
    QtXVideoEdit_zh_CN.ts

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = qmls/widgets \
                qmls\component

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    src/QtLib/ImgButton.h \
    src/QtLib/winfrmtitle.h \
    src/globalutinityobject.h \
    src/mainwin.h \
    src/qmltypesregister.h \
    src\framelrender.h \
    src\frameplayview.h \
    src/EUMltCtl.h \
    src/EUPlaylist.h \
    src/EUTractor.h \
    src/EUType.h

QT += widgets

INCLUDEPATH += $$PWD/third/include/mlt++ \
     $$PWD/third/include

LIBS += -L$$PWD/third/lib -lmlt++ -lmlt

DISTFILES += \
    QtXVideoEdit_zh_CN.ts

