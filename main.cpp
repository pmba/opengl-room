#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./libs/stb_image.h"
#include "./libs/camera.h"
#include "./libs/build.h"
#include "./libs/texture.h"
#include "./libs/animation.h"

static int windows_w = 1080, windows_h = 720;

Camera camera;

Texture floorTexture;
Texture wallTexture;
Texture windowTexture;
Texture doorTexture;
Texture doorFrameTexture;

// Animations

AnimationState doorAnimation = IDLE;
GLfloat doorAngle = 0.f;

using namespace std;

void loadTexture(const char* fileName, Texture* texture) 
{
    // Generate Texture
    glBindTexture(GL_TEXTURE_2D, texture->id);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int nrChannels;

    texture->data = stbi_load(fileName, &texture->width, &texture->height, &nrChannels, 0);

    if (!texture->data) {
        cout << "Failed to load texture: " << fileName << endl;
        exit(1);
    } 

    // stbi_image_free(data);
}

void setupTexture(Texture* texture)
{
    glPixelStoref(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
                    GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
                    GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, 
                    texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
                    texture->data);                  
}

void init(void)
{    
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glShadeModel(GL_FLAT);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    // Lighting setup
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
	// GLfloat qaLightPosition[]	= {10.0, 10.0, -10.0, 1.0};
	GLfloat qaLightPosition[]	= {0.0, 0.0, 0.0, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
    
    loadTexture("./res/textures/wall.png", &wallTexture);
    setupTexture(&wallTexture); 

    loadTexture("./res/textures/tile.png", &floorTexture);
    setupTexture(&floorTexture);

    loadTexture("./res/textures/window.png", &windowTexture);
    setupTexture(&windowTexture);

    loadTexture("./res/textures/door2.png", &doorTexture);
    setupTexture(&doorTexture);

    loadTexture("./res/textures/door-frame.png", &doorFrameTexture);
    setupTexture(&doorFrameTexture);
}

void setupCamera() 
{
    glViewport(0, 0, (GLsizei) windows_w, (GLsizei) windows_h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat) windows_w/(GLfloat) windows_h, 1.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glm::mat3 lookUpMatrix = camera.getLookAtMatrix();

    gluLookAt(lookUpMatrix[0][0], lookUpMatrix[0][1], lookUpMatrix[0][2],
              lookUpMatrix[1][0], lookUpMatrix[1][1], lookUpMatrix[1][2],
              lookUpMatrix[2][0], lookUpMatrix[2][1], lookUpMatrix[2][2]);
}

void checkDoorAngle()
{
    if (doorAnimation == FORWARDS || doorAnimation == BACKWARDS)
    {
        doorAngle += doorAnimation == FORWARDS ? 1 : -1;
        if (doorAngle == 90.0f || doorAngle == 0.0f) doorAnimation = IDLE;
    }
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    displayText(windows_w, windows_h);

    setupCamera();

    buildFrontWall(&wallTexture);
    buildBackWall(&wallTexture);
    buildLeftWall(&wallTexture);
    buildRightWall(&wallTexture);
    buildRoof(&wallTexture);
    buildFloor(&floorTexture);

    buildDoorFrame(&wallTexture);

    checkDoorAngle();
    buildDoor(&doorTexture, doorAngle);
    
    buildWindow(&windowTexture);

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    windows_w = w;
    windows_h = h;

    glViewport(0, 0, (GLsizei) windows_w, (GLsizei) windows_h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat) windows_w/(GLfloat) windows_h, 1.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -3.6);
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
        case 27:
            exit(0);
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
        case 'q':
            camera.moveUp();
            break;
        case 'e':
            camera.moveDown();
            break;
        case 'f':
            doorAnimation = doorAngle == 90.0f ? BACKWARDS : FORWARDS;
            break;
        default:
            break;
   }
}

void motion(int x, int y)
{
    camera.mouseUpdate(glm::vec2(x, y));
}

void update(int value) {
    glutPostRedisplay();
    glutTimerFunc(30, update, 0);
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(windows_w, windows_h);
   glutInitWindowPosition(100, 100);
   glutCreateWindow(argv[0]);
   init();

   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMotionFunc(motion);
   glutTimerFunc(30, update, 0);

   glutMainLoop();
   return 0; 
}