// #define GL_GLEXT_PROTOTYPES
#define GLEW_STATIC
// #include <GL/gl.h>
// #include <GL/glu.h>
// #include <GL/glut.h>
#include <GL/glew.h>
#include <iostream>
#include <unistd.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./libs/stb_image.h"
#include "./libs/camera.h"
#include "./libs/build.h"
#include "./libs/texture.h"
#include "./libs/animation.h"
#include "./libs/solids.h"
#include "./libs/material.h"

#include "./libs/obj_loader.h"

static int windows_w = 1080, windows_h = 720;

Camera camera;

Texture floorTexture;
Texture wallTexture;
Texture windowTexture;
Texture doorTexture;
Texture doorFrameTexture;
Texture homeOfficeTexture;
Texture booksTexture;

// Materials

Material ambient(0.7, 0.7, 0.7, 0.8, 0.8, 0.8, 1.0, 1.0, 1.0, 50.0);
Material chrome(0.25, 0.25, 0.25, 0.4, 0.4, 0.4, 0.774597, 0.774597, 0.774597, 0.6);
Material silver(0.19225, 0.19225, 0.19225, 0.50754, 0.50754, 0.50754, 0.508273, 0.508273, 0.508273, 0.4);

// Light Props

static GLfloat light0_ambient[]	    = { 0.2, 0.2, 0.2, 1.0 };
static GLfloat light0_diffuse[]	    = { 0.8, 0.8, 0.8, 1.0 };
static GLfloat light0_specular[]	= { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light0_position[]	= { 0.0, 0.0, 0.0, 1.0 };

static bool light1_enabled          = true;
static GLfloat light1_offset[]	    = { 19.4, 4, -19.2 };
static GLfloat light1_ambient[]	    = { 0.2, 0.2, 0.2, 1.0 };
static GLfloat light1_diffuse[]	    = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light1_specular[]	= { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light1_position[]	= { 0.0, 3.0, 0.0, 1.0 };
static GLfloat light1_direction[]	= { 0.0, -1.0, 0.0 };
static GLfloat light1_angle	        = 20.0;
static GLfloat light1_exponent	    = 2.0;

// Quadric

GLUquadric* cylinderQuadric;

// Loader

objl::Loader loader;

// Animations

AnimationState doorAnimation = IDLE;
GLfloat doorAngle = 0.f;

AnimationState windowAnimation = IDLE;
GLfloat windowTranslation = 1.f;

using namespace std;

void onPositionUpdated(glm::vec3 position);

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

void initTextures()
{
    // Loading Textures ......................................................................

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

    loadTexture("./res/textures/home-office.png", &homeOfficeTexture);
    setupTexture(&homeOfficeTexture);

    loadTexture("./objs/books/books.png", &booksTexture);
    setupTexture(&booksTexture);
}

void init(void)
{   
    glClearColor (0.31, 0.61, 0.85, 1.0);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    // Anti aliasing smoothing props ........................................................
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);

    // Lighting and Shade setup .............................................................
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL); 
    glShadeModel(GL_SMOOTH);
 
    // Turn on OpenGL lighting ..............................................................
    glEnable(GL_LIGHTING); 
    
	// Light 0 property vectors .............................................................

    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

    glEnable(GL_LIGHT0);

    // End Light 0 ..........................................................................
    // Light 1 property vectors .............................................................

    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, light1_angle);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_direction);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, light1_exponent);

    glEnable(GL_LIGHT1);

    // End Light 1 ..........................................................................

   // Enable color material mode:
   // The ambient and diffuse color of the front faces will track the color set by glColor().
   glEnable(GL_COLOR_MATERIAL); 
   glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); 

   initTextures();
}

void setupLightning()
{
    glEnable(GL_LIGHTING); 

    glEnable(GL_LIGHT0);
    glPushMatrix();
        glTranslatef(roomWidth / 2, roomHeight - 0.5, -(roomWidth / 2));
        GLfloat light0_position[]	= {0, 0, 0, 1};
        glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glPopMatrix();

    if (light1_enabled) {
        glEnable(GL_LIGHT1);
        glPushMatrix();
            glTranslatef(light1_offset[0], light1_offset[1], light1_offset[2]);

            glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
            glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, light1_angle);
            glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_direction);
            glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, light1_exponent);
        glPopMatrix();
    }
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

