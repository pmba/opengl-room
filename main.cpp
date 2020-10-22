#include <iostream>
#include <math.h>
#include <vector>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
 
#include "./libs/utils.h"
#include "./libs/construction.h"

#ifndef DEBUG
#define DEBUG if (false)
#endif

#define CAMERA_SPEED 0.008
#define MOV_SPEED 0.1

using namespace std;

/* 0 <= xRotationAngle <= 2PI */
float xRotationAngle = 4.8112;
/* 0 <= yRotationAngle <= PI */
float yRotationAngle = 1.4254;

float cameraRadius = 5.f;

int previousMotionX = 0;
int previousMotionY = 0;

// Center of the sphere of vision
float* centerPoint = new float[3] { 0.0, 0.0, 0.0 };
// The direction vector, will be used to determine the direction that we need to move
float* directionVector = new float[3] { 0.0, 0.0, 0.0 };

void setupCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(atan(tan(50.0 * 3.14159 / 360.0) / 1.0) * 360.0 / 3.141593, 1.0, 0.1, 14);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    vector<float> positions = getSpherePositions(
        centerPoint[0], centerPoint[1], centerPoint[2], 
        cameraRadius, xRotationAngle, yRotationAngle
    );

    for (int vectorPos = 0; vectorPos < 3; ++vectorPos) {
        directionVector[vectorPos] = (positions[vectorPos] - centerPoint[vectorPos]) / cameraRadius;
    }

    gluLookAt(centerPoint[0], centerPoint[1], centerPoint[2],
              positions[0], positions[1], positions[2],
              0.0, 1.0, 0.0);
}

void draw(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setupCamera();

    drawAxis(true, true, true);
    buildWall(new float[3]{ 5.0, 0.0, -5.0 }, 5.0, 3.0, getColor(255, 0, 0));

    glutSwapBuffers();
}

/* Control Functions */

void keyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
        case 'q':
            exit(EXIT_SUCCESS);
            break;
        case 'w':
            centerPoint[0] += directionVector[0] * MOV_SPEED;
            centerPoint[1] += directionVector[1] * MOV_SPEED;
            centerPoint[2] += directionVector[2] * MOV_SPEED;
            break;
        case 's':
            centerPoint[0] -= directionVector[0] * MOV_SPEED;
            centerPoint[1] -= directionVector[1] * MOV_SPEED;
            centerPoint[2] -= directionVector[2] * MOV_SPEED;
            break;
        default:
            break;
    }
}

void mouseFunction(int button, int state, int x, int y) {
    if ((button == 3) || (button == 4)) {
        // Mouse wheel event
        if (state == GLUT_UP) return;
        if (button == 3) {
            // Wheel Up
            if (cameraRadius > 0.1) cameraRadius -= 0.1;
        } else {
            // Wheel Down
            cameraRadius += 0.1;
        }
    }
}

void motionFunction(int x, int y) {
    DEBUG cout << "motionFunction : (X=" << x << ", Y=" << y << ")" << endl;

    if (x < previousMotionX) {
        // Moving mouse left direction
        DEBUG cout << "motionFunction : Moving Left" << endl;
        xRotationAngle += CAMERA_SPEED;
    } else if (x > previousMotionX) {
        // Moving mouse right direction
        DEBUG cout << "motionFunction : Moving Right" << endl;
        xRotationAngle -= CAMERA_SPEED;
    }

    if (xRotationAngle < 0) xRotationAngle = 2*M_PI;
    else if (xRotationAngle > 2*M_PI) xRotationAngle = 0;

    previousMotionX = x;

    if (y < previousMotionY) {
        // Moving mouse up
        DEBUG cout << "motionFunction : Moving Up" << endl;
        yRotationAngle += CAMERA_SPEED;
    } else if (y > previousMotionY) {
        // Moving mouse down
        DEBUG cout << "motionFunction : Moving Down" << endl;
        yRotationAngle -= CAMERA_SPEED;
    }

    if (yRotationAngle < 0) yRotationAngle = M_PI;
    else if (yRotationAngle > M_PI) yRotationAngle = 0;

    previousMotionY = y;
}

/* End Control Functions */
 
void initRendering() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0.7f, 0.9f, 1.0f, 1.0f);
}

void update(int value) {
    glutPostRedisplay();
    glutTimerFunc(60, update, 0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 500);

    glutCreateWindow("Hello world!");
    initRendering();

    glutDisplayFunc(draw);
    glutKeyboardFunc(keyboardFunc);
    glutMouseFunc(mouseFunction);
    glutMotionFunc(motionFunction);

    glutTimerFunc(25, update, 0);

    glutMainLoop();
    return 0;
}