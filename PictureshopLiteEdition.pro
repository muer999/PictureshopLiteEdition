# QT for DIP
# author    CWH CXF DSQ HHQ YW from SHU
# date      2020-03-04
CONFIG += c++11
QT += widgets
TARGET = PictureshopLiteEdition

RC_FILE += icon.rc

SOURCES += \
    main.cpp

RESOURCES += \
    images.qrc

INCLUDEPATH += \
    mainwindow \
    F:\OpenCV4.1.2-mingw530-32\install\include \
    F:\dlib-mingw32\include

LIBS += \
    F:\OpenCV4.1.2-mingw530-32\install\x86\mingw\lib\libopencv_*.dll.a \
    F:\dlib-mingw32\lib\libdlib.a

include(mainwindow/mainwindow.pri)
include(imageviewer/imageviewer.pri)
include(advancedimageprocessing/advancedimageprocessing.pri)

HEADERS += \


