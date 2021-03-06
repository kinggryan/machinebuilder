#define dDOUBLE // for ode

#include "gldisplay.h"
#include "Core3_2_context.h"
#include "glheader.h"
#include <OpenGL/gl3.h>
#include <QGLWidget>
#include <QCoreApplication>
#include "collisionCallback.cpp"
#include <QKeyEvent>
#include <cmath>
#include "groundtexture.h"
#include "MachinePieces/machinepiece.h"
#include "MachinePieces/cubebasepiece.h"
#include "MachinePieces/wheelpiece.h"

#define STEP_SIZE .01
#define MOUSE_PIXEL_RADIAN_RATIO .005

static const GLfloat g_vertex_buffer_data[16] = {    -1.0f, -1.0f, -1.5f, 1.0f,
                                               +1.0f, -1.0f, -1.5f, 1.0f,
                                               +1.0f, +1.0f, -1.5f, 1.0f,
                                               -1.0f, +1.0f, -1.5f, 1.0f};

static const GLfloat g_normal_buffer_data[12] = {0,0,1,
                                         0,0,1,
                                         0,0,1,
                                         0,0,1};
static const GLuint g_element_buffer_data[6] = {0,1,2,
                                         2,0,3};

gldisplay::gldisplay(QWidget *parent) : QGLWidget(new Core3_2_context(QGLFormat::defaultFormat()), parent) {
    bufferID[0] = 0;
    bufferID[1] = 0;
    bufferID[2] = 0;

    cameraFOVy = 1.5;
    cameraNear = .5;
    cameraFar = 40.0;
    cameraPosX = 0;
    cameraPosY = .25;
    cameraPosZ = 0;

    // Azimuth measures angle from camera direction to the negative z axis parallel to x axis
    cameraAzimuth = 0;
    // theta measures angle from camera direction to the negative z axis parallel to the y axis
    cameraThetaAltitude = 0;

    world = dWorldCreate();
    space = dSimpleSpaceCreate(0);
    contactGroup = dJointGroupCreate(0);

    gWorld = world;
    gContactGroup = contactGroup;

    dWorldSetQuickStepNumIterations(world,30);
    dWorldSetERP(world,.2);
    dWorldSetCFM(world,0);

    numberOfPieces = 0;

    dJointGroupID machineJointGroup = dJointGroupCreate(0);

    // Make pieces null
    pieces = NULL;

    addPiece(new CubeBasePiece(world,space,0,.45,-2.25));
 //   pieces[0] = (new CubeBasePiece(world,space,0,.45,-2.25));
  /*  pieces[1] = new WheelPiece(world,space,.65,1.25,-2.5);
    pieces[2] = new WheelPiece(world,space,.65,1.25,-2.5);
    pieces[3] = new CubeBasePiece(world,space,0,0,0);
    pieces[4] = new CubeBasePiece(world,space,0,0,0);
    pieces[5] = new WheelPiece(world,space,.65,1.25,-2.5);
    pieces[6] = new WheelPiece(world,space,.65,1.25,-2.5);
    dynamic_cast<CubeBasePiece*>(pieces[0])->attachPieceAtPosition(pieces[1],5,world,machineJointGroup);
    dynamic_cast<CubeBasePiece*>(pieces[0])->attachPieceAtPosition(pieces[2],4,world,machineJointGroup);
    dynamic_cast<CubeBasePiece*>(pieces[0])->attachPieceAtPosition(pieces[3],0,world,machineJointGroup);
    dynamic_cast<CubeBasePiece*>(pieces[3])->attachPieceAtPosition(pieces[4],0,world,machineJointGroup);
    dynamic_cast<CubeBasePiece*>(pieces[4])->attachPieceAtPosition(pieces[5],5,world,machineJointGroup);
    dynamic_cast<CubeBasePiece*>(pieces[4])->attachPieceAtPosition(pieces[6],4,world,machineJointGroup);
*/
//    pieces[1]->activate();
//    pieces[2]->activate();

   // groundtex = new groundtexture();

    ground = dCreatePlane(space, 0, 1, 0, 0);
 //   dWorldSetGravity(world, 0, -.45, 0);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    timer->start(1000 * STEP_SIZE);

    // Enable Keyboard Input
    setFocusPolicy(Qt::StrongFocus);

    // set look-around mode
    currentActionMode = modeCamera;
}

