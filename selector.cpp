#include "selector.h"
#include <cmath>
#include "collisionCallback.cpp"
#define MAX_SELECT_DISTANCE 50


Selector::Selector(float cameraX, float cameraY, float cameraZ, float cameraAzimuth, float cameraThetaAltitude, int clickX, int clickY, float cameraFOVy, int viewSize, float nearestDistance, dWorldID world, dSpaceID space)
    // on creation, check for nearest z collision beyond the near clip plane and call the "selected" function for the data on the selected body.
{
    selectedPiece = NULL;
    geom = dCreateRay(space,MAX_SELECT_DISTANCE);
    nearestObjectDistance = MAX_SELECT_DISTANCE+100;
    dGeomSetData(geom, this);
    raySpace = space;

    // fix rotational directions
    cameraAzimuth *= -1;
    cameraThetaAltitude *= -1;

    cameraAzimuth += cameraFOVy*((1.0*clickX/viewSize)-.5);
    cameraThetaAltitude += cameraFOVy*((1.0*clickY/viewSize)-.5);

    dGeomRaySet(geom,cameraX,cameraY,cameraZ,(-1*sin(cameraAzimuth)),(-1*sin(cameraThetaAltitude)*cos(cameraAzimuth)),(-1*cos(cameraAzimuth)*cos(cameraThetaAltitude)));

    dGeomID gSpace = (dGeomID)(space);
    dSpaceCollide2(gSpace,geom,this,&selectorCallback); // pass this object's pointer so we can call functions in callback
}

Selector::~Selector()
{
    dGeomDestroy(geom);
}

void Selector::clickBody(MachinePiece *clickedBody, float depth)
{
    if(depth <= nearestObjectDistance)
        selectedPiece = clickedBody;
}

MachinePiece* Selector::getSelectedPiece()
{
    return selectedPiece;
}

int Selector::getSelectedPieceFace(int cameraX, int cameraY, int cameraZ)
{
    if(selectedPiece == NULL)
        return -1;

    int numFaces;

    selectedPiece->selectFace(numFaces,raySpace,geom,cameraX,cameraY,cameraZ);
    // call function in selected piece and return geom array
}
