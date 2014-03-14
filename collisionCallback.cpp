#ifndef COLLISIONCALLBACK
#define COLLISIONCALLBACK

#define dDOUBLE
#include <ode/ode.h>
#include "selector.h"
#include "machinepiece.h"
#include "wheelpiece.h"

static dWorldID gWorld;
static dJointGroupID gContactGroup;

 static void nearCallback (void *data, dGeomID o1, dGeomID o2)
{
    // exit without doing anything if the two bodies are connected by a joint
    dBodyID b1 = dGeomGetBody(o1);
    dBodyID b2 = dGeomGetBody(o2);

    const int MAX_CONTACTS = 8;
    dContact contact[MAX_CONTACTS];

    int numc = dCollide (o1, o2, MAX_CONTACTS,
                        &contact[0].geom,
                        sizeof(dContact));

    for (int i=0; i<numc; i++) {
        contact[i].surface.mode = dContactApprox1;
        contact[i].surface.mu = 5;
        dJointID c = dJointCreateContact (gWorld, gContactGroup, contact+i);
        dJointAttach (c, b1, b2);
    }
}

static void selectorCallback (void *data, dGeomID o1, dGeomID o2)
{
    // find the non ray and send it's depth to the selector object given in the data field

    dGeomID tempGeom,
            ray;

    // if either is a space, return
    if(dGeomIsSpace(o1) || dGeomIsSpace(o2) ||
            dGeomGetClass(o1) == dPlaneClass || dGeomGetClass(o2) == dPlaneClass)
        return;

    if(dGeomGetClass(o1) == dRayClass)
    {
        tempGeom = o2;
        ray = o1;
    }
    else
    {
        tempGeom = o1;
        ray = o2;
    }

    dContactGeom contact[1];

    if(dCollide (o1, o2, 1, &contact[0], sizeof(dContact)) <= 0)
        return;

  //  Selector* sel = static_cast<Selector*>(data);
    static_cast<Selector*>(data)->clickBody(static_cast<MachinePiece*>(dGeomGetData(tempGeom)),contact[0].depth);
}

#endif