gldisplay::~gldisplay()
{
    for (int i = 0 ; i < numberOfPieces ; i++)
        delete pieces[i];
    delete pieces;
//    delete groundtex;
    delete timer;
}

void gldisplay::initializeGL()
{
    glSetup();

    // Black background
    glClearColor(0.0, 0.0, 0.0, 0.0);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    // Create three buffers, one for vertices, one for normals, one for indices
    glGenBuffers(3, bufferID);

#if defined(Q_OS_MAC)
    QString shaderDir = QCoreApplication::applicationDirPath() + QString("/../Resources/");
    QString vertexShaderFile = shaderDir + QString("VertexShader.vertexshader");
    QString fragmentShaderFile = shaderDir + QString("FragmentShader.fragmentshader");
    shaderID = LoadShaders(vertexShaderFile.toLatin1().constData(), fragmentShaderFile.toLatin1().constData());
#else
    shaderID = LoadShaders("VertexShader.vertexshader", "FragmentShader.fragmentshader");
#endif
}

void gldisplay::paintGL()
{
    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderID);

    for(int i = 0 ; i < numberOfPieces ; i++)
        pieces[i]->draw(bufferID,shaderID,cameraPosX,cameraPosY,cameraPosZ, cameraAzimuth, cameraThetaAltitude);

    // draw boxes
  //  pieces[0]->draw(bufferID,shaderID,cameraPosX,cameraPosY,cameraPosZ, cameraAzimuth, cameraThetaAltitude);
   // pieces[1]->draw(bufferID,shaderID,cameraPosX,cameraPosY,cameraPosZ, cameraAzimuth, cameraThetaAltitude);
   // groundtex->draw1(bufferID,shaderID,cameraPosX,cameraPosY,cameraPosZ, cameraAzimuth, cameraThetaAltitude);
  //  groundtex->draw2(bufferID,shaderID,cameraPosX,cameraPosY,cameraPosZ, cameraAzimuth, cameraThetaAltitude);
 //   groundtex->draw3(bufferID,shaderID,cameraPosX,cameraPosY,cameraPosZ, cameraAzimuth, cameraThetaAltitude);
    // TODO draw ground
}

void gldisplay::stepWorld()
{
    dSpaceCollide(space, 0, &nearCallback);
    dWorldQuickStep(world,STEP_SIZE);
    dJointGroupEmpty(contactGroup);
    updateGL();
}

void gldisplay::updateTime()
{
    stepWorld();
}

void gldisplay::keyPressEvent(QKeyEvent* key)
{
    switch(key->key())
    {
    case Qt::Key_W:
        cameraPosZ += .1 * cos(cameraThetaAltitude) * cos(cameraAzimuth);
        cameraPosX += .1 * cos(cameraThetaAltitude) * sin(cameraAzimuth);
        cameraPosY -= .1 * -sin(cameraThetaAltitude);
        break;
    case Qt::Key_S:
        cameraPosZ += -.1 * cos(cameraThetaAltitude) * cos(cameraAzimuth);
        cameraPosX += -.1 * cos(cameraThetaAltitude) * sin(cameraAzimuth);
        cameraPosY -= -.1 * -sin(cameraThetaAltitude);
        break;
    case Qt::Key_A:
        cameraPosZ += .1 * cos(cameraThetaAltitude) * cos(cameraAzimuth+(M_PI/2));
        cameraPosX += .1 * cos(cameraThetaAltitude) * sin(cameraAzimuth+(M_PI/2));
        break;
    case Qt::Key_D:
        cameraPosZ += .1 * cos(cameraThetaAltitude) * cos(cameraAzimuth-(M_PI/2));
        cameraPosX += .1 * cos(cameraThetaAltitude) * sin(cameraAzimuth-(M_PI/2));
        break;
    case Qt::Key_Q:
        cameraPosY -= .1;
        break;
    case Qt::Key_E:
        cameraPosY += .1;
        break;
  //  case Qt::Key_J:
    //    currentActionMode = currentActionMode ^ 1; break; // flip action mode
    }
}

