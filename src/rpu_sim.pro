QT       += core gui

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
    cmt_window.cpp \
    cycle_window.cpp \
    register_window.cpp \
    memory_window.cpp \
    code_window.cpp \
    pipeline_window.cpp \
    mainwindow.cpp \
    main.cpp \
    rpu/RPU.cpp \
    rpu/rpu/IFID.cpp \
    rpu/rpu/IDEX.cpp \
    rpu/rpu/EXMM.cpp \
    rpu/rpu/MMWB.cpp \
    rpu/rpu/TU.cpp \
    rpu/rpu/PCGroup.cpp \
    rpu/rpu/RegisterFileGroup.cpp \
    rpu/rpu/ThreadControl.cpp \
    rpu/rpu/Hazard.cpp

HEADERS += \
    algo.h \
    external/enum.h \
    external/vsrtl_enum.h \
    rpu/RPU.h \
    rpu/include/Common.h \
    rpu/include/Register.hpp \
    rpu/mm/Memory.hpp \
    rpu/mm/Burn.hpp \
    rpu/rpu/IFID.h \
    rpu/rpu/IDEX.h \
    rpu/rpu/EXMM.h \
    rpu/rpu/MMWB.h \
    rpu/rpu/TU.h \
    rpu/rpu/PCGroup.h \
    rpu/rpu/RegisterFileGroup.h \
    rpu/rpu/ThreadControl.h \
    rpu/rpu/Hazard.h \
    rpu/rpu/RegisterFile.hpp \
    cmt_window.h \
    cycle_window.h \
    register_window.h \
    memory_window.h \
    code_window.h \
    pipeline_window.h \
    mainwindow.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
