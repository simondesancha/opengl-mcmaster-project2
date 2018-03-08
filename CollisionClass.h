#ifndef COLLISIONCLASS_H
#define COLLISIONCLASS_H

#define GLM_FORCE_RADIANS
#include <glm/common.hpp>
#include "AnimationNodes.h"
#include <vector>

class CollisionClass
{
public:
    CollisionClass(glm::vec3 position, glm::vec2 direction, std::vector<DoorAnimationNode *> *doorList);
    bool checkDistance();
    float returnXincrement();
    float returnZincrement();

private:
#define PLAYER_RADIUS 1.0f
#define CLOSE_ENOUGH 2.0f

    float boundaryMap[24] = {
        -160, 271, -242, 492,
        -558, -160, -242, 236,
        271, 664, -242, 236,

        -1271, -840, 508, 1242,
        -840, -442, 764, 1242,
        -1664, -1271, 764, 1242
    };

    float doorLines[24] = {
        236, 366, -160,    //Z Z X
        -290, -160, 236,    //X X Z

        236, 366, -290,    //Z Z X
        -290, -160, 492,    //X X Z


        634, 764, -840,    //Z Z X
        -840, -710, 764,    //X X Z

        634, 764, -710,    //Z Z X
        -840, -710, 512    //X X Z
    };

    float fenceLine[2] = {
        492, 70             //Z Y
    };

    glm::vec2 lineMap[2] = {
       glm::vec2(270,492), glm::vec2(-160, 493),
    };

    float distanceToLine(glm::vec2 line[]);
    void setIncrements(glm::vec2 line[]);

    glm::vec3 position;
    glm::vec2 walkingDirection;
    glm::vec2 increments;

    std::vector<DoorAnimationNode*> *doorList;
};

#endif // COLLISIONCLASS_H
