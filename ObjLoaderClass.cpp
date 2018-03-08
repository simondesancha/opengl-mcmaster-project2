#include "ObjLoaderClass.h"
#include "TGALoader.h"
#include "ObjectPickerClass.h"
#include<GL/glut.h>
#include <vector>
#include <fstream>
#include <map>
#include <string>
#include <ctype.h>
#include <string.h>
#include "lodepng.h"
#include <stdio.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

/*
 * ObjLoaderClass handles the object loading from a Wavefront .obj file, and .mtl material file
 *      file        - .obj file
 *      MTLfile     - .mtl file
 *      givenID     - unique object ID
 */
ObjLoaderClass::ObjLoaderClass(char *file, char *MTLfile, int givenID)
{
    this->ID = givenID;
    read(file, MTLfile);
}

/*
 * Check this object for clicks - loop through and check each triangle for click
 *      picker          - ObjectPickerClass instance
 *      transformation  - transformation that would be applied if drawn
 */
void ObjLoaderClass::checkClick(ObjectPickerClass *picker, transformationPropertiesStruct transformation)
{
    if(ID == ID_IGNORE_CLICKS)
        return;

    glm::vec3 vert[3];

    glm::mat4 matrix;
    glm::vec4 temp;

    for(int i = 0; i < vertexMap.size(); i++)
    {
        Vertex tempVertex = vertexMap[i];

        temp = transformation.matrix*glm::vec4(tempVertex.vertex.x, tempVertex.vertex.y, tempVertex.vertex.z, 1.0f);
        vert[i%3] = glm::vec3(temp);

        if(i%3 == 2)
        {
            picker->checkMesh(vert[0], vert[1], vert[2], ID);
        }

    }
}

GLuint ObjLoaderClass::getID()
{
    return ID;
}

//Return the normal to a triangle
glm::vec3 ObjLoaderClass::getNormal(glm::vec3 vector1, glm::vec3 vector2, glm::vec3 vector3)
{
    //Get two vectors along the triangle
    glm::vec3 polyVector1 = vector2 - vector1;
    glm::vec3 polyVector2 = vector3 - vector1;

    //Find the cross product
    glm::vec3 cross = glm::cross(polyVector1, polyVector2);

    //Normalise the cross-product
    cross = glm::normalize(cross);

    //Set the normal vector
    return cross;
}

/*
 * Read the .obj file
 */
