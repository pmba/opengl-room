#pragma once

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <string>

#include "texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

const GLfloat roomWidth = 20;
const GLfloat roomHeight = 10;
const GLfloat roomDepth = 1;

GLfloat* getNormal(glm::vec3 u, glm::vec3 v)
{
    glm::vec3 normal = glm::normalize(glm::cross(u, v));
    return glm::value_ptr(normal);
}

void displayText(int windowsWidth, int windowsHeight) 
{
    glPushMatrix();
    glViewport(0, 0, windowsWidth, windowsHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, windowsWidth, windowsHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLfloat initX = windowsWidth * 0.1;
    GLfloat initY = windowsHeight * 0.9;

    glRasterPos2i(initX, initY);

    std::string s = "PRESSIONE [F] PARA INTERAGIR";
    void * font = GLUT_BITMAP_HELVETICA_18;
    for (std::string::iterator i = s.begin(); i != s.end(); ++i)
    {
        char c = *i;
        glColor3f(1.0, 1.0, 1.0);
        glutBitmapCharacter(font, c);
    }

    glColor3f(0.0, 0.0, 0.0);

    initY -= 36;
    initX -= 24;

    glBegin(GL_QUADS);
        glVertex2f(initX, initY);
        glVertex2f(initX + 28*12, initY);
        glVertex2f(initX + 28*12, initY + 54);
        glVertex2f(initX, initY + 54);
    glEnd();

    glPopMatrix();
}

void buildWall(
    GLfloat width, GLfloat height, GLfloat depth, 
    GLfloat texCoordH = 2.0, GLfloat texCoordV = 2.0)
{
    glColor3f(1, 1, 1);
    // Front Face
    glNormal3fv(getNormal({ width, 0, 0 }, { 0, height, 0 }));
    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(texCoordH, 0.0); glVertex3f(width, 0.0, 0.0);
    glTexCoord2f(texCoordH, texCoordV); glVertex3f(width, height, 0.0);
    glTexCoord2f(0.0, texCoordV); glVertex3f(0.0, height, 0.0);
    // Back Face
    glNormal3fv(getNormal({ width, 0, depth }, { 0, height, depth }));
    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, depth);
    glTexCoord2f(texCoordH, 0.0); glVertex3f(width, 0.0, depth);
    glTexCoord2f(texCoordH, texCoordV); glVertex3f(width, height, depth);
    glTexCoord2f(0.0, texCoordV); glVertex3f(0.0, height, depth);
    // Left Face
    glNormal3fv(getNormal({ 0, height, 0 }, { 0, 0, depth }));
    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(0.0, texCoordV); glVertex3f(0.0, height, 0.0);
    glTexCoord2f(texCoordH, texCoordV); glVertex3f(0.0, height, depth);
    glTexCoord2f(texCoordH, 0.0); glVertex3f(0.0, 0.0, depth);
    // Right Face
    glNormal3fv(getNormal({ 0, height, 0 }, { 0, 0, depth }));
    glTexCoord2f(0.0, 0.0); glVertex3f(width, 0.0, 0.0);
    glTexCoord2f(0.0, texCoordV); glVertex3f(width, height, 0.0);
    glTexCoord2f(texCoordH, texCoordV); glVertex3f(width, height, depth);
    glTexCoord2f(texCoordH, 0.0); glVertex3f(width, 0.0, depth);
    // Bottom Face
    glNormal3fv(getNormal({ width, 0, 0 }, { 0, 0, depth }));
    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(0.0, texCoordV); glVertex3f(width, 0.0, 0.0);
    glTexCoord2f(texCoordH, texCoordV); glVertex3f(width, 0.0, depth);
    glTexCoord2f(texCoordH, 0.0); glVertex3f(0.0, 0.0, depth);
    // Top Face
    glNormal3fv(getNormal({ width, 0, 0 }, { 0, 0, depth }));
    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, height, 0.0);
    glTexCoord2f(0.0, texCoordV); glVertex3f(width, height, 0.0);
    glTexCoord2f(texCoordH, texCoordV); glVertex3f(width, height, depth);
    glTexCoord2f(texCoordH, 0.0); glVertex3f(0.0, height, depth);
}

void buildDoorFrame(Texture* texture) 
{
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    
    glPushMatrix();
        glTranslatef(roomWidth + roomDepth - 0.2, 0, -5);
        glRotatef(-90, 0, 1, 0);

        glBegin(GL_QUADS);
            buildWall(0.4, 8.0, roomDepth);
        glEnd();

        glTranslatef(5-0.4, 0.0, 0.0);
        glBegin(GL_QUADS);
            buildWall(0.4, 8.0, roomDepth);
        glEnd();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(roomWidth + roomDepth - 0.2, 8.0, -5);
        glRotatef(-90, 0, 1, 0);

        glBegin(GL_QUADS);
            buildWall(5.0, 0.4, roomDepth);
        glEnd();
    glPopMatrix();

    glFlush();
    glDisable(GL_TEXTURE_2D);
}

