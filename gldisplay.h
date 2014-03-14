#ifndef GLDISPLAY_H
#define GLDISPLAY_H

#define dDOUBLE

#include "glheader.h"
#include <QGLWidget>
#include <OpenGL/gl3.h>
#include "Core3_2_context.h"
#include "box.h"
#include <QTimer>
#include "groundtexture.h"
#include "machinepiece.h"
#include "cubebasepiece.h"
#include <vector>

class gldisplay : public QGLWidget
{
    Q_OBJECT

public:
    gldisplay(QWidget* parent);
    ~gldisplay();
    void keyPressEvent(QKeyEvent* key);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent *event);

protected:
    void initializeGL();
    void paintGL();

    void stepWorld();

    unsigned int bufferID[3];
    unsigned int shaderID;
    unsigned int vertexArrayID;

    float   cameraNear,
            cameraFar,
            cameraFOVy,
            cameraAspect,
            cameraPosX,
            cameraPosY,
            cameraPosZ,
            cameraAzimuth,
            cameraThetaAltitude;

    int     mouseClickX,
            mouseClickY;

    dWorldID world;
    dSpaceID space;
    dJointGroupID contactGroup;
    dGeomID ground;

   // CubeBasePiece* bbox;
   // MachinePiece* cbox;
    MachinePiece** pieces;
    groundtexture* groundtex;

    QTimer* timer;

    int actionMode;

public slots:
    void updateTime();
};

#endif // GLDISPLAY_H
