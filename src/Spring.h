#ifndef SPRING_H
#define SPRING_H

#include "givr.h"
#include <glm/gtc/matrix_transform.hpp>

class Spring
{
public:
    Spring(int index1, int index2);

    int mass1Index;
    int mass2Index;
};

#endif // SPRING_H
