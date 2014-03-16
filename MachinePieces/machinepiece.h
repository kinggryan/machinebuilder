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
    virtual void changeActivationDirection() = 0;
    void setColor(float r, float g, float b)
        {
            color[0] = r;
            color[1] = g;
            color[2] = b;
        }

    // Selector related functions
    virtual int selectFace(int& numFaces, dSpaceID space, dGeomID ray, float cameraX, float cameraY, float cameraZ) = 0;

    // public data
    dBodyID body;
    dGeomID geom;
    GLfloat color[3];
    int activationDirection;
};

#endif // MACHINEPIECE_H
