#include "DrawingClass.h"
#include "propertyStructures.h"
#include "AnimationNodes.h"
#include <algorithm>
#include <GL/gl.h>

/*
 * DrawingClass handles the scene-graph and the drawing
 */
DrawingClass::DrawingClass()
{
    this->node = new SceneNode();
    glEnable(GL_LIGHTING);
    float temp[4] = {0,0,0,1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, temp);
    glShadeModel(GL_SMOOTH);
    lightCounter = 0;
}

/*
 * Left-over from Assignment 3 - not used
 */
void DrawingClass::addShape(ShapeTypeEnum shapeType, glm::vec3 position, float size)
{
    TransformationNode *tran = new TransformationNode(position, glm::vec3(0, 0, 0));
    ObjectNode *obj = new ObjectNode(shapeType, size, this->objectList.size(), tran);
    BoundaryNode *boundary = new BoundaryNode(shapeType, glm::vec3(0,0,0), size, this->objectList.size());

    obj->addChild(boundary);
    objectList.push_back(obj);

    tran->addChild(obj);
    node->addChild(tran);
}

/*
 * Adds a light to the scene graph
 *      position    - position of light
 *      lightProperties         - the properties of the light
 */
int DrawingClass::addLight(glm::vec3 position, LightPropertiesStruct lightProperties)
{
    TransformationNode *tran = new TransformationNode(position, glm::vec3(0, 0, 0));
    LightNode *light = new LightNode(lightProperties, lightCounter);
    lightCounter++;

    MaterialPropertiesStruct properties;
    properties.ambience[0] = glm::vec4(0.5, 0.1, 0.5, 1.0);
    properties.ambience[1] = glm::vec4(0.1, 0.5, 0.5, 1.0);
    properties.emission = glm::vec4(0.5, 0.5, 0.5, 1.0);

    ObjectNode *lamp = new ObjectNode(Sphere, 0, this->objectList.size(), tran, properties);
    BoundaryNode *boundary = new BoundaryNode(Sphere, glm::vec3(0,0,0), 2, this->objectList.size());
    lamp->addChild(boundary);
    objectList.push_back(lamp);

    light->addChild(lamp);
    AnimationNode *anim = new AnimationNode(glm::vec3(0,0,0), glm::vec3(0,5,0), tran);
    anim->addChild(light);
    tran->addChild(anim);

    node->addChild(tran);

    return light->getLightIndex();
}

/*
 * Reads an object from a polygon mesh and adds it to the scene graph
 *      file    - location of .obj file
 *      MTLfile - location of .mtl material file
 */
void DrawingClass::loadObjMesh(char *file, char *MTLfile)
{
    MeshObjectNode *obj = new MeshObjectNode(file, MTLfile, ID_IGNORE_CLICKS);
    node->addChild(obj);
}

/*
 * Reads an object from a polygon mesh and adds it to the scene graph
 *      file    - location of .obj file
 *      MTLfile - location of .mtl material file
 *      position, rotation      - transformations to apply to the object
 */
void DrawingClass::loadObjMesh(char *file, char *MTLfile, glm::vec3 position, glm::vec3 rotation)
{
    MeshObjectNode *obj = new MeshObjectNode(file, MTLfile, ID_IGNORE_CLICKS);

    TransformationNode *tran = new TransformationNode(position, rotation);

    tran->addChild(obj);
    node->addChild(tran);
}

/*
 * Adds a planet in an orbit animation
 *      file    - location of .obj file
 *      MTLfile - location of .mtl material file
 *      position, rotation      - transformations to apply to the object
 *      rotationAnimation       - rotation per animation cycle
 *      LightProperties         - properties of the light emmited
 *      turnOffAngle            - the angle of orbit at which to toggle the light
 *      setAsSun                - if this is the sun
 */
void DrawingClass::addOrbit(char *file, char *MTLfile, glm::vec3 position, glm::vec3 rotation,
                            glm::vec3 rotationAnimation, LightPropertiesStruct lightProperties,
                            int turnOnOffAngle, bool setAsSun)
{
    MeshObjectNode *obj = new MeshObjectNode(file, MTLfile, ID_IGNORE_CLICKS);

    TransformationNode *rotate = new TransformationNode(glm::vec3(0,0,0), rotation);
    TransformationNode *tran = new TransformationNode(position, glm::vec3(0,0,0));

    bool *statePointer;
    if(setAsSun)
        statePointer = &ifSunUp;
    else

        statePointer = &ifMoonUp;

    AnimationNode *animate = new AnimationNode(glm::vec3(0,0,0), rotationAnimation, rotate, lightCounter, turnOnOffAngle, statePointer);

    LightNode *light = new LightNode(lightProperties, lightCounter);
    lightCounter++;

    obj->addChild(light);
    animate->addChild(obj);
    tran->addChild(animate);
    rotate->addChild(tran);
    node->addChild(rotate);
}

