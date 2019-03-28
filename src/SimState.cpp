#include "SimState.h"

SimState::SimState()
{
    scene5Setup();
}

void SimState::removeOldScene(){
    springs.clear();
    masses.clear();
}

void SimState::scene1Setup(){
    removeOldScene();

    scene = 1;
    numMasses = 2;
    simsPerFrame = 16;

    deltT = 0.001;
    k = 200;
    b = 10;
    l = 10;

    createMasses();
    create1DSprings();

    float strech = 2.2f;
    masses[0].location = vec3(0.0f, planeHight, 0.0f);
    masses[0].dynamic = false;
    masses[1].location = vec3(0.0f, planeHight-(strech*l), 0.0f);
}

void SimState::scene2Setup(){
    removeOldScene();

    scene = 2;
    numMasses = 10;
    simsPerFrame = 512;

    deltT = 0.00005;
    k = 800;
    b = 4;
    l = 2;

    createMasses();
    create1DSprings();

    masses[0].location = vec3(0.0f, planeHight, 0.0f);
    masses[0].dynamic = false;
    for( float i = 1; i < masses.size() ; i++){
        masses[i].location = vec3(l*i, planeHight, 0.0f);
    }
}

void SimState::scene3Setup(){
    removeOldScene();

    int width = 10;
    int hight = 10;
    int length = 10;

    bool useCube = true;
    if (useCube){
        int cubeSize = 5;
        width = cubeSize;
        hight = cubeSize;
        length = cubeSize;
    }

    float spaceing = 5;

    scene = 3;
    numMasses = width*hight*length;
    simsPerFrame = 128;

    deltT = 0.0002;
    k = 800;
    b = 10;
    l = 5;

    createMasses();
    create3DSprings(width, length);

    for(int i = 0 ; i < masses.size() ; i++){
        float x = ((-(width-1)*spaceing)/2)+((i%width)*spaceing);
        float z = ((-(length-1)*spaceing)/2)+   (    ((int)floor((float)i/width)%length)    *spaceing);
        float y = planeHight -  (    floor((float)i/(width*length))    *spaceing);
        masses[i].location = vec3(x, y+40, z);
    }
}


void SimState::scene4Setup(){
    removeOldScene();

    int width = 10;
    int hight = 10;

    sceneObjWidth = width;

    float spaceing = 5.5;

    scene = 4;
    numMasses = width*hight;
    simsPerFrame = 512;

    deltT = 0.00005;
    k = 500;
    b = 10;
    l = 5;

    createMasses();
    create2DSprings(width);

    for(int i = 0 ; i < masses.size() ; i++){
        float x = ((-(width-1)*spaceing)/2)+((i%width)*spaceing);
        float y = planeHight - (floor((float)i/width)*spaceing)         +10;
        float z = 0.0f;
        masses[i].location = vec3(x, y, z);

        if(i < width){
            float x = ((-(width-1)*l)/2)+((i%width)*l);
            masses[i].location = vec3(x, y, z);
        }

        if(i == numMasses/4)
            masses[i].location = vec3(x, y+1, -2.0f);

        if(i == numMasses/2)
            masses[i].location = vec3(x, y-2, 3.0f);

        if(i == 3*numMasses/4)
            masses[i].location = vec3(x+2, y, -2.0f);

        if(i == (numMasses-1))
            masses[i].location = vec3(x+2, y, 7.0f);

        if(i < width)
            masses[i].dynamic = false;
    }

}

//Flapping flag
void SimState::scene5Setup(){
    removeOldScene();

    int width = 15;
    int hight = 15;

    sceneObjWidth = width;

    float spaceing = 2;

    scene = 5;
    numMasses = width*hight;
    simsPerFrame = 128;

    deltT = 0.001;
    k = 800;
    b = 10;
    l = 2;

    createMasses();
    create2DSprings(width);

    for(int i = 0 ; i < masses.size() ; i++){
        float x = ((-(width-1)*spaceing)/2)+((i%width)*spaceing);
        float y = planeHight - (floor((float)i/width)*spaceing) + 15;
        float z = 0.0f;
        masses[i].location = vec3(x, y, z);

        if(i == (numMasses-1))
            masses[i].location = vec3(x, y, 5.0f);

        if(i == 0 || i == (int)floor(hight/2)*width || i == (hight-1)*width)
            masses[i].dynamic = false;
    }
}

//Cloth on table
void SimState::scene6Setup(){
    removeOldScene();

    int width = 32;
    int hight = 32;

    sceneObjWidth = width;

    float spaceing = 1.2;

    scene = 6;
    numMasses = width*hight;
    simsPerFrame = 32;

    deltT = 0.001;
    k = 800;
    b = 10;
    l = 1.2;

    createMasses();
    create2DSprings(width);

    for(int i = 0 ; i < masses.size() ; i++){
        float x = ((-(width-1)*spaceing)/2)+((i%width)*spaceing);
        float y = planeHight*2;
        float z = ((-(hight-1)*spaceing)/2)+   (    ((int)floor((float)i/hight)%hight)    *spaceing);
        masses[i].location = vec3(x, y, z);
    }
}






void SimState::createMasses(){
    //Create masses
    for(int i = 0 ; i < numMasses ; i++){
        Mass newMass = Mass();
        masses.push_back(newMass);
        //std::cout << "Mass created" << std::endl;
    }
}

