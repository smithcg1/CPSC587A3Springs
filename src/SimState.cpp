#include "SimState.h"

SimState::SimState()
{
    scene1Setup();
}

void SimState::removeOldScene(){
    masses.clear();
    springs.clear();
}

void SimState::scene1Setup(){
    //removeOldScene();
    numMasses = 2;
    simsPerFrame = 16;

    deltT = 0.001;
    k = 200;
    b = 10;
    l = 10;

    createMasses();
    createSprings();

    float strech = 2.2f;
    masses[0].location = vec3(0.0f, planeHight, 0.0f);
    masses[0].dynamic = false;
    masses[1].location = vec3(0.0f, planeHight-(strech*l), 0.0f);
}

void SimState::scene2Setup(){
    removeOldScene();
    numMasses = 10;
    simsPerFrame = 512;

    deltT = 0.00005;
    k = 800;
    b = 10;
    l = 2;

    createMasses();
    createSprings();

    masses[0].location = vec3(0.0f, planeHight, 0.0f);
    masses[0].dynamic = false;
    for( float i = 1; i < masses.size() ; i++){
        masses[i].location = vec3(l*i, planeHight, 0.0f);
    }
}



void SimState::createMasses(){
    //Create masses
    for(int i = 0 ; i < numMasses ; i++){
        Mass newMass = Mass();
        masses.push_back(newMass);
        std::cout << "Mass created" << std::endl;
    }
}

void SimState::createSprings(){
    //Create springs
    for(int i = 0 ; i < (numMasses-1) ; i++){
        Spring newSpring = Spring(i, i+1);
        springs.push_back(newSpring);
        std::cout << "Spring created" << std::endl;
    }
}
