#ifndef SPRING_H
#define SPRING_H

#include "givr.h"
#include <glm/gtc/matrix_transform.hpp>

class Spring
{
public:
    Spring(int index1, int index2, float l);

    int mass1Index;
    int mass2Index;

    float restDistance;
};

#endif // SPRING_H
