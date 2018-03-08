#include "CollisionClass.h"
#define GLM_FORCE_RADIANS
#include <glm/geometric.hpp>
#include <math.h>
#include <stdio.h>
#include <glm/gtx/vector_angle.hpp>

/*
 * Handles collisions - this class is created on each step
 *      position    - position of player
 *      direction   - movement vector
 *      doorList    - pointer to the door list - used to see if we can walk through the doorway
 */
CollisionClass::CollisionClass(glm::vec3 position, glm::vec2 direction, std::vector<DoorAnimationNode *> *doorList)
{
    this->doorList = doorList;
    this->position = position;
    this->walkingDirection = walkingDirection;
}

/*
 * Compute the collision detection
 *      returns boolean representing if we can walk straight
 */
bool CollisionClass::checkDistance()
{
    for(int i = 0; i < sizeof(boundaryMap)/(4*sizeof(float)); i++)
    {
        if( (position.x > boundaryMap[i*4]) &&
                (position.x < boundaryMap[i*4 + 1]) &&
                (position.z > boundaryMap[i*4 + 2]) &&
                (position.z < boundaryMap[i*4 + 3]))
        {
            //Found inside box
            return false;
        }
    }

    for(int i = 0; i < sizeof(boundaryMap)/(4*sizeof(float)); i++)
    {
        if((position.x > boundaryMap[i*4]) && (position.x < boundaryMap[i*4 + 1]))
        {
            if(fabs(position.z - boundaryMap[i*4 + 2]) < CLOSE_ENOUGH)
            {
                increments.y = 0;
                increments.x = 1;
            }

            if(fabs(position.z - boundaryMap[i*4 + 3]) < CLOSE_ENOUGH)
            {
                increments.y = 0;
                increments.x = 1;
            }
        }

        if((position.z > boundaryMap[i*4+2]) && (position.z < boundaryMap[i*4 + 3]))
        {
            if(fabs(position.x - boundaryMap[i*4]) < CLOSE_ENOUGH)
            {
                increments.x = 0;
                increments.y = 1;
            }

            if(fabs(position.x - boundaryMap[i*4+1]) < CLOSE_ENOUGH)
            {
                increments.y = 1;
                increments.x = 0;
            }
        }
    }

    for(int i = 0; i < sizeof(doorLines)/(6*sizeof(float)); i++)
    {
        if(position.z > doorLines[i*6+0] && position.z < doorLines[i*6+1])
        {
            if(fabs(position.x - doorLines[i%4+2]) < CLOSE_ENOUGH)
            {
                //On door
                printf("On Door\n");
                if((*doorList)[(i-i%12)/12]->isOpen())
                {
                    return false;
                }
                else
                {
                    increments.x = 0;
                    increments.y = 1;
                    return true;
                }
            }
        }

        if(position.x > doorLines[i*6+3] && position.x < doorLines[i*6+4])
        {
            if(fabs(position.z - doorLines[i*6+5]) < CLOSE_ENOUGH)
            {
                //On door
                printf("On Door\n");
                if((*doorList)[(i-i%12)/12]->isOpen())
                {
                    return false;
                }
                else
                {
                    increments.x = 1;
                    increments.y = 0;
                    return true;
                }
            }
        }
    }

    if((fabs(position.z - fenceLine[0]) < CLOSE_ENOUGH) &&
            position.y < fenceLine[1])
    {
        increments.y = 0;
        increments.x = 1;
        return true;
    }

    if(increments.x == 0 && increments.y == 0)
        return false;
    return true;
}

/*
 *  If we cannot walk straight, return the X, Z factors to multiply the walking vector by
 */
float CollisionClass::returnXincrement()
{
    return increments.x;
}

float CollisionClass::returnZincrement()
{
    return increments.y;
}
