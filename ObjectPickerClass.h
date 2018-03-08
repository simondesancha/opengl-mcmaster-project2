#ifndef OBJPICKER_H
#define OBJPICKER_H

#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/common.hpp>
#include "propertyStructures.h"

class ObjectPickerClass
{
public:
    ObjectPickerClass(glm::vec3 origin, int winX, int winY);
    void checkObject(SingleBoundaryBoxStruct boundaryBox);
    void checkMesh(glm::vec3 vertex0, glm::vec3 vertex1, glm::vec3 vertex2, int ID);
    int returnID();

    glm::vec3 GetOGLPos(int winX, int winY);
    float rayIntersectsTriangle(glm::vec3 origin, glm::vec3 direction, glm::vec3 vertex0, glm::vec3 vertex1, glm::vec3 vertex2);

    void drawBox(float size);
private:
#define NO_INTERSECTION -1

#define BOUNDARY_BOX(index, incrementOne, incrementTwo) box[index], \
        box[index+incrementOne], \
        box[index+incrementTwo]
/*#define CUBE_COORDINATES(index) box[index].x + position.x, \
        box[index].y + position.y, \
        box[index].z + position.z*/
#define CUBE_COORDINATES(index) box[index].x, \
        box[index].y, \
        box[index].z
#define CUBE_COORDINATES_ROTATE(vector) vector.x + position.x, \
        vector.y + position.y, \
        vector.z + position.z

    glm::vec3 defaultBoundaryBox[8] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
    };

    glm::vec3 rotVector[3] = {glm::vec3(1.0f, 0.0f, 0.0f),
                                glm::vec3(0.0f, 1.0f, 0.0f),
                                glm::vec3(0.0f, 0.0f, 1.0f)};

    float closestIntersection;
    int closestIndex;
    glm::vec3 origin, direction;

    float testBoundaryBox(glm::vec3 origin, glm::vec3 direction, glm::mat4 transformation,
                          float size,  glm::vec3 directionalScale);
    void rotateScaleDefaultBox(glm::vec3 *box, glm::mat4 transformation, float scale, glm::vec3 directionalScale);
};


#endif
