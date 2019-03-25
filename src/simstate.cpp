#include "simstate.h"

SimState::SimState()
{
    void removeOldScene(){
        masses.clear();
        springs.clear();
    }

    void scene1Setup(){
        removeOldScene();
        numMasses = 2;
        simsPerFrame = 16;

        deltT = 0.001;
        k = 200;
        b = 10;
        l = 10;
    }

    void scene2Setup(){
        removeOldScene();
        numMasses = 10;
        simsPerFrame = 512;

        deltT = 0.00005;
        k = 800;
        b = 10;
        l = 2;
    }
}
