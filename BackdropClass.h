#ifndef BACKDROPCLASS_H
#define BACKDROPCLASS_H

#include <GL/gl.h>
#include "propertyStructures.h"

class BackdropClass
{
public:
    BackdropClass(char *filename);
    void draw();

private:
    void readTexture(char *filename);
    GLuint texName;
};

#endif // BACKDROPCLASS_H
