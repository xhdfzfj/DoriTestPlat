QT += quick
QT += serialport
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Class/FileAnalyseBaseClass.cpp \
        Class/MenuClass/PrivateMenuClass.cpp \
        Class/MenuClass/PrivateMenuItemClass.cpp \
        Class/PrivateEventClass.cpp \
        Class/SelfClearInterFaceClass.cpp \
        Class/SimFlash/FlashSimClass.cpp \
        Class/SimFlash/SimFlashDrawControl.cpp \
        Class/SpiCaptureDataClass.cpp \
        Class/SpiCmdInfoClass.cpp \
        Class/TransionEffect.cpp \
        Control/GuiDrawControl.cpp \
        Control/HexDataClass.cpp \
        Model/LogViewModel.cpp \
        Model/MainModelClass.cpp \
        Model/serialconfigmodel.cpp \
        Model/spiviewmodel.cpp \
        Model/xhdlogeventclass.cpp \
        main.cpp

RESOURCES += qml.qrc

TRANSLATIONS += \
    TestPlatProject_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Class/FileAnalyseBaseClass.h \
    Class/MenuClass/PrivateMenuClass.h \
    Class/MenuClass/PrivateMenuItemClass.h \
    Class/PrivateEventClass.h \
    Class/SelfClearInterFaceClass.h \
    Class/SimFlash/FlashSimClass.h \
    Class/SimFlash/SimFlashDrawControl.h \
    Class/SpiCaptureDataClass.h \
    Class/SpiCmdInfoClass.h \
    Class/TransionEffect.h \
    Control/GuiDrawControl.h \
    Control/HexDataClass.h \
    Include/CommonStruct.h \
    Model/LogViewModel.h \
    Model/MainModelClass.h \
    Model/serialconfigmodel.h \
    Model/spiviewmodel.h \
    Model/xhdlogeventclass.h

DISTFILES +=
