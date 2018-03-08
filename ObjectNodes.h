#ifndef OBJECTNODES_H
#define OBJECTNODES_H

#include "SceneGraph.h"

class ObjectNode: public SceneNode
{
public:
    ObjectNode(ShapeTypeEnum shapeType, float size, int ID, TransformationNode *parentTransformation);
    ObjectNode(ShapeTypeEnum shapeType, float size, int ID, TransformationNode *parentTransformation, MaterialPropertiesStruct materialProperties);
    void update();
    void toggleSelect();

    void transform(glm::vec3 translation, glm::vec3 rotation);
    void scale(float scaleFactor);

private:
    bool ifSelected = false;
    ShapeTypeEnum shapeType;
    float size;

    MaterialPropertiesStruct materialProps;
    TransformationNode *parentTransformation;
    GLUquadricObj *quadricObj;
};


#endif // OBJECTNODES_H
