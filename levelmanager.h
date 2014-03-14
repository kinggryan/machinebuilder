#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H
#define dDOUBLE
#include <ode/ode.h>

class levelmanager
{
public:
    levelmanager();

    static dSpaceID space;
    static dWorldID world;
    static dJointGroupID contactgroup;
};

#endif // LEVELMANAGER_H
