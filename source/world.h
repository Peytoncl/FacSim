#ifndef WORLD_H

#define WORLD_H

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

float centerThreshold = 10;

ScreenPosition WorldToScreenPosition(WorldPosition position, float zoom, ScreenPosition cameraPos)
{
    //really tedius calculation i had to find out after a bunch of trial and error and adjusting each value number by number
    float xPos = zoom * (float)position.z + (zoom) * (float)position.x;
    float yPos = (zoom / 2) * (float)position.z - (zoom / 2) * (float)position.x + (zoom) * position.y;

    //int worldX = (int)roundf((float)xPos / (float)centerThreshold);
    //int worldY = (int)roundf((float)yPos / (float)centerThreshold);

    ScreenPosition screenPos;
    
    screenPos.x = xPos + cameraPos.x;
    screenPos.y = yPos + cameraPos.y;

    return screenPos;
}

WorldPosition ScreenToWorldPosition(ScreenPosition position, float zoom, ScreenPosition cameraPos)
{
    float realPosX = position.x + cameraPos.x; //might have to change to negatives
    float realPosY = position.y + cameraPos.y;

    float offsetPosX;
    float offsetPosY; //need to finish this function
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
#define CHUNK_HEIGHT 128
#define CHUNK_LENGTH 16

typedef enum {
    PLAINS,
    DESERT,
    BEACH,
    FOREST,
    MOUNTAINS,
    OCEAN
} Biome;

typedef struct 
{
    Biome biomeMap[CHUNK_WIDTH][CHUNK_LENGTH];
    Block blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_LENGTH];
    ChunkPosition position;
} Chunk;

#define MAX_CHUNKS 64

typedef struct
{
    Chunk* loadedChunks[MAX_CHUNKS];

    int loadedChunkCount;

    uint32_t seed; //Seed needs to go here eventually
} World;

float seed;

float noiseScale = 0.1f; 

float maxHeight = 11; 

float groundHeight = 50;

float mountainPeakHeight = CHUNK_HEIGHT - CHUNK_HEIGHT / 4;

float waterHeight = 5;


float biomeScale = 0.003f;

void InitializeWorld(World* world)
{
    seed = GenerateSeed() * 0.0001f; //generate world seed, gonna add loading files later

    world->seed = seed;

    for (int i = 0; i < MAX_CHUNKS; i++)
    {

    }

    printf("World Seed: %f\n", seed);
}

Biome GetBiome(Chunk* chunk, WorldPosition position)
{
    int x = (chunk->position.x * CHUNK_WIDTH) + position.x;
    int z = (chunk->position.y * CHUNK_LENGTH) + position.z;

    float noiseValue = stb_perlin_noise3(x * biomeScale + seed, 0, z * biomeScale + seed, 0, 0, 0);

    if (noiseValue > 0.8f) 
    {
        chunk->biomeMap[position.x][position.z] = OCEAN;
    } 
    else if (noiseValue > 0.6f) 
    {
        chunk->biomeMap[position.x][position.z] = BEACH;
    } 
    else if (noiseValue > 0.4f) 
    {
        chunk->biomeMap[position.x][position.z] = PLAINS;
    } 
    else if (noiseValue > 0.2f) 
    {
        chunk->biomeMap[position.x][position.z] = MOUNTAINS;
    } 
    else if (noiseValue > 0.0f)
    {
        chunk->biomeMap[position.x][position.z] = DESERT;
    }
    else if (noiseValue > -0.2f)
    {
        chunk->biomeMap[position.x][position.z] = PLAINS;
    }
    else if (noiseValue > -0.4f)
    {
        chunk->biomeMap[position.x][position.z] = MOUNTAINS;
    }
    else if (noiseValue > -0.6f)
    {
        chunk->biomeMap[position.x][position.z] = DESERT;
    }
    else if (noiseValue > -0.8f)
    {
        chunk->biomeMap[position.x][position.z] = BEACH;
    }
    else
    {
        chunk->biomeMap[position.x][position.z] = OCEAN;
    }



    //printf("%d\n", chunk->biomeMap[position.x][position.z]);
}

Chunk* FindChunk(World* world, int chunkX, int chunkY)
{
    for (int i = 0; i < world->loadedChunkCount; i++)
    {
        if (world->loadedChunks[i]->position.x == chunkX && world->loadedChunks[i]->position.y == chunkY)
        {
            return world->loadedChunks[i];
        }
    }

    return NULL;
}

