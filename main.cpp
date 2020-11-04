#include <iostream>
#include <math.h>
#include <vector>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
 
#include "./libs/utils.h"
#include "./libs/construction.h"
#include "./libs/camera.h"

#ifndef DEBUG
#define DEBUG if (false)
#endif

#define MOV_SPEED 0.1

/* Camera Constants */
const float YAW = 0.005;
const float PITCH = 0.005;

using namespace std;

int WINDOWS_WIDTH = 800, WINDOWS_HEIGHT = 600;

float cameraYaw = 4.8112;
float cameraPitch = 1.4254;
float* cameraUp = new float[3] { 0.0, 1.0, 0.0 };

float cameraRadius = 5.f;

int previousMotionX = 0;
int previousMotionY = 0;

// Center of the sphere of vision
float* centerPoint = new float[3] { 0.0, 0.0, 0.0 };
// The direction vector, will be used to determine the direction that we need to move
float* directionVector = new float[3] { 0.0, 0.0, 0.0 };

Camera camera;

void setupCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(atan(tan(75 * M_PI / 360.0) / 1.0) * 360.0 / M_PI, WINDOWS_WIDTH/WINDOWS_HEIGHT, 0.1, 14);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glm::mat3 lookUpMatrix = camera.getLookAtMatrix();

    gluLookAt(lookUpMatrix[0][0], lookUpMatrix[0][1], lookUpMatrix[0][2],
              lookUpMatrix[1][0], lookUpMatrix[1][1], lookUpMatrix[1][2],
              lookUpMatrix[2][0], lookUpMatrix[2][1], lookUpMatrix[2][2]);
}

void draw(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    drawCrosshair(WINDOWS_WIDTH, WINDOWS_HEIGHT);

    setupCamera();

    drawAxis(true, true, true);

    buildRoom();

    glutSwapBuffers();
}

/* Control Functions */

void keyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
        case 'q':
            exit(EXIT_SUCCESS);
            break;
        case 'w':
            camera.moveForward();
            break;
        case 's':
            camera.moveBackward();
            break;
        case 'a':
            camera.moveLeft();
            break;
        case 'd':
            camera.moveRight();
            break;
    }
}

void motionFunction(int x, int y) {
    camera.mouseUpdate(glm::vec2(x, y));
}

void reshapeFunction(int width, int height) {
    WINDOWS_WIDTH = width;
    WINDOWS_HEIGHT = height;
}

/* End Control Functions */
 
void initRendering() {
    glClearColor(0.7f, 0.9f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    // Lighting set ]up
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Set lighting intensity and color
	GLfloat qaAmbientLight[]	= {0.2, 0.2, 0.2, 1.0};
	GLfloat qaDiffuseLight[]	= {0.8, 0.8, 0.8, 1.0};
	GLfloat qaSpecularLight[]	= {1.0, 1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);

	// Set the light position
	GLfloat qaLightPosition[]	= {2.5, 3.0, -2.5, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
}

void update(int value) {
    glutPostRedisplay();
    glutTimerFunc(30, update, 0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOWS_WIDTH, WINDOWS_HEIGHT);

    glutCreateWindow("Computação Gráfica - Quarto");
    initRendering();

    glutDisplayFunc(draw);
    glutKeyboardFunc(keyboardFunc);
    glutMotionFunc(motionFunction);
    glutReshapeFunc(reshapeFunction);

    glutTimerFunc(30, update, 0);

    glutMainLoop();
    return 0;
}