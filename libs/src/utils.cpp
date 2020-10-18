#include "../utils.h"
#include <math.h>

void drawAxis(bool drawXAxis, bool drawYAxis, bool drawZAxis) {

    glPushMatrix();
    
    if (drawXAxis) {
        glColor3f(1, 0, 0);
        glBegin(GL_LINES);
            glVertex3f(0, 0, 0);
            glVertex3f(5, 0, 0);
        glEnd();
    }

    if (drawYAxis) {
        glColor3f(0, 1, 0);
        glBegin(GL_LINES);
            glVertex3f(0, 0, 0);
            glVertex3f(0, 5, 0);
        glEnd();
    }

    if (drawZAxis) {
        glColor3f(0, 0, 1);
        glBegin(GL_LINES);
            glVertex3f(0, 0, 0);
            glVertex3f(0, 0, 5);
        glEnd();
    }

    glPopMatrix();
}

/**
 * Method to return the position at the surface of a sphere
 * 
 * @param x0 a float that represents the initial x position
 * @param y0 a float that represents the initial y position
 * @param z0 a float that represents the initial z position
 * @param radius a float that represents the sphere radius
 * @param xAngle a float that represents the horizontal angle variation, i.e (alpha)
 * @param yAngle a float that represents the vertical angle variation, i.e (phi)
 * */
std::vector<float> getSpherePositions(float x0, float y0, float z0, float radius, float xAngle, float yAngle) {
    float x = x0 + radius * cos(xAngle) * sin(yAngle);
    float y = y0 + radius * cos(yAngle);
    float z = z0 + radius * sin(xAngle) * sin(yAngle);

    std::vector<float> result { x, y, z };
    return result;
}