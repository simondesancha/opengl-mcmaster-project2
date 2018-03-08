#define GLM_FORCE_RADIANS
#include <glm/common.hpp>
#include "SceneGraph.h"
#include "ObjectNodes.h"
#include <GL/gl.h>
#include "propertyStructures.h"
#include "ObjLoaderClass.h"
#include "AnimationNodes.h"
#include "BackdropClass.h"
#include <vector>
#include <list>

class DrawingClass
{
public:
    DrawingClass();
    void addShape(ShapeTypeEnum shapeType, glm::vec3 position, float size);
    int addLight(glm::vec3 position, LightPropertiesStruct lightProperties);
    void loadObjMesh(char *file, char *MTLfile);
    void loadObjMesh(char *file, char *MTLfile, glm::vec3 position, glm::vec3 rotation);
    void addOrbit(char *file, char *MTLfile, glm::vec3 position, glm::vec3 rotation, glm::vec3 rotationAnimation, LightPropertiesStruct lightProps, int turnOnOffAngle, bool setAsSun);
    void addDoor(char *file, char *MTLfile);
    void addDoor(char *file, char *MTLfile, glm::vec3 position, glm::vec3 rotation);
    void openCloseDoor(int doorID);

    void processClick(glm::vec3 origin, int X, int Y);
    void transformSelected(glm::vec3 translation, glm::vec3 rotation, float scale);
    void draw();

    bool getIfDayTime();

    std::vector<DoorAnimationNode*> *getDoorList();

    void addSwitch(char *file, char *MTLfile, int lightIndex);
private:
    SceneNode *node;

    std::vector<ObjectNode*> objectList;
    std::list<int> selectedList;

    std::vector<bool> switchList;
    std::vector<int> switchLightList;

    int lightCounter;
    std::vector<DoorAnimationNode*> doorList;
    bool ifSunUp, ifMoonUp;

    void toggleSwitch(int ID);
};
