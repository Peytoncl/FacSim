#ifndef COLORS

//RGB Colors
typedef struct 
{
    float r;
    float g;
    float b;
} Color;

#define BLACK (Color){0, 0, 0}

//Normalize color to be in between 0-1
Color NormalizeColor(Color inColor)
{
    Color outColor = {inColor.r / 255, inColor.g / 255, inColor.b / 255};

    return outColor;
}

//Denormalize color to be in between 0-255
Color DeNormalizeColor(Color inColor)
{
    Color outColor = {inColor.r * 255, inColor.g * 255, inColor.b * 255};

    return outColor;
}

#endif // !COLORS