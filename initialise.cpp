#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <string>
#include <string.h>
#define GLM_FORCE_RADIANS
#include <glm/common.hpp>
#include "initialise.h"
#include "lodepng.h"
#include "DrawingClass.h"
#include "BackdropClass.h"
#include "CollisionClass.h"
#include "CameraClass.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

DrawingClass *drawer;
CameraClass *camera;
BackdropClass *backdrop;

//Draw the floor
void drawFloor()
{
    float temp[4] = {0.4, 0.1, 0.1, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, temp);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, temp);
    glBegin(GL_QUADS);
        for(float i = -200; i < 100; i++)
        {
            for(float j = -100; j < 200; j++)
            {
                glVertex3f(10*i, 0.0f, 10*j);
                glVertex3f(10*i+10, 0.0f, 10*j);
                glVertex3f(10*i+10, 0.0f, 10*j+10);
                glVertex3f(10*i, 0.0f, 10*j+10);
            }
        }
    glEnd();
}

//Mouse button call back
void mouseButton(int button, int state, int winX, int winY) {
    if (button == GLUT_LEFT_BUTTON)
    {
        //Either start or stop the camera movement - handled by camera class
        if (state == GLUT_UP)
            camera->endCameraMove();
        if(state == GLUT_DOWN)
            camera->startCameraMove(winX, winY);
    }
    else if((button == GLUT_RIGHT_BUTTON) && (state == GLUT_UP))
    {
        //Let the drawing class process the left click
        drawer->processClick(camera->getPlayerPosition(), winX, winY);
    }
}


//Mouse movement callback
void mouseMove(int x, int y)
{
    camera->mouseMove(x, y);
}

//Main display function
void displayMain()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    //Only draw the backdrop if it is daytime
    if(drawer->getIfDayTime())
        backdrop->draw();

    //Load the identity after drawing the backdrop
    glLoadIdentity();

    //Place the camera on the map
    camera->putCamera();

    //Draw the floor
    drawFloor();
    glPushMatrix();

    //Draw the scene graph
    drawer->draw();

    glPopMatrix();
    glutSwapBuffers();
    glFlush();
}

#define TRANSLATION_ARGS(X, Z) glm::vec3(X, 0.0f, Z), glm::vec3(0.0f, 0.0f, 0.0f), 1
#define ROTATION_ARGS(X, Y, Z) glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(X, Y, Z), 1
#define SCALE_ARGS(scale) glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), scale

void keyBoardCallBack(unsigned char key, int, int)
{
    switch(key)
    {
        case 's':
            drawer->transformSelected(TRANSLATION_ARGS(-1,0));
            break;
        case 'w':
            drawer->transformSelected(TRANSLATION_ARGS(1, 0));
            break;
        case 'd':
            drawer->transformSelected(TRANSLATION_ARGS(0, 1));
            break;
        case 'a':
            drawer->transformSelected(TRANSLATION_ARGS(0, -1));
            break;
        case 'q':
            drawer->transformSelected(SCALE_ARGS(0.9));
            break;
        case 'e':
            drawer->transformSelected(SCALE_ARGS(1.1));
            break;
        case 'r':
            drawer->transformSelected(ROTATION_ARGS(1.0f, 0.0f, 0.0f));
            break;
        case 't':
            drawer->transformSelected(ROTATION_ARGS(0.0f, 1.0f, 0.0f));
            break;
        case 'y':
            drawer->transformSelected(ROTATION_ARGS(0.0f, 0.0f, 1.0f));
            break;
        case 'z':
            drawer->addShape(Cube, glm::vec3(0.0f, 0.0f, 0.0f), 10);
            break;
        case 'x':
            drawer->addShape(Sphere, glm::vec3(0.0f, 0.0f, 0.0f), 10);
            break;
        case 'c':
            drawer->addShape(Cylinder, glm::vec3(0.0f, 0.0f, 0.0f), 10);
            break;
        case 'v':
            drawer->addShape(Teapot, glm::vec3(0.0f, 0.0f, 0.0f), 10);
            break;
        case ' ':
            camera->doJump();
            break;
    }
    //glutPostRedisplay();
}

//Callback for the arrow keys
void specialKey(int key, int, int){
    //Change the angle of the map
    switch(key)
    {
        case GLUT_KEY_LEFT:
            camera->doWalk(0, -1);
            break;
        case GLUT_KEY_RIGHT:
            camera->doWalk(0, 1);
            break;
        case GLUT_KEY_UP :
            camera->doWalk(1, 0);
            break;
        case GLUT_KEY_DOWN :
            camera->doWalk(-1, 0);
            break;
    }
}