Chunk* LoadChunk(World* world, ChunkPosition position)
{
    Chunk* chunk = calloc(1, sizeof(Chunk));

    chunk->position = position;

    int chunkX = position.x * CHUNK_LENGTH;
    int chunkZ = position.y * CHUNK_WIDTH;

    for (int x = 0; x < CHUNK_WIDTH; x++)
    {
        for (int z = 0; z < CHUNK_LENGTH; z++)
        {
            GetBiome(chunk, (WorldPosition){x, 0, z});

            float rawNoise = stb_perlin_noise3((x + chunkX) * noiseScale + seed, 0.0f, (z + chunkZ) * noiseScale + seed, 0, 0, 0); //use stb_perlin.h for perlin noise value at x and z positions with a chunk and seed offset and noise multiplier

            float normalizedNoise = (rawNoise + 1.0f) * 0.5f;

            float minValue = 0;
            if (normalizedNoise < minValue) //no negatives allowed for now
            {
                //normalizedNoise = minValue;
            }

            int height = (int)(normalizedNoise * maxHeight) + groundHeight; //get int height

            Block block;

            switch (chunk->biomeMap[x][z]) 
            {
                case MOUNTAINS: block.blockID = 5; break;
                case DESERT: case BEACH: block.blockID = 4; break;
                case OCEAN: block.blockID = 3; block.metadata = 1; break;
                default: block.blockID = 1; break;
            }

            if (height < 0) height = 0;
            if (height > CHUNK_HEIGHT) height = CHUNK_HEIGHT;

            /*
            if (chunk.biomeMap[x][z] == OCEAN)
            {
                for (int i = height + 1; i <= height + 20; i++)
                {
                    chunk.blocks[x][i][z] = (Block){3, 1}; 
                }
            }*/

            //printf("X: %d, Y: %d, Z: %d\n", x, height, z);

            chunk->blocks[x][height][z] = block;

        }
    }

    world->loadedChunks[world->loadedChunkCount++] = chunk;

    return chunk;
}

void UnloadChunk(World* world, int index)
{
    if (index < 0 || index >= world->loadedChunkCount) return;

    Chunk* chunk = world->loadedChunks[index];

    free(chunk);

    for (int i = index; i < world->loadedChunkCount - 1; i++)
    {
        world->loadedChunks[i] = world->loadedChunks[i + 1];
    }

    world->loadedChunkCount--;
}

void UpdateChunks(World* world, int playerX, int playerZ, int renderDistance)
{
    int playerChunkX = playerX / CHUNK_WIDTH;
    int playerChunkZ = playerZ / CHUNK_LENGTH;

    for (int distanceX = -renderDistance; distanceX <= renderDistance; distanceX++) //Load chunks that are in the render distance
    {
        for (int distanceZ = -renderDistance; distanceZ <= renderDistance; distanceZ++)
        {
            int chunkX = playerChunkX + distanceX;
            int chunkZ = playerChunkZ + distanceZ;

            if (!FindChunk(world, chunkX, chunkZ))
            {
                LoadChunk(world, (ChunkPosition){chunkX, chunkZ});
            }

        }
    }

    for (int i = 0; i < world->loadedChunkCount; i++) //Unload chunks that are out of distance
    {
        Chunk* chunk = world->loadedChunks[i];

        int distanceX = abs(chunk->position.x - playerChunkX);
        int distanceZ = abs(chunk->position.y - playerChunkZ);

        if (distanceX > renderDistance || distanceZ > renderDistance)
        {
            UnloadChunk(world, i);

            i--;
        }
    }

}

void RenderChunk(Chunk* chunk, ScreenPosition cameraPosition, float zoom)
{
    for (int y = 0; y < CHUNK_HEIGHT; y++)
    {
        for (int x = 0; x < CHUNK_WIDTH; x++)
        {
            for (int z = CHUNK_LENGTH - 1; z >= 0; z--)
            {
      
                Block block = chunk->blocks[x][y][z];

                if (block.blockID == 0) continue;

                int chunkX = chunk->position.x * CHUNK_LENGTH;
                int chunkZ = chunk->position.y * CHUNK_WIDTH;

                if (block.metadata == 1 && chunk->blocks[x][y + 1][z].blockID == block.blockID) continue;

                RenderSprite(block.blockID, zoom, (ScreenPosition)WorldToScreenPosition((WorldPosition){x + chunkX, y, z + chunkZ}, zoom, cameraPosition)); //Rendering blockId with (zoom) scale, and a world position casted to a screen position

            }
        }
    }
}

#endif