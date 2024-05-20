QT       += core gui graphs

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../numeric_method/MWR.cpp \
    ../numeric_method/Matrix_solver.cpp \
    ../src/Task_manager.cpp \
    ../src/instruction.cpp \
    main.cpp \
    mainwindow.cpp \
    uiMWR.cpp

HEADERS += \
    ../include/MWR.h \
    ../include/Matrix_solver.h \
    ../include/Task_manager.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    htmls.qrc
