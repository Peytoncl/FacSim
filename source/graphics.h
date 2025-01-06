#ifndef GRAPHICS_H

#define STB_IMAGE_IMPLEMENTATION
#include "external_headers/stb_image.h"

#define WINDOW_W 800
#define WINDOW_H 600

#ifndef GAME_MATH

#include "gameMath.h"

#endif

typedef struct 
{
    float x;
    float y;
} ScreenPosition;


/*
### SPRITES ###
*/

float spriteWidth = 32.0f / 2048.0f;
float spriteHeight = 32.0f / 2048.0f;

GLuint LoadTexture(const char* filename) 
{
    int width, height, channels;

    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
    if (!data) 
    {
        printf("Failed to load image: %s\n", filename);
        return 0;
    }

    GLuint textureID;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(data); 
    return textureID;
}

void RenderSprite(int col, int row, float scale, ScreenPosition pos) 
{
    float uMin = col * spriteWidth;
    float vMin = row * spriteHeight;
    float uMax = uMin + spriteWidth;
    float vMax = vMin + spriteHeight;

    glBegin(GL_QUADS);
        glTexCoord2f(uMin, vMax); glVertex2f(-scale + pos.x, -scale + pos.y); // Bottom-left (flipped)
        glTexCoord2f(uMax, vMax); glVertex2f(scale + pos.x, -scale + pos.y); // Bottom-right (flipped)
        glTexCoord2f(uMax, vMin); glVertex2f(scale + pos.x, scale + pos.y); // Top-right (flipped)
        glTexCoord2f(uMin, vMin); glVertex2f(-scale + pos.x, scale + pos.y); // Top-left (flipped)
    glEnd();
}

/*
### GRAPHICS ###
*/

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

bool areCirclesOverlapping(float cx1, float cy1, float r1, float cx2, float cy2, float r2) 
{
    float dx = cx2 - cx1;
    float dy = cy2 - cy1;
    float distanceSquared = dx * dx + dy * dy;

    float radiiSum = r1 + r2;
    float radiiSumSquared = radiiSum * radiiSum;

    return distanceSquared <= radiiSumSquared;
}

bool isPointInCircle(float px, float py, float cx, float cy, float radius) 
{
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