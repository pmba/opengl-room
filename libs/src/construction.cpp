#include "../construction.h"
#include "../utils.h"

/**
 * Method to build a wall in counter-clockwise direction
 * 
 * @param vertices is a 4x3 matrix containing all the wall vertices
 * @param color is a float array that determines the color of the wall
 */
void buildWall(float vertices[4][3], GLfloat* color) {
    glColor3f(color[0], color[1], color[2]);

    glPushMatrix();
    glBegin(GL_QUADS);

        for (int wall = 0; wall < 4; ++wall) {
            glVertex3f(vertices[wall][0], vertices[wall][1], vertices[wall][2]);
        }

    glEnd();
    glPopMatrix();
}

void buildRoom() {
    // Set material properties
	GLfloat qaBlack[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat qaGreen[] = {0.0, 1.0, 0.0, 1.0};
	GLfloat qaWhite[] = {1.0, 1.0, 1.0, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, qaGreen);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaGreen);
	glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
	glMaterialf(GL_FRONT, GL_SHININESS, 60.0);

    glNormal3f(0.0, 0.0, 1.0);

    // Front Wall [5, 0, -5] with (width=5, height=3)
    buildWall(new float[4][3] {
        { 5.0, 0.0, -5.0 },
        { 5.0, 3.0, -5.0 },
        { 0.0, 3.0, -5.0 },
        { 0.0, 0.0, -5.0 }
    }, getColor(241, 241, 241));

    glNormal3f(1.0, 0.0, 0.0);

    // Left Wall [0, 0, -5] with (width=5, height=3)
    buildWall(new float[4][3] {
        { 0.0, 0.0, -5.0 },
        { 0.0, 3.0, -5.0 },
        { 0.0, 3.0,  0.0 },
        { 0.0, 0.0,  0.0 }
    }, getColor(241, 241, 241));

    glNormal3f(-1.0, 0.0, 0.0);

    // Right Wall [5, 0, -5] with (width=5, height=3)
    buildWall(new float[4][3] {
        { 5.0, 0.0, -5.0 },
        { 5.0, 0.0,  0.0 },
        { 5.0, 3.0,  0.0 },
        { 5.0, 3.0, -5.0 }
    }, getColor(241, 241, 241));

    glNormal3f(0.0, 0.0, -1.0);

    // Back Wall [0, 0, 0] with (width=5, height=3)
    buildWall(new float[4][3] {
        { 0.0, 0.0,  0.0 },
        { 0.0, 3.0,  0.0 },
        { 5.0, 3.0,  0.0 },
        { 5.0, 0.0,  0.0 }
    }, getColor(241, 241, 241));

    glFlush();
}