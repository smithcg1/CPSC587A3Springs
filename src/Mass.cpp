#include "Mass.h"

Mass::Mass()
{
    location = glm::vec3(0.0, 0.0, 0.0);
}


void Mass::setLocation(glm::vec3 newLocation){
    location = newLocation;
}

