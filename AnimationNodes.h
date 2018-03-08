#ifndef ANIMATIONNODES_H
#define ANIMATIONNODES_H

#include "SceneGraph.h"

class AnimationNode: public SceneNode
{
public:
    AnimationNode(glm::vec3 translationUpdate, glm::vec3 rotationUpdate, TransformationNode *parent);
    AnimationNode(glm::vec3 translationUpdate, glm::vec3 rotationUpdate, TransformationNode *parent, int lightIndex, int turnOnOffAngle, bool *statePointer);
    void update();
private:
#define LIGHT_OFF -1

    TransformationNode *parent;
    glm::vec3 translationUpdate, rotationUpdate;

    int lightIndex;
    int angle;
    int turnOnOffAngle;
    bool *state;
};

class DoorAnimationNode: public SceneNode
{
public:
    DoorAnimationNode(glm::vec3 axisLocation);
    void update();
    void checkClick(ObjectPickerClass *picker, transformationPropertiesStruct transformation);
    void applyTransformations();

    void openClose();
    bool isOpen();
private:
    glm::vec3 axisLocation;
    int angle, desiredAngle;

    bool ifOpen;
};

#endif // ANIMATIONNODES_H
