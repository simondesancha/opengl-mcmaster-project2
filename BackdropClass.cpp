#include "BackdropClass.h"
#include <vector>
#include "lodepng.h"
#include <stdio.h>

/*
 *  BackdropClass handle the backdrop
 *      filename        - picture to load as background (as PNG image)
 */
BackdropClass::BackdropClass(char *filename)
{
    readTexture(filename);
}

// Draw the backdrop
void BackdropClass::draw()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glOrtho(0, 1, 0, 1, 0, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDepthMask(false);

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, texName);

    glBegin(GL_QUADS);
        glTexCoord2f(1, 1);
        glVertex2f(0, 0);

        glTexCoord2f(0, 1);
        glVertex2f(1, 0);

        glTexCoord2f(0, 0);
        glVertex2f(1, 1);

        glTexCoord2f(1, 0);
        glVertex2f(0, 1);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glDepthMask(true);

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
}

//Read in the image as a texture
void BackdropClass::readTexture(char *filename)
{
    //glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &texName);

    glBindTexture(GL_TEXTURE_2D, texName);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST);


   // For .PNG:
    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, filename);

    if(error)
    {
        printf("PNG error: %s\n", lodepng_error_text(error));
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width,
                 height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 &image[0]);
}
