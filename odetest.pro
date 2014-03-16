#-------------------------------------------------
#
# Project created by QtCreator 2014-02-22T18:31:47
#
#-------------------------------------------------

QT       += core gui
QT += widgets
QT += opengl

DEFINES += GL_GLEXT_PROTOTYPES

mac {
    eval(QT_MAJOR_VERSION = "4") {
        LIBS += -framework Foundation -framework Cocoa
        OBJECTIVE_SOURCES  +=  core_profile_attributes.mm
    }
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = odetest
TEMPLATE = app

#LIBS += /Users/kinggryan/Desktop/ode-0.13/ode/src -libode.la

#LIBS += -L/Users/kinggryan/ode-0.13/lib/DebugDoubleDLL -libode_doubled.dylib

SOURCES += main.cpp\
        mainwindow.cpp \
    glheader.cpp \
    gldisplay.cpp \
    box.cpp \
    levelmanager.cpp \
    collisionCallback.cpp \
    groundtexture.cpp \
    selector.cpp \
    MachinePieces/wheelpiece.cpp \
    MachinePieces/cubebasepiece.cpp

HEADERS  += mainwindow.h \
    glheader.h \
    Core3_2_context.h \
    gldisplay.h \
    box.h \
    levelmanager.h \
    collisionCallback.h \
    groundtexture.h \
    selector.h \
    MachinePieces/machinepiece.h \
    MachinePieces/cubebasepiece.h \
    MachinePieces/wheelpiece.h \
    MachinePieces/piecetypes.h

INCLUDEPATH += /Users/kinggryan/ode-0.13/include

FORMS    += mainwindow.ui

OTHER_FILES += \
    VertexShader.vertexshader \
    FragmentShader.fragmentshader

mac {
    APP_SHADER_FILES.files = VertexShader.vertexshader FragmentShader.fragmentshader
    APP_SHADER_FILES.path = Contents/Resources
    QMAKE_BUNDLE_DATA += APP_SHADER_FILES
}

macx: LIBS += -L/Users/kinggryan/ode-0.13/lib/DebugDoubleDLL/ -lode_doubled
       # LIBS += -L/Users/kinggryan/ode-0.13/lib/DebugDoubleDLL/ -libode_doubled.dylib

INCLUDEPATH += /Users/kinggryan/ode-0.13/lib/DebugDoubleDLL
DEPENDPATH += /Users/kinggryan/ode-0.13/lib/DebugDoubleDLL
