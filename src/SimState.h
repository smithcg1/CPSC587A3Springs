#ifndef SIMSTATE_H
#define SIMSTATE_H
#include "Mass.h"
#include "Spring.h"

class SimState
{
public:
    SimState();

    void removeOldScene();
    void scene1Setup();
    void scene2Setup();
    void scene3Setup();
    void scene4Setup();
    void scene5Setup();
    void scene6Setup();

    void createMasses();
    void create1DSprings();
    void create2DSprings(int width);
    void create3DSprings(int width, int length);

    int numMasses;              //Number of masses in the system
    int simsPerFrame;
    std::vector<Mass> masses;
    std::vector<Spring> springs;

    float deltT;                //Time step
    float k;                    //Spring constant
    float b;                    //Resistance
    float l;                    //Rest spring distance
    const glm::vec3 g = glm::vec3(0.0f, -9.81f, 0.0f);


    float planeSize = 10.0;
    float planeHight = 10.0;
    float tableHight = -10.0;


    int sceneObjWidth;
    int scene;
};

#endif // SIMSTATE_H
