/*
 * Misc property structures and definitions
 *
 */

#ifndef PROPERTYSTRUCTURES_H
#define PROPERTYSTRUCTURES_H

#define GLM_FORCE_RADIANS
#include <GL/gl.h>
#include <glm/glm.hpp>

#define LIGHT_INDEX(index) GL_LIGHT0 + index

#define DOOR_BIT_MASK 256
#define SWITCH_BIT_MASK 128

enum ShapeTypeEnum {
    Cube,
    Cylinder,
    Sphere,
    Teapot
};

enum TransformationType {
    Rotation,
    Translation
};

#define NUM_TEXTURES 8
typedef struct {
    GLuint textureNames[NUM_TEXTURES];
    int textureCounter;
} textureStruct;

typedef struct {
    glm::mat4 matrix;
} transformationPropertiesStruct;

typedef struct { //Assuming all material properties are the same
    glm::vec4 ambience[2];  //[2] for unselected-selected

    glm::vec4 emission; //Unused if point-source
} MaterialPropertiesStruct;

enum LightTypeEnum {
    PointLight,
    SpotLight
};

typedef struct {
    glm::vec4 ambience;
    glm::vec4 specular;
    glm::vec4 diffuse;

    glm::vec4 emission; //Unused if point-source

    LightTypeEnum lightType;
    float cutoffAngle;
    glm::vec3 direction;
} LightPropertiesStruct;

typedef struct {
 //   glm::vec3 position;
    float size;
  //  glm::vec3 angle;
    glm::mat4 transformation;
    bool selected;

    int ID;

    glm::vec3 scale;
} SingleBoundaryBoxStruct;

#endif // PROPERTYSTRUCTURES_H
