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

//spritesheet size is 2048 and each sprite is 32
float spriteWidth = 32.0f / 2048.0f; 
float spriteHeight = 32.0f / 2048.0f;

GLuint LoadTexture(const char* filename) //Load an image file with stb_image.h
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

void RenderSprite(int blockID, float scale, ScreenPosition pos) 
{
    int col = blockID % 8;
    int row = blockID / 8;

    float uMin = col * spriteWidth;
    float vMin = row * spriteHeight;
    float uMax = uMin + spriteWidth;
    float vMax = vMin + spriteHeight;

    glBegin(GL_QUADS);
        glTexCoord2f(uMin, vMax); glVertex2f(-scale + pos.x, -scale + pos.y); 
        glTexCoord2f(uMax, vMax); glVertex2f(scale + pos.x, -scale + pos.y);
        glTexCoord2f(uMax, vMin); glVertex2f(scale + pos.x, scale + pos.y);
        glTexCoord2f(uMin, vMin); glVertex2f(-scale + pos.x, scale + pos.y); 
    glEnd();
}

/*
### GRAPHICS ###
*/

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