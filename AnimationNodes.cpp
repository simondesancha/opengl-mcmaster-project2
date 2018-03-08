#include "AnimationNodes.h"
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

/*
 * Handles animation
 *      translationUpdate, rotationUpdate      the animation to apply on each call
 *      parentTransformation     - pointer to parent transformation node
 */
AnimationNode::AnimationNode(glm::vec3 translationUpdate, glm::vec3 rotationUpdate, TransformationNode *parent)
{
    this->parent = parent;
    this->translationUpdate = translationUpdate;
    this->rotationUpdate = rotationUpdate;

    this->lightIndex = LIGHT_OFF;
}

/*
 * Handles animation
 *      translationUpdate, rotationUpdate      the animation to apply on each call
 *      parentTransformation     - pointer to parent transformation node
 *      turnOffAngle        - the angle at which to first switch on/off
 *      lightIndex          - the index for the light to switch on/off
 *      statePointer        - pointer to a boolean representing light on/off
 */
AnimationNode::AnimationNode(glm::vec3 translationUpdate, glm::vec3 rotationUpdate, TransformationNode *parent, int lightIndex, int turnOnOffAngle, bool *statePointer)
{
    this->parent = parent;
    this->translationUpdate = translationUpdate;
    this->rotationUpdate = rotationUpdate;

    this->lightIndex = lightIndex;
    this->turnOnOffAngle = turnOnOffAngle;
    angle = 0;

    this->state = statePointer;
}

/*
 * Update function for the animation
 */
void AnimationNode::update()
{
    parent->updateTransformation(translationUpdate, rotationUpdate);

    if(angle >= 360)
        angle = 0;
    if(angle == turnOnOffAngle + 0 && lightIndex != LIGHT_OFF)
    {
        glEnable(LIGHT_INDEX(lightIndex));
        *state = true;
    }
    else if(angle == (turnOnOffAngle + 180)%360 && lightIndex != LIGHT_OFF)
    {
        glDisable(LIGHT_INDEX(lightIndex));
        *state = false;
    }
    angle += rotationUpdate.x;

    SceneNode::update();
}

/*
 * Handles animation specifically for the doors
 *      axisLocation        - location of the door axis on the map
 */
DoorAnimationNode::DoorAnimationNode(glm::vec3 axisLocation)
{
    this->axisLocation = axisLocation;
    angle = 0;
    desiredAngle = 0;
    ifOpen = false;
}

/*
 * Update function for the door animation
 */
void DoorAnimationNode::update()
{
    if(angle < desiredAngle)
    {
        angle++;
    }
    else if(angle > desiredAngle)
    {
        angle--;
    }

    glm::vec3 translation = -axisLocation;

    glTranslatef(axisLocation.x, axisLocation.y, axisLocation.z);
    glRotatef(angle, 0, 1.0f, 0);
    glTranslatef(translation.x, translation.y, translation.z);

    glPushMatrix();


    SceneNode::update();
}

/*
 * CheckClick function for the door animation
 */
void DoorAnimationNode::checkClick(ObjectPickerClass *picker, transformationPropertiesStruct transformation)
{
    transformationPropertiesStruct temp = transformation;

    glm::vec3 translation = -axisLocation;

    temp.matrix = temp.matrix*glm::translate(glm::mat4(), axisLocation);
    temp.matrix = temp.matrix*glm::rotate(glm::mat4(), (float)angle, glm::vec3(0.0f,1.0f,0.0f));
    temp.matrix = temp.matrix*glm::translate(glm::mat4(), translation);

    SceneNode::checkClick(picker, temp);
}

//Open or close this door
void DoorAnimationNode::openClose()
{
    ifOpen = !ifOpen;

    if(ifOpen)
    {
        desiredAngle = -90;
    }
    else
    {
        desiredAngle = 0;
    }
}

bool DoorAnimationNode::isOpen()
{
    return this->ifOpen;
}
