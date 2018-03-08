#include "SceneGraph.h"
#include <GL/gl.h>
#define GLM_FORCE_RADIANS
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glut.h>

/*
 * SceneNode is the root node of a scene graph
 */
void SceneNode::update()
{
    for(unsigned int i = 0; i < childList.size(); i++)
    {
        childList[i]->update();

        glPopMatrix();
        glPushMatrix();
    }
}

/*
 * Generic checkclick function
 */
void SceneNode::checkClick(ObjectPickerClass *picker, transformationPropertiesStruct transformation)
{
    for(unsigned int i = 0; i < childList.size(); i++)
        childList[i]->checkClick(picker, transformation);
}

/*
 * Generic scale function
 */
void SceneNode::scale(float scaleFactor)
{
    for(unsigned int i = 0; i < childList.size(); i++)
        childList[i]->scale(scaleFactor);
}

/*
 * Generic addChild function
 */
void SceneNode::addChild(SceneNode *child)
{
    childList.push_back(child);
}

/*
 * TransformationNode represents a transformation
 *      translation, rotation       - transformation
 */
TransformationNode::TransformationNode(glm::vec3 translation, glm::vec3 rotation)
{
    this->translation = translation;
    this->rotation = rotation;
}

//Update function - apply transformation
void TransformationNode::update()
{
    glTranslatef(translation.x, translation.y, translation.z);

    glRotatef(rotation.x, rotVectorX[0].x, rotVectorX[0].y, rotVectorX[0].z);
    glRotatef(rotation.y, rotVectorX[1].x, rotVectorX[1].y, rotVectorX[1].z);
    glRotatef(rotation.z, rotVectorX[2].x, rotVectorX[2].y, rotVectorX[2].z);

    glPushMatrix();

    SceneNode::update();
}

//Edit the tranfomation
void TransformationNode::updateTransformation(glm::vec3 translation, glm::vec3 rotation)
{
    this->translation += translation;
    this->rotation += rotation;
}

/*
 * Implementation of checkClick   - apply the transformations it would apply if drawing
 */
void TransformationNode::checkClick(ObjectPickerClass *picker, transformationPropertiesStruct transformation)
{
    transformationPropertiesStruct temp = transformation;

    temp.matrix = temp.matrix*glm::translate(glm::mat4(), translation);
    temp.matrix = temp.matrix*glm::rotate(glm::mat4(), rotation.x, rotVectorX[0]);
    temp.matrix = temp.matrix*glm::rotate(glm::mat4(), rotation.y, rotVectorX[1]);
    temp.matrix = temp.matrix*glm::rotate(glm::mat4(), rotation.z, rotVectorX[2]);

    SceneNode::checkClick(picker, temp);
}

/*
 * LightNode represents a light
 */
LightNode::LightNode(LightPropertiesStruct lightProperties, int givenLightIndex)
{
    this->lightIndex = givenLightIndex;
    this->lightProperties = lightProperties;

    int index = LIGHT_INDEX(lightIndex);
    glEnable(index); //NOTE: ensure compatability with this line

    glLightfv(index, GL_AMBIENT, &lightProperties.ambience.x);
    glLightfv(index, GL_DIFFUSE, &lightProperties.diffuse.x);
    glLightfv(index, GL_SPECULAR, &lightProperties.specular.x);

    if(lightProperties.lightType == SpotLight)
    {
        glLightf(index, GL_SPOT_CUTOFF, lightProperties.cutoffAngle);
        glLightfv(index, GL_SPOT_DIRECTION, &lightProperties.direction.x);
    }
}

void LightNode::update()
{
    glm::vec4 position =  glm::vec4(0,0,0,1);
    glLightfv(LIGHT_INDEX(lightIndex), GL_SPOT_DIRECTION, &lightProperties.direction.x);

    glLightfv(LIGHT_INDEX(lightIndex), GL_POSITION, &position.x);

    SceneNode::update();
}

int LightNode::getLightIndex()
{
    return LIGHT_INDEX(lightIndex);
}

/*
 * Unused in this project
 */
BoundaryNode::BoundaryNode(ShapeTypeEnum shapeType, glm::vec3 offset, float size, int ID)
{
    ifDoingByBounds = false;
    this->offset = offset;
    boundaryBox.ID = ID;
    boundaryBox.size = size;
    boundaryBox.selected = false;
    boundaryBox.scale = glm::vec3(1,1,1);
}

BoundaryNode::BoundaryNode(glm::vec3 directionalScales, glm::vec3 offset, float size, int ID)
{
    ifDoingByBounds = false;
    this->offset = offset;
    boundaryBox.ID = ID;
    boundaryBox.size = size;
    boundaryBox.selected = false;
    boundaryBox.scale = directionalScales;
}

BoundaryNode::BoundaryNode(glm::vec3 *bounds)
{
    this->offset = glm::vec3(1,1,1);
    boundaryBox.ID = 0;
    boundaryBox.size = 10;
    boundaryBox.selected = false;
    boundaryBox.scale = glm::vec3(1,1,1);

    ifDoingByBounds = true;
    memcpy(&this->bounds[0], bounds, 8*sizeof(glm::vec3));
}

void BoundaryNode::checkClick(ObjectPickerClass *picker, transformationPropertiesStruct transformation)
{
    //boundaryBox.angle = transformation.rotation;
    //boundaryBox.position = transformation.transformation + offset;
    boundaryBox.transformation = transformation.matrix;
    picker->checkObject(boundaryBox);
}

void BoundaryNode::scale(float scaleFactor)
{
    boundaryBox.size = boundaryBox.size * scaleFactor;

    SceneNode::scale(scaleFactor);
}

void BoundaryNode::update()
{
    //ObjectPickerClass *tem = new ObjectPickerClass(glm::vec3(0,0,0), 0, 0);
    //tem->drawBox(this->boundaryBox.size);
    SceneNode::update();
}
