#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PI 3.14159265358979323846

//Initialize random seed based on the current time
void RandomInitialization()
{
    srand(time(NULL));
}

// Returns a random int with "min" being the lowest and "max" being the highest
int RandomInt(int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}

//Generate random 32 bit seed for world
uint32_t GenerateSeed() {
    return ((uint32_t)rand() << 16) | (uint32_t)rand();
}