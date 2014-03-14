#include "groundtexture.h"
#include <cmath>

static const GLfloat testVerty[12] = {-15,0,-15,1,
                         -15,0,15,1,
                         15,0,15,1};

static const GLuint testEls[3] = {0,1,2};

static const GLfloat testNorms[9] = {0,1,0,
                               0,1,0,
                               0,1,0};

groundtexture::groundtexture()
{
    groundVertices = new GLfloat[400];
    groundNormals = new GLfloat[300];

    for(int i = 0 ; i < 100 ; i++)
    {
        groundVertices[4*i]         = (i % 10) - 5;
        groundVertices[(4*i) + 1]   = 0;
        groundVertices[(4*i) + 2]   = (i / 10) - 5;
        groundVertices[(4*i) + 3]   =   1.0;

        groundNormals[3*i] = 0;
        groundNormals[(3*i) +1] = 1;
        groundNormals[(3*i) +2] = 0;
    }

    groundElements1 = new GLuint[81*3];
    groundElements2 = new GLuint[81*3];

    int index = 0;
    for(int i = 0 ; i < 100 ; i++)
    {
        if(i % 10 != 9)
        {
            groundElements1[3*index]        =   i;
            groundElements1[(3*index) + 1]  =   i + 1;
            groundElements1[(3*index) + 2]  =   i + 10;

            groundElements2[3*index]        =   i;
            groundElements2[(3*index) + 1]  =   i + 1;
            groundElements2[(3*index) + 2]  =   i + 11;

            index++;
        }
    }
}

groundtexture::~groundtexture()
{
    delete groundVertices;
    delete groundNormals;
    delete groundElements1;
    delete groundElements2;
}

void groundtexture::draw1(unsigned int* buffer, unsigned int shader, float cameraX, float cameraY, float cameraZ, float cameraAzimuth, float cameraAltitude)
{
    // use box' rotation for modelViewMatrix
    GLfloat modelViewMatrix[16] = {
        1, 0, 0, 0.0,
        0, 1, 0, 0.0,
        0, 0, 1, 0.0,
        0, 0, 0, 1.0 };

    glUniformMatrix4fv(
            glGetUniformLocation(shader, "modelViewMatrix"),
            1, false, modelViewMatrix);

    // Load vertex data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);

    // Load normal data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundNormals), groundNormals, GL_STATIC_DRAW);

    // Load index data into buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(groundElements1), groundElements1, GL_STATIC_DRAW);

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
    glDrawElements(GL_TRIANGLES, sizeof(groundElements1) / sizeof(GLuint), GL_UNSIGNED_INT, (void*)0);

    glDisableVertexAttribArray(positionAttribute);
}

    void groundtexture::draw2(unsigned int* buffer, unsigned int shader, float cameraX, float cameraY, float cameraZ, float cameraAzimuth, float cameraAltitude)
    {
        // use box' rotation for modelViewMatrix
        GLfloat modelViewMatrix[16] = {
            1, 0, 0, 0.0,
            0, 1, 0, 0.0,
            0, 0, 1, 0.0,
            0, 0, 0, 1.0 };

        glUniformMatrix4fv(
                glGetUniformLocation(shader, "modelViewMatrix"),
                1, false, modelViewMatrix);

        // Load vertex data into buffer
        glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);

        // Load normal data into buffer
        glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(groundNormals), groundNormals, GL_STATIC_DRAW);

        // Load index data into buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[2]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(groundElements2), groundElements2, GL_STATIC_DRAW);

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
        glDrawElements(GL_TRIANGLES, sizeof(groundElements2) / sizeof(GLuint), GL_UNSIGNED_INT, (void*)0);

        glDisableVertexAttribArray(positionAttribute);
    }

void groundtexture::draw3(unsigned int* buffer, unsigned int shader, float cameraX, float cameraY, float cameraZ, float cameraAzimuth, float cameraAltitude)
{

    // use box' rotation for modelViewMatrix
    GLfloat modelViewMatrix[16] = {
        1, 0, 0, 0.0,
        0, 1, 0, 0.0,
        0, 0, 1, 0.0,
        0, 0, 0, 1.0 };

    glUniformMatrix4fv(
            glGetUniformLocation(shader, "modelViewMatrix"),
            1, false, modelViewMatrix);

    // Load vertex data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(testVerty), testVerty, GL_STATIC_DRAW);

    // Load normal data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(testNorms), testNorms, GL_STATIC_DRAW);

    // Load index data into buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(testEls), testEls, GL_STATIC_DRAW);

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
    glDrawElements(GL_TRIANGLES, sizeof(testEls) / sizeof(GLuint), GL_UNSIGNED_INT, (void*)0);

    glDisableVertexAttribArray(positionAttribute);
}
