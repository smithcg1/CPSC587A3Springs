#include "Spring.h"

Spring::Spring(int index1, int index2, float l)
{
    mass1Index = index1;
    mass2Index = index2;

    restDistance = l;
}

