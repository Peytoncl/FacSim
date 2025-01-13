#include <GL/glut.h>

#include <stdio.h>
#include "stdbool.h"
#include <stdlib.h> 
#include <math.h>
#include "unistd.h"
#include "string.h"

#include "windows.h"

#include "world.h"
#include "game.h"

//Key input setup
#define KEY_COUNT 256 

bool keys[KEY_COUNT];

//Game values (non customizable)
float CENTER_X = (WINDOW_W / 2);
float CENTER_Y = (WINDOW_H / 2);

ScreenPosition mousePos = {0, 0};

ScreenPosition cameraPosition = {0, 0};
WorldPosition cameraWorldPosition = {0, 0, 0};

float zoom = 0.045f;

GLuint textureID; //Spritesheet textureId in GPU

Settings settings = {2}; //Render distance of 2 

Player player;

World world;


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

  InitializeWorld(&world); //Initialize the world (gonna add loading world)


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
 
  for (int i = world.loadedChunkCount - 1; i >= 0; i--)
  {
    RenderChunk(world.loadedChunks[i], cameraPosition, zoom, cameraWorldPosition);
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

float cameraSpeedMultiplier = 0.003f; //temporary variable just used as camera speed

ChunkPosition playerCurrentChunk;

bool firstChunkLoad = false;

void update()
{
  //printf("X: %d, Y: %d, Z: %d\n", cameraWorldPosition->x * 8, 0, cameraWorldPosition->z * 8);

  WorldPosition mouseWorldPos = ScreenToWorldPosition(world, mousePos, zoom, cameraPosition, 0.0f);

  if (mouseWorldPos.y != -1) cameraWorldPosition = mouseWorldPos;

  int chunkX = floorf(cameraWorldPosition.x / CHUNK_WIDTH);
  int chunkZ = floorf(cameraWorldPosition.z / CHUNK_LENGTH);

  if (playerCurrentChunk.x != chunkX || playerCurrentChunk.y != chunkZ || !firstChunkLoad)
  {
    playerCurrentChunk.x = chunkX;
    playerCurrentChunk.y = chunkZ;

    if(!firstChunkLoad) UpdateChunks(&world, cameraWorldPosition.x, cameraWorldPosition.z, settings.renderDistance);

    firstChunkLoad = true;
  }

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

void mouseMove(int x, int y)
{
  float ndc_x = (float)x / WINDOW_W * 2 - 1;
  float ndc_y = (float)y / WINDOW_H * 2 - 1;

  mousePos.x = ndc_x;
  mousePos.y = -ndc_y;

  //printf("%d, %d\n", mousePos.x, mousePos.y);
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

  glutPassiveMotionFunc(mouseMove);

  glutMainLoop();      
  return 0;   
}