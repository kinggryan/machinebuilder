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
#include "MachinePieces/machinepiece.h"
#include "MachinePieces/cubebasepiece.h"
#include <vector>
#include "MachinePieces/piecetypes.h"

enum actionMode {modeCamera,
                 modeAddPiece,
                 modeActivatePiece,
                 modeChangeActivationDirection};

class gldisplay : public QGLWidget
{
    Q_OBJECT

public:
    gldisplay(QWidget* parent);
    ~gldisplay();
    void keyPressEvent(QKeyEvent* key);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent *event);

    // called from GUI
    void setAddPieceMode(pieceType piece);

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
//    std::vector<MachinePiece*> pieces;
    groundtexture* groundtex;

    QTimer* timer;

    actionMode currentActionMode;

private:
    void addPiece(MachinePiece* pieceToAdd);
    int numberOfPieces;
    pieceType pieceTypeToAdd;

public slots:
    void updateTime();
    void activateGravity();
    void activatePieceMode();
    void changeActivationDirectionMode();
};

#endif // GLDISPLAY_H
