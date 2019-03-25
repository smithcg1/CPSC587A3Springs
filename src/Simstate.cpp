#include "Simstate.h"

SimState::SimState()
{
    scene1Setup();
}

void SimState::removeOldScene(){
    masses.clear();
    springs.clear();
}

void SimState::scene1Setup(){
    removeOldScene();
    numMasses = 2;
    simsPerFrame = 16;

    deltT = 0.001;
    k = 200;
    b = 10;
    l = 10;
}

void SimState::scene2Setup(){
    removeOldScene();
    numMasses = 10;
    simsPerFrame = 512;

    deltT = 0.00005;
    k = 800;
    b = 10;
    l = 2;
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
