#ifndef UTILS_H
#define UTILS_H

#include <GL/glut.h>
#include <vector>

float* getColor(int R, int G, int B);

void drawCrosshair(int windowsWidth, int windowsHeight);

void drawAxis(bool drawXAxis, bool drawYAxis, bool drawZAxis);

std::vector<float> getSpherePositions(float x0, float y0, float z0, float radius, float xAngle, float yAngle);

#endif