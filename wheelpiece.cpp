#define dDOUBLE

#include "wheelpiece.h"
#include <ode/ode.h>

// Wheel rotates along x-y plane, flat side faces z as default

WheelPiece::WheelPiece(dWorldID& world,dSpaceID& space, float x, float y, float z)
{
    dBodyCreate(world);
    body = dBodyCreate(world);

    radius = .45;
    thickness = .25;

    geom = dCreateCylinder(space, radius, thickness);
    dGeomSetBody(geom, body);
    dGeomSetData(geom, this);
    dMass mass;
    mass.setCylinder(.5, 1, radius, thickness);
    dBodySetMass(body, &mass);

    const dMatrix3 rotationMatrix = {1,0,0,0,
                                     0,1,0,0,
                                     0,0,1,0};

    dBodySetRotation(body, rotationMatrix);

    dBodySetPosition(body,x,y,z);

    attachmentOffset = thickness + .25;

    color[0] = .5;
    color[1] = 1;
    color[2] = 1;
}

void WheelPiece::draw(unsigned int *buffer, unsigned int shader, float cameraX, float cameraY, float cameraZ, float cameraAzimuth, float cameraAltitude)
{
    const dReal* tempRotMatrix = dBodyGetRotation(body);
    const dReal* tempPosMatrix = dBodyGetPosition(body);

    // use box' rotation for modelViewMatrix
    GLfloat modelViewMatrix[16] = {
        tempRotMatrix[0], tempRotMatrix[4], tempRotMatrix[8], 0.0,
        tempRotMatrix[1], tempRotMatrix[5], tempRotMatrix[9], 0.0,
        tempRotMatrix[2], tempRotMatrix[6], tempRotMatrix[10], 0.0,
        tempPosMatrix[0], tempPosMatrix[1], tempPosMatrix[2], 1.0 };

    glUniformMatrix4fv(
            glGetUniformLocation(shader, "modelViewMatrix"),
            1, false, modelViewMatrix);

    GLfloat vertex_buffer_data[64] = { 0, -radius, -thickness/2, 1.0,
                                       0, -radius, thickness/2, 1.0,
                                       radius*sin(M_PI/4), -cos(M_PI/4)*radius, -thickness/2, 1.0,
                                       radius*sin(M_PI/4), -cos(M_PI/4)*radius, thickness/2, 1.0,
                                       radius, 0, -thickness/2, 1.0,
                                       radius, 0, thickness/2, 1.0,
                                       radius*sin(3*M_PI/4), -cos(3*M_PI/4)*radius, -thickness/2, 1.0,
                                       radius*sin(3*M_PI/4), -cos(3*M_PI/4)*radius, thickness/2, 1.0,
                                       0, radius, -thickness/2, 1.0,
                                       0, radius, thickness/2, 1.0,
                                       radius*sin(5*M_PI/4), -cos(5*M_PI/4)*radius, -thickness/2, 1.0,
                                       radius*sin(5*M_PI/4), -cos(5*M_PI/4)*radius, thickness/2, 1.0,
                                       -radius, 0, -thickness/2, 1.0,
                                       -radius, 0, thickness/2, 1.0,
                                       radius*sin(7*M_PI/4), -cos(7*M_PI/4)*radius, -thickness/2, 1.0,
                                       radius*sin(7*M_PI/4), -cos(7*M_PI/4)*radius, thickness/2, 1.0,};


    GLfloat normal_buffer_data[48] = { 0,-1,0,
                                       0,-1,0,
                                       sin(M_PI/4),-cos(M_PI/4),0,
                                       sin(M_PI/4),-cos(M_PI/4),0,
                                       1,0,0,
                                       1,0,0,
                                       sin(3*M_PI/4),-cos(3*M_PI/4),0,
                                       sin(3*M_PI/4),-cos(3*M_PI/4),0,
                                       0,1,0,
                                       0,1,0,
                                       sin(5*M_PI/4), -cos(5*M_PI/4),0,
                                       sin(5*M_PI/4), -cos(5*M_PI/4),0,
                                       -1,0,0,
                                       -1,0,0,
                                       sin(7*M_PI/4), -cos(7*M_PI/4),0,
                                       sin(7*M_PI/4), -cos(7*M_PI/4),0
                                     };

    GLuint element_buffer_data[84] = {0,1,2,
                                      1,2,3,
                                      2,3,4,
                                      3,4,5,
                                      4,5,6,
                                      5,6,7,
                                      6,7,8,
                                      7,8,9,
                                      8,9,10,
                                      8,10,11,
                                      10,11,12,
                                      11,12,13,
                                      12,13,14,
                                      13,14,15,
                                      14,15,0,
                                      15,0,1,
                                      0,2,4,
                                      0,4,14,
                                      4,6,12,
                                      4,12,14,
                                      6,10,12,
                                      6,8,10,
                                      1,3,5,
                                      1,5,15,
                                      5,7,13,
                                      5,13,15,
                                      7,11,13,
                                      7,9,11
                                     };

    // Load vertex data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

    // Load normal data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normal_buffer_data), normal_buffer_data, GL_STATIC_DRAW);

    // Load index data into buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(element_buffer_data), element_buffer_data, GL_STATIC_DRAW);

    // Bind attributes for position and normal
    GLuint positionAttribute = glGetAttribLocation(shader, "vertexPos_modelspace");
    glEnableVertexAttribArray(positionAttribute);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    glVertexAttribPointer(positionAttribute, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

    GLuint normalAttribute = glGetAttribLocation(shader, "vertexNormal_modelspace");
    glEnableVertexAttribArray(normalAttribute);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
    glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // assign uniform data
    glUniform1f(glGetUniformLocation(shader, "aspect"), 1);
    glUniform1f(glGetUniformLocation(shader, "cotFOVy"), 1/tan(1.5/2));
    glUniform1f(glGetUniformLocation(shader, "near"), .5);
    glUniform1f(glGetUniformLocation(shader, "cameraX"), cameraX);

    // pass negative y because of weird discrepancy between world and other coords
    glUniform1f(glGetUniformLocation(shader, "cameraY"), -cameraY);
    glUniform1f(glGetUniformLocation(shader, "cameraZ"), cameraZ);
    glUniform1f(glGetUniformLocation(shader, "azimuth"), cameraAzimuth);
    glUniform1f(glGetUniformLocation(shader, "altitude"), cameraAltitude);
    glUniform1f(glGetUniformLocation(shader, "far"), 40);
    glUniform3f(glGetUniformLocation(shader, "objColorIn"), color[0],color[1],color[2]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[2]);
    glDrawElements(GL_TRIANGLES, sizeof(element_buffer_data) / sizeof(GLuint), GL_UNSIGNED_INT, (void*)0);

    glDisableVertexAttribArray(positionAttribute);
}

void WheelPiece::attachToBase(dBodyID otherBody, dWorldID world, dJointGroupID jointGroup, dReal x, dReal y, dReal z, const dMatrix3 rotationMatrix)
{
    dBodySetPosition(body, x, y, z);
    dBodySetRotation(body, rotationMatrix);

/*    dJointID connectingJoint = dJointCreateBall(world,jointGroup);
    dJointAttach(connectingJoint,otherBody,body);
    dJointSetBallAnchor(connectingJoint,otherBodyPosition[0], otherBodyPosition[1], otherBodyPosition[2]+.15);
    dJointID axisRestriction = dJointCreateAMotor(world,jointGroup);
    dJointAttach(axisRestriction,otherBody,body);
    dJointSetAMotorNumAxes(axisRestriction,2);
    dJointSetAMotorAxis(axisRestriction,0,1,1,0,0);
    dJointSetAMotorAxis(axisRestriction,0,1,0,1,0);

    dJointSetAMotorParam(axisRestriction,dParamVel,.1);
    dJointSetAMotorParam(axisRestriction,dParamFMax,.5); */

    connectingJoint = dJointCreateHinge2(world,0);
    dJointAttach (connectingJoint,otherBody,body);
    const dReal *a = dBodyGetPosition (body);
    dJointSetHinge2Anchor (connectingJoint,a[0],a[1],a[2]);
    dJointSetHinge2Axis1 (connectingJoint,1,0,0);
    dJointSetHinge2Axis2 (connectingJoint,0,0,1);
    dJointSetHinge2Param (connectingJoint,dParamLoStop,0);
    dJointSetHinge2Param (connectingJoint,dParamHiStop,0);
}

float WheelPiece::getAttachmentOffset()
{
    return attachmentOffset;
}

void WheelPiece::activate()
{
    dJointSetHinge2Param (connectingJoint,dParamVel2,-2);
    dJointSetHinge2Param (connectingJoint,dParamFMax2,0.7);
}

int WheelPiece::selectFace(int& numFaces, dSpaceID space, dGeomID ray, int cameraX, int cameraY, int cameraZ)
{

}