void checkWindowTranslation()
{
    if (windowAnimation == FORWARDS || windowAnimation == BACKWARDS)
    {
        windowTranslation += windowAnimation == FORWARDS ? 0.02 : -0.02;
        if (windowTranslation >= 1.0f || windowTranslation <= 0.0f) windowAnimation = IDLE;
    }
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor (0.31, 0.61, 0.85, 1.0);

    // displayText(windows_w, windows_h, actionText);
    drawCrosshair(windows_w, windows_h);

    setupCamera();
    setupLightning();

    glPushMatrix();
      glTranslatef(light1_offset[0], light1_offset[1], light1_offset[2]); // Move the spotlight.
      
      // Draw the spotlight cone in wireframe after disabling lighting
      glPushMatrix();
         glDisable(GL_LIGHTING);
            glRotatef(-90.0, 1.0, 0.0, 0.0);
            glColor3f(1.0, 0.0, 1.0);
            glutSolidCone(3.0 * tan( light1_angle/180.0 * M_PI ), 3.0, 20, 20);
         glEnable(GL_LIGHTING);
      glPopMatrix();
    
   glPopMatrix();

    ambient.active();

    buildFrontWall(&wallTexture);
    // buildBackWall(&wallTexture);
    buildLeftWall(&wallTexture);
    buildRightWall(&wallTexture);
    buildRoof(&wallTexture);
    buildFloor(&floorTexture);

    buildDoorFrame(&wallTexture);

    checkDoorAngle();
    buildDoor(&doorTexture, doorAngle);

    buildBoard(&homeOfficeTexture, rgb(154, 149, 143), {1.5, 1.8618, 0.1});
    buildTable(false);
    // buildTable(true);

    checkWindowTranslation();
    glPushMatrix();
        glTranslatef(windowPosition.x + 0.2, windowPosition.y + 0.2, -(roomWidth + 0.4));
        buildWindowGlass();
    glPopMatrix();

    glPushMatrix();
        GLfloat windowTranslationOffset = ((windowDimention.x - (2 * windowDimention.z)) / 2) * windowTranslation;
        glTranslatef((windowPosition.x) + windowTranslationOffset, windowPosition.y + 0.2, -(roomWidth + 0.6));
        buildWindowGlass();
    glPopMatrix();

    buildShelfWithBooks();

    buildWindow(silver);

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
        case 'p':
            doorAnimation = doorAngle == 90.0f ? BACKWARDS : FORWARDS;
            break;
        case 'j':
            windowAnimation = windowTranslation == 1.0f ? BACKWARDS : FORWARDS;
            break;
        case 'l':
            light1_enabled = !light1_enabled;
            if (!light1_enabled) glDisable(GL_LIGHT1);
            break;

        case '4':
            light1_offset[0] -= 0.2;
            cout << light1_offset[0] << ", " << light1_offset[1] << ", " << light1_offset[2] << endl;
            break;
        case '5':
            light1_offset[2] += 0.2;
            cout << light1_offset[0] << ", " << light1_offset[1] << ", " << light1_offset[2] << endl;
            break;
        case '6':
            light1_offset[0] += 0.2;
            cout << light1_offset[0] << ", " << light1_offset[1] << ", " << light1_offset[2] << endl;
            break;
        case '8':
            light1_offset[2] -= 0.2;
            cout << light1_offset[0] << ", " << light1_offset[1] << ", " << light1_offset[2] << endl;
            break;
        case '7':
            light1_offset[1] += 0.2;
            cout << light1_offset[0] << ", " << light1_offset[1] << ", " << light1_offset[2] << endl;
            break;
        case '9':
            light1_offset[1] -= 0.2;
            cout << light1_offset[0] << ", " << light1_offset[1] << ", " << light1_offset[2] << endl;
            break;
        
        default:
            break;
    }
}

void specialKeyInput(int key, int x, int y)
{
    switch (key) {
        case GLUT_KEY_F1:
            glDisable(GL_LIGHT0);
            break;
        default:
            break;
    }

    glutPostRedisplay();
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
   
   glutCreateWindow("Trabalho Final - CG");

   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(specialKeyInput);
   glutMotionFunc(motion);
   glutTimerFunc(30, update, 0);

    // bool loadout = loader.LoadFile("./objs/books/books.obj");

    // cout << "Loadout: " << loadout << endl;

   init();
   glutMainLoop();
   return 0; 
}