bool ObjLoaderClass::read(char *file, char *MTLfile)
{
    if(readMTL(MTLfile) == false)
        return false;

    FILE *fp;

    fp=fopen(file,"r");
    if (!fp)
    {
        printf("can't open file %s\n", file);
        return false;
    }

    char lineHeader[128];
    int res = 0;
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec3 > temp_normals;
    std::vector< glm::vec3 > generated_normals;
    std::vector< int > temp_materialIndexs;
    int lastMaterialIndex = 0;

    int vertexIndexOffset = 0;
    int normalIndexOffset = 0;

    while(1)
    {
        if(fgets(lineHeader, 128, fp) == NULL)
            break;

        if(strncmp(lineHeader, "o ", 2) == 0)
        {
            vertexIndexOffset = temp_vertices.size();
            normalIndexOffset = temp_normals.size();
        }
        else if(strncmp(lineHeader, "v ", 2) == 0)
        {
            glm::vec3 vertex;
            Vector vec;
            sscanf(lineHeader, "%*s %f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
        }
        else if(strncmp(lineHeader, "vt", 2) == 0)
        {
            glm::vec2 uv;
            sscanf(lineHeader, "%*s %f %f\n", &uv.x, &uv.y );
            temp_uvs.push_back(uv);
        }
        else if(strncmp( lineHeader, "vn", 2 ) == 0)
        {
            glm::vec3 normal;
            sscanf(lineHeader, "%*s %f %f %f\n", &normal.x, &normal.y, &normal.z );
            temp_normals.push_back(normal);
        }
        else if(strncmp(lineHeader, "f", 1) == 0)
        {
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = sscanf(lineHeader, "%*s %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            if (matches != 9)
            {
                //Check for other format:
                if(sscanf(lineHeader, "%*s %d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2] ) != 6)
                {
                    printf("OBJ error, number args: %d\n", matches);
                    return false;
                }
                uvIndex[0] = 0;
                uvIndex[1] = 0;
                uvIndex[2] = 0;
            }
            vertexIndices.push_back(vertexIndex[0]+vertexIndexOffset);
            vertexIndices.push_back(vertexIndex[1]+vertexIndexOffset);
            vertexIndices.push_back(vertexIndex[2]+vertexIndexOffset);
            uvIndices    .push_back(uvIndex[0]);
            uvIndices    .push_back(uvIndex[1]);
            uvIndices    .push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]+normalIndexOffset);
            normalIndices.push_back(normalIndex[1]+normalIndexOffset);
            normalIndices.push_back(normalIndex[2]+normalIndexOffset);
            temp_materialIndexs.push_back(lastMaterialIndex);
            temp_materialIndexs.push_back(lastMaterialIndex);
            temp_materialIndexs.push_back(lastMaterialIndex);
        }
        else if(strncmp(lineHeader, "usemtl", 6) == 0)
        {
            char tempName[30];
            std::string currentMaterialName;
            sscanf(lineHeader, "%*s %s\n", &tempName);
            currentMaterialName = tempName;

            lastMaterialIndex = materialsIndex[currentMaterialName];
        }
    }

    fclose(fp);
    Vertex tempVertex;
    for(unsigned int i = 0; i<vertexIndices.size(); i++)
    {
        tempVertex.vertex = temp_vertices[vertexIndices[i]-1];
        tempVertex.normal = temp_normals[normalIndices[i]-1];

        if(temp_uvs.size() > 0)
            tempVertex.uv = temp_uvs[uvIndices[i]-1]; //To safeguard missing uvs

        tempVertex.materialIndex = temp_materialIndexs[i];

        vertexMap.push_back(tempVertex);
    }
    return true;
}

//Draw the object
void ObjLoaderClass::draw()
{
       glEnable(GL_TEXTURE_2D);
       glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

       for(int i = 0; i < vertexMap.size(); i++)
       {
           Vertex tempVertex = vertexMap[i];
           setMaterial(materials[tempVertex.materialIndex]);

           if(i % 3 == 0)
                glBegin(GL_TRIANGLES);

           glNormal3f(tempVertex.normal.x, tempVertex.normal.y, tempVertex.normal.z);

           glTexCoord2f(tempVertex.uv.x, tempVertex.uv.y);

           glVertex3f(tempVertex.vertex.x, tempVertex.vertex.y, tempVertex.vertex.z);

           if((i+1) % 3 == 0)
                glEnd();

       }
       glFlush();
       glBindTexture(GL_TEXTURE_2D, 0);
       glDisable(GL_TEXTURE_2D);


return;
}

/*
 * Add material into the material list
 *      name    - material name
 *      materialProperties      - properties
 */
void ObjLoaderClass::addMaterial(std::string name, materialStruct *materialProperties)
{
    materialsIndex.insert(std::make_pair(name, materialCounter));
    materialCounter++;

    materials.push_back(*materialProperties);

    //erase materialProperties
    memset(materialProperties, 0, sizeof(materialStruct));
}

//Read the MTL material file
bool ObjLoaderClass::readMTL(char *file)
{
    FILE *fp;

    fp=fopen(file,"r");
    if (!fp)
    {
        printf("can't open file %s\n", file);
        return false;
    }

    std::string currentMaterialName;
    materialStruct tempProperties;
    memset(&tempProperties, 0, sizeof(materialStruct));
    char lineHeader[128];
    char tempName[30];

    materialCounter = 0;

    while(1)
    {
        if(fgets(lineHeader, 128, fp) == NULL)
            break;

        if(strncmp(lineHeader, "newmtl", 6) == 0)
        {
            if(currentMaterialName.size() > 0)
                addMaterial(currentMaterialName, &tempProperties);

            sscanf(lineHeader, "%*s %s\n", &tempName);
            currentMaterialName = tempName;
        }
        else if(strncmp(lineHeader, "Ka ", 3) == 0)
        {
            sscanf(lineHeader, "%*s %f %f %f\n", &tempProperties.ambience[0], &tempProperties.ambience[1], &tempProperties.ambience[2]);
            tempProperties.ambience[3] = 1.0;
        }
        else if(strncmp(lineHeader, "Kd ", 3) == 0)
        {
            sscanf(lineHeader, "%*s %f %f %f\n", &tempProperties.diffuse[0], &tempProperties.diffuse[1], &tempProperties.diffuse[2]);
            tempProperties.diffuse[3] = 1.0;
        }
        else if(strncmp(lineHeader, "Ks ", 3) == 0)
        {
            sscanf(lineHeader, "%*s %f %f %f\n", &tempProperties.specular[0], &tempProperties.specular[1], &tempProperties.specular[2]);
            tempProperties.specular[3] = 1.0;
        }
        else if(strncmp(lineHeader, "map_Kd ", 7) == 0)
        {
            sscanf(lineHeader, "%*s %s\n", &tempProperties.texture);
            readTexture(tempProperties.texture, &tempProperties);
        }
    }
    addMaterial(currentMaterialName, &tempProperties);

    fclose(fp);

    return true;
}

//Set the current material
void ObjLoaderClass::setMaterial(ObjLoaderClass::materialStruct materialProperties)
{
    glBindTexture(GL_TEXTURE_2D, materialProperties.textureID);

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialProperties.diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialProperties.ambience);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialProperties.specular);
    float shine = 27;
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shine);
}

#define IF_PNG 0

void ObjLoaderClass::readTexture(char *textureFile, ObjLoaderClass::materialStruct *materialProps)
{
    //glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &materialProps->textureID);

    glBindTexture(GL_TEXTURE_2D, materialProps->textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST);
#if IF_PNG == 1
   // For .PNG:
    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, textureFile);

    if(error)
    {
        printf("PNG error: %s\n", lodepng_error_text(error));
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width,
                 height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 &image[0]);

#else

    //For .TGA:

    NS_TGALOADER::IMAGE *tga = new NS_TGALOADER::IMAGE();
    //if(tga->LoadTGA("house_obj/house_diffuse.tga") == false)
    if(tga->LoadTGA(textureFile) == false)
    {
        printf("TGA error\n");
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tga->getWidth(),
                     tga->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     tga->getDataForOpenGL());
#endif

}

/*
 * MeshObjectNode is the scene node wrapper representing a ObjeLoaderClass
 */
MeshObjectNode::MeshObjectNode(char *file, char *MTLfile, int givenID)
{
    loader = new ObjLoaderClass(file, MTLfile, givenID);
}

void MeshObjectNode::update()
{
    loader->draw();
    SceneNode::update();
}

void MeshObjectNode::checkClick(ObjectPickerClass *picker, transformationPropertiesStruct transformation)
{
    loader->checkClick(picker, transformation);
    SceneNode::checkClick(picker, transformation);
}