/*
 * Adds a light switch
 *      file    - location of .obj file
 *      MTLfile - location of .mtl material file
 *      lightIndex  - index of light to toggle
 */
void DrawingClass::addSwitch(char *file, char *MTLfile, int lightIndex)
{
    MeshObjectNode *obj = new MeshObjectNode(file, MTLfile, switchList.size() | SWITCH_BIT_MASK);
    switchList.push_back(false);
    switchLightList.push_back(lightIndex);

    node->addChild(obj);
}

/*
 * Adds a door to the scene graph
 *      file    - location of .obj file
 *      MTLfile - location of .mtl material file
 */
void DrawingClass::addDoor(char *file, char *MTLfile)
{
    MeshObjectNode *obj = new MeshObjectNode(file, MTLfile, doorList.size() | DOOR_BIT_MASK);

    DoorAnimationNode *animate = new DoorAnimationNode(glm::vec3(-224.042007f, 0.0f, 387.870941f));

    doorList.push_back(animate);

    animate->addChild(obj);
    node->addChild(animate);
}

/*
 * Adds a door to the scene graph
 *      file    - location of .obj file
 *      MTLfile - location of .mtl material file
 *      position, rotation      - transformations to apply to the object
 */
void DrawingClass::addDoor(char *file, char *MTLfile, glm::vec3 position, glm::vec3 rotation)
{
    MeshObjectNode *obj = new MeshObjectNode(file, MTLfile, doorList.size() | DOOR_BIT_MASK);

    DoorAnimationNode *animate = new DoorAnimationNode(glm::vec3(-224.042007f, 0.0f, 387.870941f));

    TransformationNode *tran = new TransformationNode(position, rotation);

    doorList.push_back(animate);

    animate->addChild(obj);
    tran->addChild(animate);
    node->addChild(tran);

}

//Toggle the door at doorID
void DrawingClass::openCloseDoor(int doorID)
{
    doorList[doorID]->openClose();
}

//Toggle the switch at ID
void DrawingClass::toggleSwitch(int ID)
{
    switchList[ID] = !switchList[ID];
    if(switchList[ID])
    {
        glEnable(switchLightList[ID]);
    }
    else
    {
        glDisable(switchLightList[ID]);
    }
}

/*
 * Process a right-click
 *      origin      - the player position
 *      X, Y        - the window coordinates of the click
 */
void DrawingClass::processClick(glm::vec3 origin, int X, int Y)
{
    ObjectPickerClass *objPicker = new ObjectPickerClass(origin, X, Y);

    transformationPropertiesStruct temp;
    temp.matrix = glm::mat4();

    node->checkClick(objPicker, temp);

    int ID = objPicker->returnID();
    if(ID == -1)
        return;

    if(ID & DOOR_BIT_MASK)
    {
        ID &= ~DOOR_BIT_MASK;
        openCloseDoor(ID);
    }
    else if(ID & SWITCH_BIT_MASK)
    {
        ID &= ~SWITCH_BIT_MASK;
        toggleSwitch(ID);
    }
    else
    {
        objectList[ID]->toggleSelect();

        if(std::find(selectedList.begin(), selectedList.end(), ID) != selectedList.end())
        {
            selectedList.remove(ID);
        }
        else
        {
            selectedList.push_back(ID);
        }
    }
}

//Leftover from Assignment 3 - unusedd
void DrawingClass::transformSelected(glm::vec3 translation, glm::vec3 rotation, float scale)
{
    std::list<int>::iterator it = selectedList.begin();
    for(; it != selectedList.end(); it++)
    {
        if(scale != 1)
        {
            objectList[*it]->scale(scale);
        }
        else
        {
            objectList[*it]->transform(translation, rotation);
        }
    }
}

//Draw the scene graph!
void DrawingClass::draw()
{
    node->update();
}

//Returns if we are in daytime
bool DrawingClass::getIfDayTime()
{
    return this->ifSunUp;
}

//Return a list pointing to the doors
std::vector<DoorAnimationNode *> *DrawingClass::getDoorList()
{
    return &doorList;
}
