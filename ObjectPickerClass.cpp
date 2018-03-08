#include "ObjectPickerClass.h"
#include <GL/gl.h>
#include <GL/glu.h>
#define GLM_FORCE_RADIANS
#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <string.h>

/*
 * ObjectPickerClass handles the right-click - a class is created each time a click occurs
 *      origin      - player position
 *      winX, winY  - window coordinates of click
 */
ObjectPickerClass::ObjectPickerClass(glm::vec3 origin, int winX, int winY)
{
    this->origin = origin;
    this->direction = GetOGLPos(winX, winY);

    closestIntersection = NO_INTERSECTION;
}

/*
 * Check the boundarybox - leftover from Assignment 3 and is unused
 */
void ObjectPickerClass::checkObject(SingleBoundaryBoxStruct boundaryBox)
{
    float tempIntersection = testBoundaryBox(origin, direction, boundaryBox.transformation,
                                                boundaryBox.size, boundaryBox.scale);
    if(tempIntersection != NO_INTERSECTION)
    {
        if(tempIntersection < closestIntersection || closestIntersection == NO_INTERSECTION)
        {
            closestIntersection = tempIntersection;
            closestIndex = boundaryBox.ID;
        }
    }
}

/*
 * Check a triangle from an polygon mesh
 *      vertex0-2       - triangle to check
 *      ID              - object ID
 */
void ObjectPickerClass::checkMesh(glm::vec3 vertex0, glm::vec3 vertex1, glm::vec3 vertex2, int ID)
{
    float tempIntersection = rayIntersectsTriangle(origin, direction, vertex0, vertex1, vertex2);
    if(tempIntersection != NO_INTERSECTION)
    {
        if(tempIntersection < closestIntersection || closestIntersection == NO_INTERSECTION)
        {
            closestIntersection = tempIntersection;
            closestIndex = ID;
        }
    }
}

//Return the result - ID of selcted object, NO_INTERSECTION if none
int ObjectPickerClass::returnID()
{
    if(closestIntersection != NO_INTERSECTION)
        return closestIndex;
    return NO_INTERSECTION;
}

//Returns a normalized vector in direction of click
glm::vec3 ObjectPickerClass::GetOGLPos(int windowX, int windowY)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY;
    GLdouble posX, posY, posZ;
    GLdouble farX, farY, farZ;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    winX = (float)windowX;
    winY = (float)viewport[3] - (float)windowY;

    gluUnProject(winX, winY, 0, modelview, projection, viewport, &posX, &posY, &posZ);
    gluUnProject(winX, winY, 1, modelview, projection, viewport, &farX, &farY, &farZ);

    glm::vec3 direction = glm::vec3(farX - posX, farY - posY, farZ - posZ);

    return glm::normalize(direction);
}

//Leftover from Assignment 3
float ObjectPickerClass::testBoundaryBox(glm::vec3 origin, glm::vec3 direction, glm::mat4 transformation,
                                         float size,  glm::vec3 directionalScale)
{
    float closestIntersect = NO_INTERSECTION;
    float tempIntersect;
    glm::vec3 box[8];
    rotateScaleDefaultBox(&box[0], transformation, size, directionalScale);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    for(int i = 0; i < 8; i++)
    {
        tempIntersect = rayIntersectsTriangle(origin, direction, BOUNDARY_BOX(i%8, 1, 2));
        if(tempIntersect != NO_INTERSECTION)
        {
            if(tempIntersect < closestIntersect || closestIntersect == NO_INTERSECTION)
                closestIntersect = tempIntersect;
        }

        glBegin(GL_TRIANGLES);
            glVertex3fv(&box[i%8].x);
            glVertex3fv(&box[i%8+1].x);
            glVertex3fv(&box[i%8+2].x);
        glEnd();
    }

    for(int i = 0; i < 2; i++)
    {
        tempIntersect = rayIntersectsTriangle(origin, direction, BOUNDARY_BOX(i, 2, 4));
        if(tempIntersect != NO_INTERSECTION)
        {
            if(tempIntersect < closestIntersect || closestIntersect == NO_INTERSECTION)
                closestIntersect = tempIntersect;
        }

        tempIntersect = rayIntersectsTriangle(origin, direction, BOUNDARY_BOX(i, 6, 4));
        if(tempIntersect != NO_INTERSECTION)
        {
            if(tempIntersect < closestIntersect || closestIntersect == NO_INTERSECTION)
                closestIntersect = tempIntersect;
        }

        glBegin(GL_TRIANGLES);
            glVertex3fv(&box[i].x);
            glVertex3fv(&box[i+2].x);
            glVertex3fv(&box[i+4].x);
        glEnd();

        glBegin(GL_TRIANGLES);
            glVertex3fv(&box[i+0].x);
            glVertex3fv(&box[i+6].x);
            glVertex3fv(&box[i+4].x);
        glEnd();

    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    return(closestIntersect);
}

/*
 *  Performs ray-triangle intersection test
 *      origin      - player position
 *      direction   - normalized click direction vector
 *      vertex0-2   - triangle to test
 */
float ObjectPickerClass::rayIntersectsTriangle(glm::vec3 origin, glm::vec3 direction, glm::vec3 vertex0, glm::vec3 vertex1, glm::vec3 vertex2)
{
    float a,f,u,v, t;

    glm::vec3 triangleSurface1 = vertex1 - vertex0;
    glm::vec3 triangleSurface2 = vertex2 - vertex0;

    glm::vec3 crossed1 = glm::cross(direction, triangleSurface2);
    a = glm::dot(triangleSurface1, crossed1);

    if (a > -0.00001 && a < 0.00001)
        return(NO_INTERSECTION);

    f = 1/a;
    glm::vec3 s = origin - vertex0;
    u = f * glm::dot(s, crossed1);

    if (u < 0.0 || u > 1.0)
        return(NO_INTERSECTION);

    glm::vec3 q = glm::cross(s, triangleSurface1);
    v = f * glm::dot(direction, q);

    if (v < 0.0 || u + v > 1.0)
        return(NO_INTERSECTION);

    t = f * glm::dot(triangleSurface2, q);

    return (t > 0.00001) ? t : NO_INTERSECTION;
}

//Left from Assignment 3
void ObjectPickerClass::rotateScaleDefaultBox(glm::vec3 *box, glm::mat4 transformation, float scale, glm::vec3 directionalScale)
{
    memcpy(box, defaultBoundaryBox, 8*sizeof(glm::vec3));
    for(int j = 0; j < 3; j++)
    {
        for(int i = 0; i < 8; i++)
        {
            glm::vec4 temp = glm::vec4(box[i].x, box[i].y, box[i].z, 1.0f);
            box[i] = glm::vec3(transformation*temp);
            //box[i] = glm::rotate(box[i], angle[2-j], rotVector[2-j]);
        }
    }

    for(int i = 0; i < 8; i++)
    {
        box[i] = scale*box[i];
        box[i].x = directionalScale.x * box[i].x;
        box[i].y = directionalScale.y * box[i].y;
        box[i].z = directionalScale.z * box[i].z;
    }
}
