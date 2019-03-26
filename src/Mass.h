#ifndef MASS_H
#define MASS_H

#include "givr.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace givr;
using namespace givr::camera;
using namespace givr::geometry;
using namespace givr::style;

class Mass
{
public:
    Mass();
    void setLocation(glm::vec3 newLocation);

    bool dynamic = true;


    float weight = 10.0f;
    vec3 location = vec3(0.0f, 0.0f, 0.0f);
    vec3 velocity = vec3(0.0f, 0.0f, 0.0f);
    vec3 totalForce = vec3(0.0f, 0.0f, 0.0f);
};

#endif // MASS_H
