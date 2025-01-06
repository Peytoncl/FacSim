#include <GL/glut.h>

#include <stdio.h>
#include "stdbool.h"
#include <stdlib.h> 
#include <math.h>
#include "unistd.h"
#include "string.h"

#include "windows.h"

#include "graphics.h"

//Key input setup
#define KEY_COUNT 256 

bool keys[KEY_COUNT];

//Game values (non customizable)
float CENTER_X = (WINDOW_W / 2);
float CENTER_Y = (WINDOW_H / 2);

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

  gluOrtho2D(-1.0, 1.0, -1.0, 1.0);  // Adjust as needed

  glMatrixMode(GL_MODELVIEW);
}

void display() 
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  glEnable(GL_TEXTURE_2D);


  glBindTexture(GL_TEXTURE_2D, textureID);


  RenderSprite(1, 0, (ScreenPosition){0, 0});

  RenderSprite(1, 0, (ScreenPosition){0.1f, 0});


  glDisable(GL_TEXTURE_2D);


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

  glutKeyboardFunc(keyDown);  
  glutKeyboardUpFunc(keyUp);  

  glutReshapeFunc(windowResize);

  glutMainLoop();      
  return 0;   
}