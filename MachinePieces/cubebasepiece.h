#ifndef CUBEBASEPIECE_H
#define CUBEBASEPIECE_H
#include <ode/ode.h>
#include "machinepiece.h"
#include "glheader.h"
#include <OpenGL/gl3.h>

class CubeBasePiece : public MachinePiece
{
public:
    CubeBasePiece(dWorldID&,dSpaceID&, float x, float y, float z);
    ~CubeBasePiece() {}

    void draw(unsigned int* buffer, unsigned int shader, float cameraX, float cameraY, float cameraZ, float cameraAzimuth, float cameraAltitude);
    bool attachPieceAtPosition(MachinePiece* piece, int position, dWorldID world, dJointGroupID jointGroup);
    // TODO make attachToBase work for cube
    void attachToBase(dBodyID otherBody, dWorldID world, dJointGroupID jointGroup, dReal x, dReal y, dReal z, const dMatrix3 rotationMatrix);
    float getAttachmentOffset() {return .5;}
    void activate() {}
    void changeActivationDirection() {}

    //SElector Related Functions
    int selectFace(int& numFaces, dSpaceID space, dGeomID ray, float cameraX, float cameraY, float cameraZ);


    static const dReal sides[3];
    MachinePiece* attachedPieces[6];
};

#endif // CUBEBASEPIECE_H
