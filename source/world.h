#ifndef WORLD_H

#include "graphics.h"

typedef struct 
{
    int x;
    int y;
    int z;
} WorldPosition;

typedef struct 
{
    int x;
    int y;
} ChunkPosition;

ScreenPosition WorldToScreenPosition(WorldPosition position, float zoom)
{
    //really tedius calculation i had to find out after a bunch of trial and error and adjusting each value number by number
    float xPos = zoom * (float)position.z + (zoom) * (float)position.x;
    float yPos = (zoom / 2) * (float)position.z - (zoom / 2) * (float)position.x + (zoom) * position.y;

    ScreenPosition screenPos;
    
    screenPos.x = xPos;
    screenPos.y = yPos;

    return screenPos;
}


// ### CHUNK SYSTEM ###

#define STB_PERLIN_IMPLEMENTATION
#include "external_headers/stb_perlin.h"

typedef struct 
{
    unsigned short blockID;
    unsigned char metadata;
} Block;


#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 16
#define CHUNK_LENGTH 16

typedef struct 
{
    Block blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_LENGTH];
    ChunkPosition position;
} Chunk;


#define WORLD_SIZE 256

typedef struct
{
    Chunk chunks[WORLD_SIZE];

    //Seed needs to go here eventually
} World;

float seed;

float noiseScale = 0.1f; 

float maxHeight = 10; 

void InitializeWorld()
{
    seed = GenerateSeed() * 0.001f; //generate world seed, gonna add loading files later

    printf("World Seed: %f\n", seed);
}

Chunk LoadChunk(Chunk chunk, ChunkPosition position)
{
    chunk.position = position;

    int chunkX = position.x * CHUNK_LENGTH;
    int chunkZ = position.y * CHUNK_WIDTH;

    for (int x = 0; x < 16; x++)
    {
        for (int z = 0; z < 16; z++)
        {
            float rawNoise = stb_perlin_noise3((x + chunkX) * noiseScale + seed, 0.0f, (z + chunkZ) * noiseScale + seed, 0, 0, 0); //use stb_perlin.h for perlin noise value at x and z positions with a chunk and seed offset and noise multiplier

            float normalizedNoise = (rawNoise + 1.0f) * 0.5f;

            float minValue = 0;
            if (normalizedNoise < minValue) //no negatives allowed for now
            {
                normalizedNoise = minValue;
            }

            int height = (int)(normalizedNoise * maxHeight); //get int height

            Block block = (Block){1, 0}; //grass block with no metadata

            chunk.blocks[x][height][z] = block; //apply noise height to the block at x and z
        }
    }

    return chunk;
}

#endif