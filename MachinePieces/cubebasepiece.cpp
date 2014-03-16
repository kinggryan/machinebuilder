#define dDOUBLE

#include "cubebasepiece.h"
#include <ode/ode.h>

#define CUBE_PIECE_DENSITY 2.5

static const dReal  cwidth=.5,
                    cthikness=.5,
                    clength=.5;
const dReal CubeBasePiece::sides[3] = { cwidth, cthikness, clength };

CubeBasePiece::CubeBasePiece(dWorldID& world,dSpaceID& space, float x, float y, float z)
{
    body = dBodyCreate(world);
    geom = dCreateBox(space, sides[0], sides[1], sides[2]);
    dGeomSetBody(geom, body);
    dGeomSetData(geom, this);
    dMass mass;
    mass.setBox(CUBE_PIECE_DENSITY, sides[0], sides[1], sides[2]);
    dBodySetMass(body, &mass);

    const dMatrix3 rotationMatrix = {1,0,0,0,
                                     0,1,0,0,
                                     0,0,1,0};

    dBodySetRotation(body, rotationMatrix);

    dBodySetPosition(body,x,y,z);

    for(int i = 0 ; i < 6 ; i++)
        attachedPieces[i] = NULL; // initialize attached piece array to all null pointers

    color[0] = 1;
    color[1] = 1;
    color[2] = 1;
}

void CubeBasePiece::draw(unsigned int *buffer, unsigned int shader, float cameraX, float cameraY, float cameraZ, float cameraAzimuth, float cameraAltitude)
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

    // Pass negative y because of weird discrepancy between camera and physics coords
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

bool CubeBasePiece::attachPieceAtPosition(MachinePiece* piece, int position, dWorldID world, dJointGroupID jointGroup)
{
    // takes piece and attaches it to this machine at the given position. If that position is taken, returns false. Otherwise returns true

    if(attachedPieces[position] != NULL)
        return false;

    attachedPieces[position] = piece;

    const dReal* attachPos = dBodyGetPosition(body);

    // even numbers are +, odd are -
    // 0,1 are x
    // 2,3 are y
    // 4,5 are z
    switch(position)
    {
    case 0:
    {
        const dReal rotationMatrix[12] = { cos(M_PI/2), 0.0, -sin(M_PI/2), 0.0,
                                          0.0, 1.0, 0.0, 0.0,
                                          sin(M_PI/2), 0.0, cos(M_PI/2), 0.0 };
        piece->attachToBase(body,world,jointGroup,attachPos[0]+piece->getAttachmentOffset(),attachPos[1],attachPos[2], rotationMatrix); break;
    }
    case 1:
    {
        const dReal rotationMatrix[12] = { cos(3*M_PI/2), 0.0, -sin(3*M_PI/2), 0.0,
                                          0.0, 1.0, 0.0, 0.0,
                                          sin(3*M_PI/2), 0.0, cos(3*M_PI/2), 0.0 };
        piece->attachToBase(body,world,jointGroup,attachPos[0]-piece->getAttachmentOffset(),attachPos[1],attachPos[2], rotationMatrix); break;
    }
    case 2:
    {
        const dReal rotationMatrix[12] = { 1.0, 0.0, 0.0, 0.0,
                                          0.0, cos(M_PI/2), sin(M_PI/2), 0.0,
                                          0.0, -sin(M_PI/2), cos(M_PI/2), 0.0 };
        piece->attachToBase(body,world,jointGroup,attachPos[0],attachPos[1]+piece->getAttachmentOffset(),attachPos[2], rotationMatrix); break;
    }
    case 3:
    {
        const dReal rotationMatrix[12] = { 1.0, 0.0, 0.0, 0.0,
                                          0.0, cos(3*M_PI/2), sin(3*M_PI/2), 0.0,
                                          0.0, -sin(3*M_PI/2), cos(3*M_PI/2), 0.0 };
        piece->attachToBase(body,world,jointGroup,attachPos[0],attachPos[1]-piece->getAttachmentOffset(),attachPos[2], rotationMatrix); break;
    }
    case 4:
    {
        const dReal rotationMatrix[12] = { 1.0, 0.0, 0.0, 0.0,
                                          0.0, 1.0, 0.0, 0.0,
                                          0.0, 0.0, 1.0, 0.0 };
        piece->attachToBase(body,world,jointGroup,attachPos[0],attachPos[1],attachPos[2]+piece->getAttachmentOffset(), rotationMatrix); break;
    }
    case 5:
    {
        const dReal rotationMatrix[12] = { 0.0, 1.0, 0.0, 0.0,
                                          -1.0, 0.0, 0.0, 0.0,
                                          0.0, 0.0, 1.0, 0.0 };
        piece->attachToBase(body,world,jointGroup,attachPos[0],attachPos[1],attachPos[2]-piece->getAttachmentOffset(), rotationMatrix); break;
    }
    }

    return true;
}