void releaseKey(int key, int x, int y) {

    switch (key)
    {
        case GLUT_KEY_UP :
        case GLUT_KEY_DOWN :
            camera->stopWalk(1, 0);
            break;
        case GLUT_KEY_LEFT:
        case GLUT_KEY_RIGHT:
            camera->stopWalk(0, 1);
            break;
    }
}

//Handle screen resize
void windowResize(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

void initEnvironment()
{
    LightPropertiesStruct lightProperties;

    drawer = new DrawingClass();

    drawer->addDoor("models/door.obj", "models/house_obj.mtl");
    drawer->addDoor("models/door.obj", "models/house_obj.mtl",
                    glm::vec3(-1000, 0, 1000), glm::vec3(0, 180, 0));
    drawer->loadObjMesh("models/fenceGood.obj", "models/fence.mtl",
                        glm::vec3(-500, 0, 500), glm::vec3(0, 90, 0));
    drawer->loadObjMesh("models/fenceGood.obj", "models/fence.mtl",
                        glm::vec3(-680, 0, 500), glm::vec3(0, 90, 0));
    drawer->loadObjMesh("models/fenceGood.obj", "models/fence.mtl",
                        glm::vec3(-860, 0, 500), glm::vec3(0, 90, 0));

    lightProperties = {glm::vec4(1, 1, 1, 0.1),
                       glm::vec4(0.1, 0.1, 0, 0.1),
                       glm::vec4(0.1, 0.1, 0, 0.1),
                       glm::vec4(0.1, 0.1, 0, 0.1),
                       SpotLight,
                       25.0f,
                       glm::vec3(0.2f, -1.0f, 0.0f)};

    int lightIndex = drawer->addLight(glm::vec3(0.0f, 100.0f, 0.0f), lightProperties);
    drawer->addSwitch("models/button.obj", "models/button.mtl", lightIndex);

    drawer->loadObjMesh("models/light.obj", "models/light.mtl", glm::vec3(0, 260.0f, 0), glm::vec3(0,0,0));

    lightProperties = {
        glm::vec4(0.58f, 0.89f, 0.62f, 1),
        glm::vec4(0.58f, 0.89f, 0.62f, 1),
        glm::vec4(0.58f, 0.89f, 0.62f, 1),
        glm::vec4(0.58f, 0.89f, 0.62f, 1),
        PointLight,
        25.0f,
        glm::vec3(0.0f, -1.0f, 0.0f)
    };

    drawer->addOrbit("models/moon2.obj", "models/moon2.mtl",
                            glm::vec3(-3000, 0, 0), glm::vec3(0,0,0),
                            glm::vec3(1, 0, 0), lightProperties, 0, false);

    lightProperties = {
        glm::vec4(0.99f, 0.89f, 0.05f, 1),
        glm::vec4(0.99f, 0.89f, 0.05f, 1),
        glm::vec4(0.99f, 0.89f, 0.05f, 1),
        glm::vec4(0.99f, 0.89f, 0.05f, 1),
        PointLight,
        25.0f,
        glm::vec3(0.0f, -1.0f, 0.0f)
    };

    drawer->addOrbit("models/moon2.obj", "models/sun.mtl",
                           glm::vec3(-3000, 0, 0), glm::vec3(180,0,0),
                            glm::vec3(1, 0, 0), lightProperties, 180, true);

    drawer->loadObjMesh("models/house.obj", "models/house_obj.mtl");
    drawer->loadObjMesh("models/house.obj", "models/house_obj.mtl",
                        glm::vec3(-1000, 0, 1000), glm::vec3(0, 180, 0));



    backdrop = new BackdropClass("models/cowsBackground.png");

    camera = new CameraClass(drawer->getDoorList());
}

//Only re-display on this timer in order to limit the frame-rate
void timerCB(int millisec)
{
        glutTimerFunc(millisec, timerCB, millisec);
        glutPostRedisplay();
}

int initGlut(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Project");

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glClearColor(0, 0, 0, 0);
    gluPerspective(45,1,1,3500);

    glutKeyboardFunc(keyBoardCallBack);
    glutDisplayFunc(displayMain);
    glutSpecialFunc(specialKey);
    glutSpecialUpFunc(releaseKey);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMove);
    glutPassiveMotionFunc(mouseMove);
    glutTimerFunc(TIME_PER_FRAME, timerCB, TIME_PER_FRAME);

    initEnvironment();

    glutMainLoop();

    return 0;
}

