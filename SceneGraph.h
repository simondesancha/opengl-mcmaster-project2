#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/common.hpp>
#include <GL/glu.h>
#include "ObjectPickerClass.h"
#include "propertyStructures.h"

/*
 * SceneNode is the root node of a scene graph
 *      update()            - draw/apply the node and its children
 *      checkClick(..)      - check the node and its children for a click
 *      scale(..)           - scale the node
 *      addChild(..)        - add a node as a child
 */
class SceneNode
{
public:
    virtual void update();
    virtual void checkClick(ObjectPickerClass *picker, transformationPropertiesStruct transformation);
    virtual void scale(float scaleFactor);
    void addChild(SceneNode *child);

private:
    std::vector<SceneNode*> childList;
};

/*
 * TransformationNode represents a transformation
 *      translation, rotation       - transformation
 */
class TransformationNode: public SceneNode
{
public:
    TransformationNode(glm::vec3 translation, glm::vec3 rotation);
    void update();
    void updateTransformation(glm::vec3 translation, glm::vec3 rotation);
    void checkClick(ObjectPickerClass *picker, transformationPropertiesStruct transformation);

private:
    glm::vec3 rotVectorX[3] = {glm::vec3(1.0f, 0.0f, 0.0f),
                              glm::vec3(0.0f, 1.0f, 0.0f),
                              glm::vec3(0.0f, 0.0f, 1.0f)};
    glm::vec3 translation, rotation;
    float size;
};

/*
 * LightNode represents a light
 */
class LightNode: public SceneNode
{
public:
    LightNode(LightPropertiesStruct lightProperties, int givenLightIndex);
    void update();
    int getLightIndex();

private:
    LightPropertiesStruct lightProperties;
    int lightIndex;
};

/*
 * Unused in this project
 */
class BoundaryNode: public SceneNode
{
public:
    BoundaryNode(ShapeTypeEnum shapeType, glm::vec3 offset, float size, int ID);
    BoundaryNode(glm::vec3 directionalScales, glm::vec3 offset, float size, int ID);
    BoundaryNode(glm::vec3 *bounds);
    void checkClick(ObjectPickerClass *picker, transformationPropertiesStruct transformation);
    void scale(float scaleFactor);
    void update();

private:
    glm::vec3 offset;
    SingleBoundaryBoxStruct boundaryBox;

    bool ifDoingByBounds;
    glm::vec3 bounds[8];
};



#endif
