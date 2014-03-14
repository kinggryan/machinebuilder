#define dDOUBLE

#include "box.h"
#include <ode/ode.h>

static const dReal  cwidth=.5,
                    cthikness=.5,
                    clength=.5;
const dReal box::sides[3] = { cwidth, cthikness, clength };

static const GLfloat g_vertex_buffer_data[16] = {    -1.0f, -1.0f, -1.5f, 1.0f,
                                               +1.0f, -1.0f, -1.5f, 1.0f,
                                               +1.0f, +1.0f, -1.5f, 1.0f,
                                               -1.0f, +1.0f, -1.5f, 1.0f};

static const GLfloat g_normal_buffer_data[12] = {0,0,1,
                                         0,0,1,
                                         0,0,1,
                                         0,0,1};
static const GLuint g_element_buffer_data[6] = {0,1,2,
                                         2,0,3};

box::box(dWorldID& world, dSpaceID& space, float x, float y, float z)
{
    dBodyCreate(world);
    body = dBodyCreate(world);
    geom = dCreateBox(space, sides[0], sides[1], sides[2]);
    dGeomSetBody(geom, body);
    dGeomSetData(geom, this);
    dMass mass;
    mass.setBox(1, sides[0], sides[1], sides[2]);
    dBodySetMass(body, &mass);

    const dMatrix3 rotationMatrix = {1,0,0,0,
                                     0,1,0,0,
                                     0,0,1,0};

    dBodySetRotation(body, rotationMatrix);

    dBodySetPosition(body,x,y,z);
}

box::~box()
{
    dBodyDestroy(body);
    dGeomDestroy(geom);
}

void box::draw(unsigned int *buffer, unsigned int shader, float cameraX, float cameraY, float cameraZ, float cameraAzimuth, float cameraAltitude)
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

    GLfloat vertex_buffer_data[32] = { -sides[0]/2,-sides[1]/2,-sides[2]/2,1.0,
                                       sides[0]/2,-sides[1]/2,-sides[2]/2,1.0,
                                       sides[0]/2,sides[1]/2,-sides[2]/2,1.0,
                                       -sides[0]/2,sides[1]/2,-sides[2]/2,1.0,
                                       -sides[0]/2,-sides[1]/2,sides[2]/2,1.0,
                                       sides[0]/2,-sides[1]/2,sides[2]/2,1.0,
                                       sides[0]/2,sides[1]/2,sides[2]/2,1.0,
                                       -sides[0]/2,sides[1]/2,sides[2]/2,1.0};

    GLfloat normal_buffer_data[24] = { -sides[0],-sides[1],-sides[2],
                                       sides[0],-sides[1],-sides[2],
                                       sides[0],sides[1],-sides[2],
                                       -sides[0],sides[1],-sides[2],
                                       -sides[0],-sides[1],sides[2],
                                       sides[0],-sides[1],sides[2],
                                       sides[0],sides[1],sides[2],
                                       -sides[0],sides[1],sides[2]};

    GLuint element_buffer_data[36] = {0,1,2,
                                      0,2,3,
                                      0,3,4,
                                      3,4,7,
                                      3,2,7,
                                      2,6,7,
                                      4,5,7,
                                      5,6,7,
                                      0,1,4,
                                      5,1,4,
                                      1,2,5,
                                      2,5,6};

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
    glUniform1f(glGetUniformLocation(shader, "cameraY"), cameraY);
    glUniform1f(glGetUniformLocation(shader, "cameraZ"), cameraZ);
    glUniform1f(glGetUniformLocation(shader, "azimuth"), cameraAzimuth);
    glUniform1f(glGetUniformLocation(shader, "altitude"), cameraAltitude);
    glUniform1f(glGetUniformLocation(shader, "far"), 40);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[2]);
    glDrawElements(GL_TRIANGLES, sizeof(element_buffer_data) / sizeof(GLuint), GL_UNSIGNED_INT, (void*)0);

    glDisableVertexAttribArray(positionAttribute);

//    delete tempRotMatrix;
//    delete tempPosMatrix;
}

void box::setLinearVelocity(float x, float y, float z)
{
    dBodySetLinearVel(body,x,y,z);
}