void CubeBasePiece::attachToBase(dBodyID otherBody, dWorldID world, dJointGroupID jointGroup, dReal x, dReal y, dReal z, const dMatrix3 rotationMatrix)
{
    dBodySetPosition(body,x,y,z);

    dJointID connectingJoint = dJointCreateSlider(world,jointGroup);
    dJointAttach(connectingJoint,otherBody,body);
    dJointSetSliderParam(connectingJoint,dParamLoStop,0);
    dJointSetSliderParam(connectingJoint,dParamHiStop,0);
}

int CubeBasePiece::selectFace(int& numFaces, dSpaceID space, dGeomID ray, float cameraX, float cameraY, float cameraZ)
{
    numFaces = 6;

    const dReal* rotationMatrix = dGeomGetRotation(geom);
    const dReal* currentPosition = dGeomGetPosition(geom);

    dGeomID faces[6];

    // create six faces
    faces[0] = dCreateBox(space,.01,sides[1],sides[2]);
    faces[1] = dCreateBox(space,.01,sides[1],sides[2]);
    dGeomSetPosition(faces[0],currentPosition[0]-(sides[0]/2),currentPosition[1],currentPosition[2]);
    dGeomSetPosition(faces[1],currentPosition[0]+(sides[0]/2),currentPosition[1],currentPosition[2]);
    faces[2] = dCreateBox(space,sides[0],.01,sides[2]);
    faces[3] = dCreateBox(space,sides[0],.01,sides[2]);
    dGeomSetPosition(faces[2],currentPosition[0],currentPosition[1]+(sides[1]/2),currentPosition[2]);
    dGeomSetPosition(faces[3],currentPosition[0],currentPosition[1]-(sides[1]/2),currentPosition[2]);
    faces[4] = dCreateBox(space,sides[0],sides[1],.01);
    faces[5] = dCreateBox(space,sides[0],sides[1],.01);
    dGeomSetPosition(faces[4],currentPosition[0],currentPosition[1],currentPosition[2]+(sides[2]/2));
    dGeomSetPosition(faces[5],currentPosition[0],currentPosition[1],currentPosition[2]-(sides[2]/2));

    int selectedFace;
    int selectedPointDistance = 1000;
    dContactGeom contact[1];


    // find collision point closest to camera
    for(int i = 0 ; i < 6 ; i++)
    {
        // rotate geom before collision
        dGeomSetRotation(faces[i],rotationMatrix);

        if(dCollide(faces[i],ray,1,&contact[0],sizeof(dContact)) != 0)
        {
            float tempDistance = pow(contact[0].pos[0] - cameraX,2) + pow(contact[0].pos[1] - cameraY,2) + pow(contact[0].pos[2] - cameraZ,2);
            if(tempDistance <= selectedPointDistance)
            {
                selectedPointDistance = tempDistance;
                selectedFace = i;
            }
        }

        dGeomDestroy(faces[i]); // delete the geoms
    }

    return selectedFace;
}
