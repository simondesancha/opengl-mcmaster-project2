#include "CameraClass.h"
#include "CollisionClass.h"
#include <GL/glu.h>
#include <math.h>

/*
 * CameraClass handles the camera and the walking
 *      doorlist    - pointer to the door list (used to tell if we can walk through the doors)
 */
CameraClass::CameraClass(std::vector<DoorAnimationNode *> *doorList)
{
    this->doorList = doorList;
    x = 0;
    y = Y_POSITION;
    z = 0;
    verticalAngle = 0;
    horizontalAngle = 0;
    ifAnimationNeeded = 0;
    deltaHorizontalMove = 0;
}

//Place the camera on the map
void CameraClass::putCamera()
{
    if(ifAnimationNeeded != 0)
    {
        doAnimation();
    }
    else
    {
        y = Y_POSITION;
    }
    gluLookAt(	x, y, z,
                x+lx, y - ly,  z+lz,
                0.0f, 1.0f,  0.0f);
}

//Start the camera move (ie start clicking-and-dragging)
void CameraClass::startCameraMove(int x, int y)
{
    xOrigin = x;
    yOrigin = y;
    ifCameraMove = true;
}

//Stop the camera move (ie stop clicking-and-dragging)
void CameraClass::endCameraMove()
{
    horizontalAngle += deltaHorizontalAngle;
    verticalAngle += deltaVerticalAngle;
    ifCameraMove = false;
}

//Continue the camera move
void CameraClass::mouseMove(int x, int y)
{
    if(ifCameraMove)
    {
        deltaHorizontalAngle = -(x - xOrigin) * 0.001f;
        deltaVerticalAngle = -(y - yOrigin) * 0.001f;

        lx = sin(horizontalAngle + deltaHorizontalAngle);
        lz = -cos(horizontalAngle + deltaHorizontalAngle);

        ly = sin(verticalAngle + deltaVerticalAngle);
    }
}


/*Compute the players position on the map,
 *  computes the walking, as well as collisions
 */
void CameraClass::computePos()
{
    float oldX, oldZ;
    oldX = x;
    oldZ = z;

    x += deltaMove * lx * 0.1f;
    z += deltaMove * lz * 0.1f;

    x += -1 * deltaHorizontalMove * lz * 0.1f;
    z += deltaHorizontalMove * lx * 0.1f;

    if(x > TOO_LARGE_X || z > TOO_LARGE_Z)
    {
        x = oldX;
        z = oldZ;
        return;
    }

    CollisionClass *col = new CollisionClass(this->getPlayerPosition(), glm::vec2(lx, lz), doorList);
    if(col->checkDistance())
    {
        if(col->returnXincrement() == 0)
            x = oldX;

        if(col->returnZincrement() == 0)
            z = oldZ;
    }
    delete(col);
}

//Start player move (ie arrow key is pressed)
void CameraClass::startPlayerMove(int forward, int right)
{
    if(right != 0)
    {
        deltaHorizontalMove = ((float)right)*SPEED_SCALE*(0.5f);
    }
    else if(forward != 0)
    {
        deltaMove = ((float)forward)*SPEED_SCALE*(0.5f);
    }

}

//Stop player move (ie arrow key is release)
void CameraClass::endPlayerMove(int forward, int right)
{
    if(right)
        deltaHorizontalMove = 0;

    if(forward)
        deltaMove = 0;
}

glm::vec3 CameraClass::getPlayerPosition()
{
    return glm::vec3(x, y, z);
}

//Start player move (ie arrow key is pressed)
void CameraClass::doWalk(int forward, int right)
{
    if(forward != 0)
    {
        if(ifAnimationNeeded & ANIMATION_WALKING)
            return;
        ifAnimationNeeded |= ANIMATION_WALKING;

        startPlayerMove(forward, right);
        ifWalkingForward = true;
        framesElapsedWalking = 0;
    }
    else
    {
        if(ifAnimationNeeded & ANIMATION_STRAFE)
            return;
        ifAnimationNeeded |= ANIMATION_STRAFE;

        startPlayerMove(forward, right);
    }
}

//Jump function
void CameraClass::doJump()
{
    if(ifAnimationNeeded & ANIMATION_JUMPING)
        return;
    ySpeed = JUMP_SPEED;
    ifAnimationNeeded |= ANIMATION_JUMPING;
}

//Arrow key is released
void CameraClass::stopWalk(int forward, int right)
{
    if(forward != 0)
        ifWalkingForward = false;

    if(right)
    {
        ifAnimationNeeded &= ~ANIMATION_STRAFE;
        endPlayerMove(0, 1);
    }
}

/*
 * Computes any pending camera animation (ie jumping and the walking animation)
 */
void CameraClass::doAnimation()
{
    if(ifAnimationNeeded & ANIMATION_JUMPING)
    {
        y += ySpeed;
        ySpeed -= GRAVITY_PER_FRAME;

        if(y <= Y_POSITION)
        {
            ifAnimationNeeded &= ~ANIMATION_JUMPING;
        }

        computePos();
    }
    else if(ifAnimationNeeded & ANIMATION_WALKING)
    {
        framesElapsedWalking++;

        if(framesElapsedWalking > STEP_LENGTH_FRAMES)
        {
            if(ifWalkingForward != 0)
            {
                framesElapsedWalking = 0;
            }
            else
            {
                endPlayerMove(1, 0);
                ifAnimationNeeded &= ~ANIMATION_WALKING;
            }
        }

        y = Y_POSITION + (0.5-fabs(framesElapsedWalking/STEP_LENGTH_FRAMES-0.5))*STEP_Y_HEIGHT;

        computePos();
    }
    else if(ifAnimationNeeded & ANIMATION_STRAFE)
    {
        computePos();
    }
}
