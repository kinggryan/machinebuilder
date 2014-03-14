#ifndef MACHINEPIECE_H
#define MACHINEPIECE_H
#define dDOUBLE
#include <ode/ode.h>
#include "glheader.h"
#include <OpenGL/gl3.h>

// MachinePiece is abstract : all other machine pieces inherit from it

class MachinePiece
{
public:
    ~MachinePiece() {    dBodyDestroy(body);
                         dGeomDestroy(geom); }

    virtual void draw(unsigned int* buffer, unsigned int shader, float cameraX, float cameraY, float cameraZ, float cameraAzimuth, float cameraAltitude) = 0;
    void setLinearVelocity(float x, float y, float z) { dBodySetLinearVel(body,x,y,z); }
    virtual void attachToBase(dBodyID body, dWorldID world, dJointGroupID jointGroup, dReal x, dReal y, dReal z, const dMatrix3 rotationMatrix) = 0;
    virtual float getAttachmentOffset() = 0;
    virtual void activate() = 0;

    // Selector related functions
    virtual int selectFace(int& numFaces, dSpaceID space, dGeomID ray, int cameraX, int cameraY, int cameraZ) = 0;

    dBodyID body;
    dGeomID geom;
    GLfloat color[3];
};

#endif // MACHINEPIECE_H
