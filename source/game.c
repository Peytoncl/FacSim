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

ScreenPosition cameraPosition = (ScreenPosition){0, 0};

float zoom = 0.05f;

GLuint textureID; //Spritesheet textureId in GPU

Chunk chunk;

Chunk chunk1;

void init()
{
  glClearColor(0, 0, 0, 0); //make background black

  char *buffer = (char*)malloc(1024);

  if (getcwd(buffer, 1024) != NULL) 
  {
    textureID = LoadTexture(strcat(buffer, "/images/spritesheet.png")); //Load spritesheet texture buffer to memory
  } 
  else 
  {
    perror("getcwd");
  }

  free(buffer); 


  RandomInitialization(); //Initialize math randomization

  InitializeWorld(); //Initialize the world (gonna add loading world)


  chunk = LoadChunk(chunk, (ChunkPosition){0, 0}); //Generate chunk 1

  chunk1 = LoadChunk(chunk1, (ChunkPosition){1, 0}); //Generate chunk 2


  glMatrixMode(GL_PROJECTION);

  glLoadIdentity();

  glEnable(GL_TEXTURE_2D);

  glEnable(GL_BLEND);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  gluOrtho2D(-1.0, 1.0, -1.0, 1.0); 

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

  glBindTexture(GL_TEXTURE_2D, textureID); //bind texture to future texture calls in this display update

  //RenderSprite(1, zoom, (ScreenPosition)WorldToScreenPosition((WorldPosition){0, 0, 1}, zoom));

  //gonna use a World variable later to loop through loaded chunks

  //Load chunk 1

  for (int y = 0; y < 16; y++)
  {
    for (int x = 0; x < 16; x++)
    {
      for (int z = 15; z >= 0; z--)
      {
      
        Block block = chunk.blocks[x][y][z];

        if (block.blockID == 0) continue;

        int chunkX = chunk.position.x * CHUNK_LENGTH;
        int chunkZ = chunk.position.y * CHUNK_WIDTH;

        RenderSprite(block.blockID, zoom, (ScreenPosition)WorldToScreenPosition((WorldPosition){x + cameraPosition.x + chunkX, y, z + cameraPosition.y + chunkZ}, zoom)); //Rendering blockId with (zoom) scale, and a world position casted to a screen position
      
      }
    }
  }

  //Load chunk 2

  for (int y = 0; y < 16; y++)
  {
    for (int x = 0; x < 16; x++)
    {
      for (int z = 15; z >= 0; z--)
      {
      
        Block block = chunk1.blocks[x][y][z];

        if (block.blockID == 0) continue;

        int chunkX = chunk1.position.x * CHUNK_LENGTH;
        int chunkZ = chunk1.position.y * CHUNK_WIDTH;

        RenderSprite(block.blockID, zoom, (ScreenPosition)WorldToScreenPosition((WorldPosition){x + cameraPosition.x + chunkX, y, z + cameraPosition.y + chunkZ}, zoom)); //Rendering blockId with (zoom) scale, and a world position casted to a screen position
      
      }
    }
  }

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

float cameraSpeedMultiplier = 0.001f; //temporary variable just used as camera speed

void update()
{
  //move camera gonna make better later

  if (keys['w'])
  {
    cameraPosition.y -= 1 * cameraSpeedMultiplier;
  }
  if (keys['s'])
  {
    cameraPosition.y += 1 * cameraSpeedMultiplier;
  }
  if (keys['a'])
  {
    cameraPosition.x += 1 * cameraSpeedMultiplier;
  }
  if (keys['d'])
  {
    cameraPosition.x -= 1 * cameraSpeedMultiplier;
  }

  glutPostRedisplay();
}

void windowResize(int width, int height)
{
  glutReshapeWindow(WINDOW_W, WINDOW_H); //lock resizing window for now
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