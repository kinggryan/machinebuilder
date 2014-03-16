#ifndef SELECTOR_H
#define SELECTOR_H
#define dDOUBLE
#include <ode/ode.h>
#include "MachinePieces/machinepiece.h"

class Selector
{
public:
    Selector(float cameraX, float cameraY, float cameraZ, float cameraAzimuth, float cameraThetaAltitude, int clickX, int clickY, float cameraFOVy, int viewSize, float nearestDistance, dWorldID world, dSpaceID space);
    ~Selector();

    void clickBody(MachinePiece* clickedBody, float depth);
    MachinePiece* getSelectedPiece();
    int getSelectedPieceFace();

protected:
    float nearestObjectDistance;
    MachinePiece* selectedPiece;
    dGeomID geom;
    dSpaceID raySpace;
};

#endif // SELECTOR_H