void gldisplay::mousePressEvent(QMouseEvent *event)
{
    if(currentActionMode == modeCamera)
    {
        // store clicks
        mouseClickX = event->x();
        mouseClickY = event->y();
    }
    else if(currentActionMode == modeAddPiece)
    {
        // select piece and face
        Selector* sel = new Selector(cameraPosX,cameraPosY,-cameraPosZ,cameraAzimuth,cameraThetaAltitude,event->x(),event->y(),cameraFOVy,500,0,world,space);
        int tempFace = sel->getSelectedPieceFace();

        if(tempFace == -1) // if invalid
        {
            currentActionMode = modeCamera;
            delete sel;
        }
        else
        {
            // add a new piece
            switch(pieceTypeToAdd)
            {
            case wheelPiece:
                addPiece(new WheelPiece(world,space,0,0,0)); break;
            }

            // and connect it
            dJointGroupID machineJointGroup = dJointGroupCreate(0);
            dynamic_cast<CubeBasePiece*>(sel->getSelectedPiece())->attachPieceAtPosition(pieces[numberOfPieces-1],tempFace,world,machineJointGroup);
            delete sel;

            // return to look mode
            currentActionMode = modeCamera;
        }
    }
    else if(currentActionMode == modeActivatePiece)
    {
        Selector* sel = new Selector(cameraPosX,cameraPosY,-cameraPosZ,cameraAzimuth,cameraThetaAltitude,event->x(),event->y(),cameraFOVy,500,0,world,space);
        MachinePiece* selectedPiece = sel->getSelectedPiece();

        if(selectedPiece != NULL)
        {
            selectedPiece->activate();
        }

        delete sel;
        currentActionMode = modeCamera;
    }
    else if(currentActionMode == modeChangeActivationDirection)
    {
        Selector* sel = new Selector(cameraPosX,cameraPosY,-cameraPosZ,cameraAzimuth,cameraThetaAltitude,event->x(),event->y(),cameraFOVy,500,0,world,space);
        MachinePiece* selectedPiece = sel->getSelectedPiece();

        if(selectedPiece != NULL)
        {
            selectedPiece->changeActivationDirection();
        }

        delete sel;
        currentActionMode = modeCamera;
    }
}

void gldisplay::mouseMoveEvent(QMouseEvent *event)
{
    if(currentActionMode == modeCamera)
    {
        // Turn viewpoint if mouse is being held down on screen
        cameraAzimuth += MOUSE_PIXEL_RADIAN_RATIO * (event->x() - mouseClickX);
        cameraThetaAltitude += MOUSE_PIXEL_RADIAN_RATIO * (event->y() - mouseClickY);
        mouseClickX = event->x();
        mouseClickY = event->y();
    }
}


void gldisplay::addPiece(MachinePiece* pieceToAdd)
{
    // resize piece array
    if(pieces != NULL)
    {
        MachinePiece* tempPieceArray[numberOfPieces+1];

        for(int i = 0 ; i < numberOfPieces ; i++)
            tempPieceArray[i] = pieces[i];

        delete pieces;

        pieces = new MachinePiece*[++numberOfPieces];
        for(int i = 0 ; i < numberOfPieces-1 ; i++)
            pieces[i] = tempPieceArray[i];

        pieces[numberOfPieces-1] = pieceToAdd;
    }
    else
    {
        pieces = new MachinePiece*[++numberOfPieces];
        pieces[0] = pieceToAdd;
    }
}

void gldisplay::setAddPieceMode(pieceType piece)
{
    currentActionMode = modeAddPiece;
    pieceTypeToAdd = piece;
}

void gldisplay::activateGravity()
{
    dWorldSetGravity(world, 0, -.45, 0);
}

void gldisplay::activatePieceMode()
{
    currentActionMode = modeActivatePiece;
}

void gldisplay::changeActivationDirectionMode()
{
    currentActionMode = modeChangeActivationDirection;
}
