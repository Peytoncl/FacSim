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

ScreenPosition WorldToScreenPosition(WorldPosition position, float zoom, ScreenPosition cameraPos)
{
    //really tedius calculation i had to find out after a bunch of trial and error and adjusting each value number by number
    float xPos = zoom * (float)position.z + (zoom) * (float)position.x;
    float yPos = (zoom / 2) * (float)position.z - (zoom / 2) * (float)position.x + (zoom) * position.y;

    ScreenPosition screenPos;
    
    screenPos.x = xPos + cameraPos.x;
    screenPos.y = yPos + cameraPos.y;

    return screenPos;
}


// ### CHUNK SYSTEM ###

#define STB_PERLIN_IMPLEMENTATION
#include "external_headers/stb_perlin.h"

typedef struct 
{
    unsigned short blockID;
    unsigned char metadata; //1 is transparent
} Block;


#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 32
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

float maxHeight = 11; 

float groundHeight = 8;

float waterHeight = 5;

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

    for (int x = 0; x < CHUNK_WIDTH; x++)
    {
        for (int z = 0; z < CHUNK_LENGTH; z++)
        {
            float rawNoise = stb_perlin_noise3((x + chunkX) * noiseScale + seed, 0.0f, (z + chunkZ) * noiseScale + seed, 0, 0, 0); //use stb_perlin.h for perlin noise value at x and z positions with a chunk and seed offset and noise multiplier

            float normalizedNoise = (rawNoise + 1.0f) * 0.5f;

            float minValue = 0;
            if (normalizedNoise < minValue) //no negatives allowed for now
            {
                //normalizedNoise = minValue;
            }

            int height = (int)(normalizedNoise * maxHeight) + groundHeight; //get int height

            Block block;

            printf("%d\n", height);

            if (height < waterHeight + groundHeight) //water
            {
                block = (Block){2, 0};

                for (int i = height + 1; i <= waterHeight + groundHeight; i++) //Set every block up to sea level to water
                {
                    Block waterBlock = (Block){3, 1}; //water with transparent meta data

                    chunk.blocks[x][i][z] = waterBlock;
                }
            }
            else block = (Block){1, 0}; //grass block with no metadata

            if (block.blockID == 1) //Apply dirt block underneath grass block
            {
                Block dirtBlock = (Block){2, 0};

                chunk.blocks[x][height - 1][z] = dirtBlock;
            }

            chunk.blocks[x][height][z] = block; //apply noise height to the block at x and z
        }
    }

    return chunk;
}

void RenderChunk(Chunk chunk, ScreenPosition cameraPosition, float zoom)
{
    for (int y = 0; y < CHUNK_HEIGHT; y++)
    {
        for (int x = 0; x < CHUNK_WIDTH; x++)
        {
            for (int z = CHUNK_LENGTH - 1; z >= 0; z--)
            {
      
                Block block = chunk.blocks[x][y][z];

                if (block.blockID == 0) continue;

                int chunkX = chunk.position.x * CHUNK_LENGTH;
                int chunkZ = chunk.position.y * CHUNK_WIDTH;

                if (block.metadata == 1 && chunk.blocks[x][y + 1][z].blockID == block.blockID) continue;

                RenderSprite(block.blockID, zoom, (ScreenPosition)WorldToScreenPosition((WorldPosition){x + chunkX, y, z + chunkZ}, zoom, cameraPosition)); //Rendering blockId with (zoom) scale, and a world position casted to a screen position

            }
        }
    }
}

#endif