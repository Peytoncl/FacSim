#ifndef GRAPHICS_H

#define WINDOW_W 800
#define WINDOW_H 600

#ifndef GAME_MATH

#include "gameMath.h"

#endif

typedef struct 
{
    float x;
    float y;
} Vector2;

void drawCircle(GLfloat x, GLfloat y, GLfloat radius, float r, float g, float b)
{
    glColor3f(r, g, b);

    int numSegments = 100; // Increase for a smoother circle
    float angle;
    
    //glBegin(GL_LINE_LOOP); Use GL_LINE_LOOP for a hollow circle, or GL_POLYGON for a filled one

    for (int i = 0; i < numSegments; i++) {
        angle = 2.0f * PI * i / numSegments;
        float xPos = x + (cos(angle) * radius);
        float yPos = y + (sin(angle) * radius);
        glVertex2f(xPos, yPos);
    }

}

bool areCirclesOverlapping(float cx1, float cy1, float r1, float cx2, float cy2, float r2) {
    float dx = cx2 - cx1;
    float dy = cy2 - cy1;
    float distanceSquared = dx * dx + dy * dy;

    float radiiSum = r1 + r2;
    float radiiSumSquared = radiiSum * radiiSum;

    return distanceSquared <= radiiSumSquared;
}

bool isPointInCircle(float px, float py, float cx, float cy, float radius) {
    float dx = px - cx;
    float dy = py - cy;
    float distanceSquared = dx * dx + dy * dy;
    
    return distanceSquared <= radius * radius;
}

void drawText(GLfloat x, GLfloat y, int font, char *string, float r, float g, float b)
{
  glColor3f(r, g, b);
  glRasterPos2f(x, y);

  int len, i;
  len = (int)strlen(string);

  for (i = 0; i < len; i++) 
  {
    glutBitmapCharacter(font, string[i]);
  }
}

#endif