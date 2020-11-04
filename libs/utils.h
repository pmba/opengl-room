#ifndef UTILS_H
#define UTILS_H

#include <GL/glut.h>
#include <vector>

float* getColor(int R, int G, int B);

void drawCrosshair(int windowsWidth, int windowsHeight);

void drawAxis(bool drawXAxis, bool drawYAxis, bool drawZAxis, 
    float at[3] = new float[3] { 0.0, 0.0, 0.0 },
    float size = 5.0);

#endif