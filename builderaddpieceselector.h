#ifndef BUILDERADDPIECESELECTOR_H
#define BUILDERADDPIECESELECTOR_H
#include "selector.h"

class BuilderAddPieceSelector : public Selector
{
public:
    BuilderAddPieceSelector(float cameraX, float cameraY, float cameraZ, float cameraAzimuth, float cameraThetaAltitude, int clickX, int clickY, float cameraFOVy, int viewSize, float nearestDistance, dWorldID world, dSpaceID space);
    ~BuilderAddPieceSelector();

    void clickBodyClass(MachinePiece* clickedBody, float depth);
    void clickBody(MachinePiece *clickedBody, float depth) {}
};

#endif // BUILDERADDPIECESELECTOR_H
