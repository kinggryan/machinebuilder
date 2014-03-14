#ifndef WHEELPIECE_H
#define WHEELPIECE_H
#include <ode/ode.h>
#include "machinepiece.h"
#include "glheader.h"
#include <OpenGL/gl3.h>

class WheelPiece : public MachinePiece
{
public:
    WheelPiece(dWorldID&,dSpaceID&, float x, float y, float z);
    ~WheelPiece() {}

    void draw(unsigned int* buffer, unsigned int shader, float cameraX, float cameraY, float cameraZ, float cameraAzimuth, float cameraAltitude);
    void attachToBase(dBodyID otherBody, dWorldID world, dJointGroupID jointGroup, dReal x, dReal y, dReal z, const dMatrix3 rotationMatrix);
    float getAttachmentOffset();
    void activate();

    // Selector Related Functions
    int selectFace(int& numFaces, dSpaceID space, dGeomID ray, int cameraX, int cameraY, int cameraZ);

    float radius;
    float thickness;
    float attachmentOffset;

    dJointID connectingJoint;
};

#endif // WHEELPIECE_H