void SimState::create1DSprings(){
    //Create springs
    for(int i = 0 ; i < (numMasses-1) ; i++){
        Spring newSpring = Spring(i, i+1, l);
        springs.push_back(newSpring);
        //std::cout << "Spring created" << std::endl;
    }
}

void SimState::create2DSprings(int width){
    Spring newSpring = Spring(0,0,l);

    float springLength2 = sqrt(pow(l,2)+pow(l,2));      //Diagonal springs are sqrt(a^2 + b^2)

    //Create springs
    for(int i = 0 ; i < (numMasses-1) ; i++){
        if(i <= (numMasses-1-width)){         //If not bottom row
            newSpring = Spring(i, i+width, l);      //Down
            springs.push_back(newSpring);

            if(i%width != (width-1)){            //If not right edge
                newSpring = Spring(i, i+1, l);          //Right
                springs.push_back(newSpring);
                newSpring = Spring(i, i+1+width, springLength2);    //Down-Right
                springs.push_back(newSpring);
            }

            if(i%width != 0){  //If not left edge
                newSpring = Spring(i, i-1+width, springLength2);      //Attach Down-Left
                springs.push_back(newSpring);
            }
        }
        else{    //Else it is the bottom row
            newSpring = Spring(i, i+1, l);          //Right
            springs.push_back(newSpring);
        }

        //Reinforcing springs
        if(i <= (numMasses-1-(width*2))){           //If not bottom 2 rows
            newSpring = Spring(i, i+(width*2), l*2);          //2 Down
            springs.push_back(newSpring);
        }

        if((i%width) < (width-2)){           //If not bottom 2 rows
            newSpring = Spring(i, i+2, l*2);          //2 Down
            springs.push_back(newSpring);
        }
    }
}

void SimState::create3DSprings(int width, int length){
    Spring newSpring = Spring(0,0, l);

    float springLength2 = sqrt(pow(l,2)+pow(l,2));  //Diagonal springs are sqrt(a^2 + b^2)
    float springLength3 = sqrt(pow(springLength2,2)+pow(l,2));  //Diagonal springs are sqrt(a^2 + b^2)

    //Create springs
    for(int i = 0 ; i < (numMasses-1) ; i++){

        //Link sheets (XZ)
        if(((int)floor(i/width) % length) != (length-1)){    //If not end of sheet
            if(i <= (numMasses-1-width)){                   //If not bottom row
                newSpring = Spring(i, i+width, l);      //Down
                springs.push_back(newSpring);

                if(i%width != (width-1)){            //If not right edge
                    newSpring = Spring(i, i+1, l);          //Right
                    springs.push_back(newSpring);
                    newSpring = Spring(i, i+1+width, springLength2);    //Down-Right
                    springs.push_back(newSpring);
                }

                if(i%width != 0){  //If not left edge
                    newSpring = Spring(i, i-1+width, springLength2);      //Attach Down-Left
                    springs.push_back(newSpring);
                }
            }
            else{    //Else it is the bottom row
                newSpring = Spring(i, i+1, l);          //Right
                springs.push_back(newSpring);
            }

        }

        else{
            if(i % (width*length) != (width*length)-1){
                newSpring = Spring(i, i+1, l);             //Down
                springs.push_back(newSpring);
            }
        }



        //Link strips (YZ)
        if(i+(width*length) <= (numMasses-1)){             //If not bottom sheet
            newSpring = Spring(i, i+(width*length), l);             //Down
            springs.push_back(newSpring);


            if(((int)floor(i/width) % length) != (length-1)){          //If not end of sheet
                newSpring = Spring(i, i+width+(width*length), springLength2);
                springs.push_back(newSpring);
            }
            //}

            if(((int)floor(i/width) % length) != 0){                   //If not start of sheet
                newSpring = Spring(i, i-width+(width*length), springLength2);
                springs.push_back(newSpring);
            }
        }

        //Link Across (XY)
        if(i+(width*length) <= (numMasses-1)){             //If not bottom XZ
            if(i % width != (width-1)){                    //If not right YZ
                newSpring = Spring(i, i+1+(width*length), springLength2);
                springs.push_back(newSpring);

                if(i%(width*length) < (width*length-width)){ //If not front XY
                    newSpring = Spring(i, i+1+width+(width*length), springLength3);
                    springs.push_back(newSpring);
                }

                if(i%(width*length) > (width-1)){               //If not back XY
                    newSpring = Spring(i, i+1-width+(width*length), springLength3);
                    springs.push_back(newSpring);
                }
            }
            if(i % width != 0){                              //If not left YZ
                if(i%(width*length) < (width*length-width)){ //If not front XY
                    newSpring = Spring(i, i-1+width+(width*length), springLength3);
                    springs.push_back(newSpring);
                }

                if(i%(width*length) > (width-1)){               //If not back XY
                    newSpring = Spring(i, i-1-width+(width*length), springLength3);
                    springs.push_back(newSpring);
                }
            }

            if(i % width != 0){                            //If not back XY
                newSpring = Spring(i, i-1+(width*length), springLength2);
                springs.push_back(newSpring);
            }
        }
    }   //Do not pass here


}