void buildDoor(Texture* texture, GLfloat angle)
{
    GLfloat width = 4.2;
    GLfloat height = 8.0;
    GLfloat depth = roomDepth / 3;

    glPushMatrix();
        glTranslatef(roomWidth, 0, -0.4);
        glRotatef(90 + angle, 0, 1, 0);

        glColor3f(0.91, 0.9, 0.93);

        glEnable(GL_TEXTURE_2D);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        glBindTexture(GL_TEXTURE_2D, texture->id);

        glBegin(GL_QUADS);

            // Front Face
            glNormal3fv(getNormal({ width, 0, 0 }, { 0, height, 0 }));
            glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
            glTexCoord2f(1.0, 0.0); glVertex3f(width, 0.0, 0.0);
            glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
            glTexCoord2f(0.0, 1.0); glVertex3f(0.0, height, 0.0);
            // Back Face
            glNormal3fv(getNormal({ width, 0, depth }, { 0, height, depth }));
            glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, depth);
            glTexCoord2f(1.0, 0.0); glVertex3f(width, 0.0, depth);
            glTexCoord2f(1.0, 1.0); glVertex3f(width, height, depth);
            glTexCoord2f(0.0, 1.0); glVertex3f(0.0, height, depth);

        glEnd();
        
        glDisable(GL_TEXTURE_2D);

        glBegin(GL_QUADS);
            // Left Face
            glNormal3fv(getNormal({ 0, height, 0 }, { 0, 0, depth }));
            glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
            glTexCoord2f(0.0, 1.0); glVertex3f(0.0, height, 0.0);
            glTexCoord2f(1.0, 1.0); glVertex3f(0.0, height, depth);
            glTexCoord2f(1.0, 0.0); glVertex3f(0.0, 0.0, depth);
            // Right Face
            glNormal3fv(getNormal({ 0, height, 0 }, { 0, 0, depth }));
            glTexCoord2f(0.0, 0.0); glVertex3f(width, 0.0, 0.0);
            glTexCoord2f(0.0, 1.0); glVertex3f(width, height, 0.0);
            glTexCoord2f(1.0, 1.0); glVertex3f(width, height, depth);
            glTexCoord2f(1.0, 0.0); glVertex3f(width, 0.0, depth);
            // Bottom Face
            glNormal3fv(getNormal({ width, 0, 0 }, { 0, 0, depth }));
            glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
            glTexCoord2f(0.0, 1.0); glVertex3f(width, 0.0, 0.0);
            glTexCoord2f(1.0, 1.0); glVertex3f(width, 0.0, depth);
            glTexCoord2f(1.0, 0.0); glVertex3f(0.0, 0.0, depth);
            // Top Face
            glNormal3fv(getNormal({ width, 0, 0 }, { 0, 0, depth }));
            glTexCoord2f(0.0, 0.0); glVertex3f(0.0, height, 0.0);
            glTexCoord2f(0.0, 1.0); glVertex3f(width, height, 0.0);
            glTexCoord2f(1.0, 1.0); glVertex3f(width, height, depth);
            glTexCoord2f(1.0, 0.0); glVertex3f(0.0, height, depth);
        glEnd();
    glPopMatrix();

    glFlush();
    glDisable(GL_TEXTURE_2D);
}

void buildFrontWall(Texture* texture)
{
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glPushMatrix();
        glTranslatef(0.0, 0.0, -20.0 - roomDepth);

        glBegin(GL_QUADS);
            buildWall(roomWidth, roomHeight, roomDepth);
        glEnd();
    glPopMatrix();

    glFlush();
    glDisable(GL_TEXTURE_2D);
}

void buildBackWall(Texture* texture)
{
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glPushMatrix();
        glTranslatef(roomWidth + roomDepth, 0.0, roomDepth);
        glRotatef(180.0, 0.0, 1.0, 0.0);

        glBegin(GL_QUADS);
            buildWall(roomWidth + roomDepth, roomHeight, roomDepth);
        glEnd();
    glPopMatrix();

    glFlush();
    glDisable(GL_TEXTURE_2D);
}

void buildLeftWall(Texture* texture)
{
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    
    glPushMatrix();
        glTranslatef(-roomDepth, 0.0, 0.0);
        glRotatef(90.0, 0.0, 1.0, 0.0);

        glBegin(GL_QUADS);
            buildWall(roomWidth, roomHeight, roomDepth);
        glEnd();
    glPopMatrix();

    glFlush();
    glDisable(GL_TEXTURE_2D);
}

void buildRightWall(Texture* texture)
{
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    
    glPushMatrix();
        glTranslatef(roomWidth + roomDepth, 0.0, -roomWidth);
        glRotatef(270.0, 0.0, 1.0, 0.0);

        glBegin(GL_QUADS);
            buildWall(roomWidth - 5, roomHeight, roomDepth);
        glEnd();

        glTranslatef(roomWidth - 5, 8.4, 0.0);
        
        glBegin(GL_QUADS);
            buildWall(5, roomHeight - 8.4, roomDepth);
        glEnd();
    glPopMatrix();

    glFlush();
    glDisable(GL_TEXTURE_2D);
}

