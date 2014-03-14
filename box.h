#ifndef BOX_H
#define BOX_H
#include <ode/ode.h>
#include "glheader.h"
#include <OpenGL/gl3.h>

class box
{
public:
    box(dWorldID&,dSpaceID&, float x, float y, float z);
    ~box();

    void draw(unsigned int* buffer, unsigned int shader, float cameraX, float cameraY, float cameraZ, float cameraAzimuth, float cameraAltitude);
    void setLinearVelocity(float x, float y, float z);

    dBodyID body;
    dGeomID geom;
    static const dReal sides[3];
};

#endif // BOX_H
