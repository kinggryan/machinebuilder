#include "builderaddpieceselector.h"

BuilderAddPieceSelector::BuilderAddPieceSelector(float cameraX, float cameraY, float cameraZ, float cameraAzimuth, float cameraThetaAltitude, int clickX, int clickY, float cameraFOVy, int viewSize, float nearestDistance, dWorldID world, dSpaceID space)
    : Selector(cameraX, cameraY, cameraZ, cameraAzimuth, cameraThetaAltitude, clickX, clickY, cameraFOVy, viewSize, nearestDistance, world, space)
{

}

BuilderAddPieceSelector::~BuilderAddPieceSelector()
{

}

void BuilderAddPieceSelector::clickBodyClass(MachinePiece* clickedBody, float depth)
{
    if(depth <= nearestObjectDistance)
        selectedPiece = clickedBody;

    // TODO actions involving clicked body
}
