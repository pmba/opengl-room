#pragma once

#include <GL/gl.h>

class Material {
    private:
    GLfloat ambient_r;
    GLfloat ambient_g;
    GLfloat ambient_b;

    GLfloat diffuse_r;
    GLfloat diffuse_g;
    GLfloat diffuse_b;

    GLfloat specular_r;
    GLfloat specular_g;
    GLfloat specular_b;

    GLfloat high_shininess;


    public:

    Material(GLfloat ar, GLfloat ag, GLfloat ab, GLfloat dr, GLfloat dg, GLfloat db, GLfloat sr, GLfloat sg, GLfloat sb, GLfloat shininess){
        ambient_r=ar;   ambient_g=ag;   ambient_b=ab;
        diffuse_r=dr;   diffuse_g=dg;   diffuse_b=db;
        specular_r=sr;  specular_g=sg;  specular_b=sb;
        high_shininess=shininess;
    }

    void active(){
        GLfloat ambient[]={ambient_r, ambient_g, ambient_b, 1.0};
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        GLfloat diffuse[]={diffuse_r, diffuse_g, diffuse_b};
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        GLfloat specular[]={specular_r, specular_g, specular_b};
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialf(GL_FRONT, GL_SHININESS, high_shininess * 128);
    }
};