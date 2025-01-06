#include <GL/glut.h>

#include <stdio.h>
#include "stdbool.h"
#include <stdlib.h> 
#include <math.h>
#include "unistd.h"
#include "string.h"

#include "windows.h"

#include "world.h"

//Key input setup
#define KEY_COUNT 256 

bool keys[KEY_COUNT];

//Game values (non customizable)
float CENTER_X = (WINDOW_W / 2);
float CENTER_Y = (WINDOW_H / 2);

float zoom = 0.1f;

GLuint textureID; //Spritesheet textureId in GPU

void init()
{
  glClearColor(0, 0, 0, 0);

  char *buffer = (char *)malloc(1024);

  if (getcwd(buffer, 1024) != NULL) 
  {
    textureID = LoadTexture(strcat(buffer, "/images/spritesheet.png"));
  } 
  else 
  {
    perror("getcwd");
  }

  free(buffer); 

  glMatrixMode(GL_PROJECTION);

  glLoadIdentity();

  glEnable(GL_TEXTURE_2D);

  glEnable(GL_BLEND);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  gluOrtho2D(-1.0, 1.0, -1.0, 1.0);  // Adjust as needed

  glMatrixMode(GL_MODELVIEW);
}

void windowClosed()
{

  glDisable(GL_BLEND);

  glDisable(GL_TEXTURE_2D);
}

void display() 
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  glBindTexture(GL_TEXTURE_2D, textureID);

  RenderSprite(1, 0, zoom, (ScreenPosition)WorldToScreenPosition((WorldPosition){0, 0, 1}, zoom));

  RenderSprite(1, 0, zoom, (ScreenPosition)WorldToScreenPosition((WorldPosition){1, 0, 1}, zoom));

  RenderSprite(1, 0, zoom, (ScreenPosition){0, 0, 0});

  RenderSprite(1, 0, zoom, (ScreenPosition)WorldToScreenPosition((WorldPosition){1, 0, 0}, zoom));

  RenderSprite(1, 0, zoom, (ScreenPosition)WorldToScreenPosition((WorldPosition){2, 0, 1}, zoom));

  RenderSprite(1, 0, zoom, (ScreenPosition)WorldToScreenPosition((WorldPosition){2, 0, 0}, zoom));

  RenderSprite(1, 0, zoom, (ScreenPosition)WorldToScreenPosition((WorldPosition){3, 0, 0}, zoom));

  RenderSprite(1, 0, zoom, (ScreenPosition)WorldToScreenPosition((WorldPosition){0, 1, 1}, zoom));

  RenderSprite(1, 0, zoom, (ScreenPosition)WorldToScreenPosition((WorldPosition){1, 1, 1}, zoom));

  RenderSprite(1, 0, zoom, (ScreenPosition)WorldToScreenPosition((WorldPosition){2, 1, 0}, zoom));

  RenderSprite(1, 0, zoom, (ScreenPosition)WorldToScreenPosition((WorldPosition){0, 2, 1}, zoom));

  glutSwapBuffers();
}

void keyDown(unsigned char key, int x, int y) 
{
  keys[key] = true; 
}

void keyUp(unsigned char key, int x, int y) 
{
  keys[key] = false; 
}

void update()
{



  glutPostRedisplay();
}

void windowResize(int width, int height)
{
  glutReshapeWindow(WINDOW_W, WINDOW_H);
}

int main(int argc, char** argv) 
{
  glutInit(&argc, argv);    

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);        
  glutInitWindowSize(WINDOW_W, WINDOW_H);        
    
  glutCreateWindow("Isometric");  

  init();

  for (int i = 0; i < KEY_COUNT; i++) 
  {
    keys[i] = false;
  }

  glutDisplayFunc(display); 
  glutIdleFunc(update);      

  atexit(windowClosed);

  glutKeyboardFunc(keyDown);  
  glutKeyboardUpFunc(keyUp);  

  glutReshapeFunc(windowResize);

  glutMainLoop();      
  return 0;   
}