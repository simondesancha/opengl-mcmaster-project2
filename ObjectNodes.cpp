#include <GL/gl.h>
#define GLM_FORCE_RADIANS
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glut.h>
#include "SceneGraph.h"
#include "ObjectNodes.h"

/*
 * Represents an object - leftover from Assignment 3 but is unused in this project
 *  left it in as it may be useful
 */
ObjectNode::ObjectNode(ShapeTypeEnum shapeType, float size, int ID, TransformationNode *parentTransformation)
{
    this->shapeType = shapeType;
    this->size = size;
    this->parentTransformation = parentTransformation;
    ifSelected = false;

    if(shapeType == Cylinder)
        quadricObj = gluNewQuadric();

    this->materialProps.ambience[0] = glm::vec4(0.1, 0.5, 0.5, 1.0);
    this->materialProps.ambience[1] = glm::vec4(0.5, 0.1, 0.5, 1.0);
    this->materialProps.emission = glm::vec4(0,0,0,0);
}

ObjectNode::ObjectNode(ShapeTypeEnum shapeType, float size, int ID, TransformationNode *parentTransformation, MaterialPropertiesStruct materialProperties)
{
    this->shapeType = shapeType;
    this->size = size;
    this->parentTransformation = parentTransformation;
    ifSelected = false;

    if(shapeType == Cylinder)
        quadricObj = gluNewQuadric();

    this->materialProps = materialProperties;
}

void ObjectNode::update()
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
                 ifSelected ? &materialProps.ambience[1].x : &materialProps.ambience[0].x);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &materialProps.emission.x);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 27.0f);

    switch(shapeType)
    {
        case Cylinder:
            glTranslatef(size/2, size/2, 0);
            gluCylinder(quadricObj, size/2, size/2, size, 20, 20);
            break;
        case Cube:
            //In order to place the cube at the bottom left corner:
            glTranslatef(size/2, size/2, size/2);
            glutSolidCube(size);
            break;
        case Sphere:
            //In order to place the sphere at the bottom left corner:
            glTranslatef(size/2, size/2, size/2);
            glutSolidSphere(size/2, 20, 20);
            break;
        case Teapot:
            //In order to place the sphere at the bottom left corner:
            glTranslatef(size/2, size/2, size/2);
            glutSolidTeapot(size);
            break;
    }

    SceneNode::update();
}

void ObjectNode::toggleSelect()
{
    ifSelected = !ifSelected;
}

void ObjectNode::transform(glm::vec3 translation, glm::vec3 rotation)
{
    parentTransformation->updateTransformation(translation, rotation);
}

void ObjectNode::scale(float scaleFactor)
{
    size = size * scaleFactor;

    SceneNode::scale(scaleFactor);
}