void buildFloor(Texture* texture)
{
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    
    glNormal3f(0.0, 1.0, 0.0);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(0.0, 2.0); glVertex3f(20.0, 0.0, 0.0);
    glTexCoord2f(2.0, 2.0); glVertex3f(20.0, 0.0, -20.0);
    glTexCoord2f(2.0, 0.0); glVertex3f(0.0, 0.0, -20.0);

    glEnd();

    glFlush();
    glDisable(GL_TEXTURE_2D);
}

void buildRoof(Texture* texture)
{
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    
    glNormal3fv(getNormal({ 0, 0, -20 }, { 20, 0, 0 }));
    glBegin(GL_QUADS);

        glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 10.0, 0.0);
        glTexCoord2f(0.0, 2.0); glVertex3f(20.0, 10.0, 0.0);
        glTexCoord2f(2.0, 2.0); glVertex3f(20.0, 10.0, -20.0);
        glTexCoord2f(2.0, 0.0); glVertex3f(0.0, 10.0, -20.0);

    glEnd();

    glFlush();
    glDisable(GL_TEXTURE_2D);
}

void buildWindow(Texture* texture)
{
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    
    glPushMatrix();

        glTranslatef(0.0, 0.0, -0.3);

        glNormal3fv(getNormal({ 8.0, 0.0, 0.0 }, { 0.0, 5.0, 0.0 }));
        glBegin(GL_QUADS);

        glTexCoord2f(0.0, 1.0); glVertex3f(6.0, 3.0, -19.5);
        glTexCoord2f(1.0, 1.0); glVertex3f(14.0, 3.0, -19.5);
        glTexCoord2f(1.0, 0.0); glVertex3f(14.0, 8.0, -19.5);
        glTexCoord2f(0.0, 0.0); glVertex3f(6.0, 8.0, -19.5);

        glEnd();

        glFlush();
        glDisable(GL_TEXTURE_2D);

        // Left Border ...........................................................................

        glNormal3fv(getNormal({ 0.0, 5.0, 0.0 }, { 0.0, 0.0, 0.5 }));
        glBegin(GL_QUADS);
        glColor3f(0.647, 0.651, 0.631);

        glTexCoord2f(0.0, 1.0); glVertex3f(6.0, 3.0, -20.0);
        glTexCoord2f(1.0, 1.0); glVertex3f(6.0, 8.0, -20.0);
        glTexCoord2f(1.0, 0.0); glVertex3f(6.0, 8.0, -19.5);
        glTexCoord2f(0.0, 0.0); glVertex3f(6.0, 3.0, -19.5);

        glEnd();
        glFlush();

        // Right Border ..........................................................................

        glNormal3fv(getNormal({ 0.0, 5.0, 0.0 }, { 0.0, 0.0, 0.5 }));
        glBegin(GL_QUADS);
        glColor3f(0.647, 0.651, 0.631);

        glTexCoord2f(0.0, 1.0); glVertex3f(14.0, 3.0, -20.0);
        glTexCoord2f(1.0, 1.0); glVertex3f(14.0, 8.0, -20.0);
        glTexCoord2f(1.0, 0.0); glVertex3f(14.0, 8.0, -19.5);
        glTexCoord2f(0.0, 0.0); glVertex3f(14.0, 3.0, -19.5);

        glEnd();
        glFlush();

        // Top Border ............................................................................

        glNormal3fv(getNormal({ 0.0, 0.0, 0.5 }, { 8.0, 0.0, 0.0 }));
        glBegin(GL_QUADS);
        glColor3f(0.647, 0.651, 0.631);

        glTexCoord2f(0.0, 1.0); glVertex3f(6.0, 8.0, -19.5);
        glTexCoord2f(1.0, 1.0); glVertex3f(6.0, 8.0, -20.0);
        glTexCoord2f(1.0, 0.0); glVertex3f(14.0, 8.0, -20.0);
        glTexCoord2f(0.0, 0.0); glVertex3f(14.0, 8.0, -19.5);

        glEnd();
        glFlush();

        // Bottom Border .........................................................................

        glNormal3fv(getNormal({ 0.0, 0.0, 0.5 }, { 8.0, 0.0, 0.0 }));
        glBegin(GL_QUADS);
        glColor3f(0.647, 0.651, 0.631);

        glTexCoord2f(0.0, 1.0); glVertex3f(6.0, 3.0, -19.5);
        glTexCoord2f(1.0, 1.0); glVertex3f(6.0, 3.0, -20.0);
        glTexCoord2f(1.0, 0.0); glVertex3f(14.0, 3.0, -20.0);
        glTexCoord2f(0.0, 0.0); glVertex3f(14.0, 3.0, -19.5);

        glEnd();
        glFlush();

    glPopMatrix();
}