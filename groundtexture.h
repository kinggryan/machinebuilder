#ifndef GROUNDTEXTURE_H
#define GROUNDTEXTURE_H

#include "glheader.h"
#include <OpenGL/gl3.h>

class groundtexture
{
public:
    groundtexture();
    ~groundtexture();
    void draw1(unsigned int* buffer, unsigned int shader, float cameraX, float cameraY, float cameraZ, float cameraAzimuth, float cameraAltitude);
    void draw2(unsigned int* buffer, unsigned int shader, float cameraX, float cameraY, float cameraZ, float cameraAzimuth, float cameraAltitude);
    void draw3(unsigned int* buffer, unsigned int shader, float cameraX, float cameraY, float cameraZ, float cameraAzimuth, float cameraAltitude);


    GLfloat* groundVertices;
    GLuint* groundElements1;
    GLuint* groundElements2;
    GLfloat* groundNormals;
};

#endif // GROUNDTEXTURE_H
