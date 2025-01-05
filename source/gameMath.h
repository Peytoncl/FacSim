#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PI 3.14159265358979323846

unsigned int seed;

//Initialize random seed based on the current time
void RandomInitialization()
{
    seed = time(NULL);
    srand(seed);
}

// Returns a pseudo-random int with "min" being the lowest and "max" being the highest
int RandomInt(int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}