#include "../foundation.h"

void drawRoomWalls() {
    glColor3f(1, 1, 0);

    glPushMatrix();
    glBegin(GL_QUADS);

        glVertex3f(1, 0, 1);
        glVertex3f(1, 0, 0);
        glVertex3f(0, 1, 0);
        glVertex3f(0, 0, 1);

    glEnd();
    glPopMatrix();
}