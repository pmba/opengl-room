#include "../construction.h"

/**
 * Method to build a wall in counter-clockwise direction
 * 
 * @param pivot is a float array containing x, y and z coordinates, i.e the initial point
 * @param witdt is the width of the wall
 * @param height is the height of the wall
 * @param color is a float array that determines the color of the wall
 */
void buildWall(float* pivot, float width, float height, float* color) {
    // Get the initial coordinates
    float initialX = pivot[0];
    float initialY = pivot[1];
    float initialZ = pivot[2];

    glColor3f(color[0], color[1], color[2]);

    glPushMatrix();
    glBegin(GL_QUADS);

        glVertex3f(initialX, initialY, initialZ);
        glVertex3f(initialX, initialY + height, initialZ);
        glVertex3f(initialX - width, initialY + height, initialZ);
        glVertex3f(initialX - width, initialY, initialZ);

    glEnd();
    glPopMatrix();
}