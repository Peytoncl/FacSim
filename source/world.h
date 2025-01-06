#ifndef WORLD_H

#include "graphics.h"

typedef struct 
{
    int x;
    int y;
    int z;
} WorldPosition;

ScreenPosition WorldToScreenPosition(WorldPosition position, float zoom)
{
    float xPos = zoom * (float)position.z + (zoom) * (float)position.x;
    float yPos = (zoom / 2) * (float)position.z - (zoom / 2) * (float)position.x + (zoom) * position.y;

    ScreenPosition screenPos;
    
    screenPos.x = xPos;
    screenPos.y = yPos;

    return screenPos;
}

#endif