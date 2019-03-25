#ifndef SIMSTATE_H
#define SIMSTATE_H


class SimState
{
public:
    SimState();

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
};

#endif // SIMSTATE_H
