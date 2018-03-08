#include <GL/gl.h>
#include <vector>
#include <string>
#include <map>
#define GLM_FORCE_RADIANS
#include <glm/common.hpp>
#include "SceneGraph.h"

class ObjLoaderClass
{
public:
    GLuint getID();
    void draw();
    ObjLoaderClass(char *file, char *MTLfile, int givenID);

    typedef struct {
        float ambience[4];
        float diffuse[4];
        float specular[4];

        char texture[32];
        GLuint textureID;
    } materialStruct;

    typedef struct {
        glm::vec3 vertex;
        glm::vec2 uv;
        glm::vec3 normal;

        int materialIndex;
    } Vertex;

    void checkClick(ObjectPickerClass *picker, transformationPropertiesStruct transformation);

private:
#define ID_IGNORE_CLICKS -1

    GLuint ID;
    GLuint testName;

    std::vector<Vertex> vertexMap;

    std::vector< glm::vec3 > out_vertices;
    std::vector< glm::vec2 > out_uvs;
    std::vector< glm::vec3 > out_normals;
    std::vector< int > out_materialIndex;
    std::map<std::string, int> materialsIndex;
    std::vector<materialStruct> materials;
    materialStruct testMat;
    int materialCounter;

    bool read(char *file, char *MTLfile);
    bool readMTL(char *file);
    void setMaterial(materialStruct materialProperties);
    void readTexture(char *textureFile, materialStruct *materialProps);

    typedef struct {
        float x;
        float y;
        float z;
    } Vector;

    void addMaterial(std::string name, materialStruct *materialProperties);

    Vector data[50];
    glm::vec3 getNormal(glm::vec3 vector1, glm::vec3 vector2, glm::vec3 vector3);
};

class MeshObjectNode: public SceneNode
{
public:
    MeshObjectNode(char *file, char *MTLfile, int givenID);
    void update();
    void checkClick(ObjectPickerClass *picker, transformationPropertiesStruct transformation);
    void doClick(glm::vec3 origin, glm::vec3 dir);

private:
    ObjLoaderClass *loader;
};
