#define GLM_FORCE_RADIANS
#include <glm/common.hpp>
#include "AnimationNodes.h"
#include <vector>

class CameraClass
{
public:
    CameraClass(std::vector<DoorAnimationNode*> *doorList);
    void init();
    void putCamera();
    void startCameraMove(int x, int y);
    void endCameraMove();
    void mouseMove(int x, int y);
    void computePos();
    void startPlayerMove(int forward, int right);
    void endPlayerMove(int forward, int right);
    glm::vec3 getPlayerPosition();

    void doWalk(int forward, int right);
    void doJump();
    void stopWalk(int forward, int right);

private:
#define FRAME_RATE 30.0f
#define TIME_PER_FRAME 1.0f/FRAME_RATE

#define Y_POSITION 50.0f
#define SPEED_SCALE 69
#define JUMP_SPEED 5.0f
#define GRAVITY_PER_FRAME 4.0f/FRAME_RATE

#define STEP_LENGTH_FRAMES 25
#define STEP_Y_HEIGHT 5.0f
#define STEP_XZ_WOBBLE 1.0f

#define ANIMATION_WALKING 1
#define ANIMATION_JUMPING 2
#define ANIMATION_STRAFE 4

#define TOO_LARGE_X 1000000
#define TOO_LARGE_Z 1000000
    bool ifCameraMove;
    float deltaHorizontalAngle, horizontalAngle;
    float deltaVerticalAngle, verticalAngle;
    float xOrigin, yOrigin;
    float lx, lz, ly, x, z, y;
    float deltaMove, deltaHorizontalMove;

    float ySpeed;
    int ifAnimationNeeded;
    bool ifWalking;
    int ifWalkingForward, ifWalkingLeft;
    float framesElapsedWalking;
    bool ifXwobbledLast;

    std::vector<DoorAnimationNode*> *doorList;

    void doAnimation();
};
