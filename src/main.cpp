//------------------------------------------------------------------------------
// A simple example showing how to use the triangle geometry
//------------------------------------------------------------------------------
#include "givr.h"
#include <glm/gtc/matrix_transform.hpp>

#include "io.h"
#include "turntable_controls.h"

#include "SimState.h"


#include <math.h>

using namespace glm;
using namespace givr;
using namespace givr::camera;
using namespace givr::geometry;
using namespace givr::style;

PhongStyle::InstancedRenderContext spheres;
PhongStyle::InstancedRenderContext cylinders;
PhongStyle::RenderContext triangles;

void physicsCalculation();
void updateForces();
void updateMasses();
void collisionResolution(int i);
void createShapes();

SimState simState = SimState();




int main(void)
{
    io::GLFWContext windows;
    auto window = windows.create(io::Window::dimensions{640, 480}, "Simple example");
    window.enableVsync(true);

    auto view = View(TurnTable(), Perspective());
    TurnTableControls controls(window, view.camera);

    //Setup keybindings
    window.keyboardCommands() |
            io::Key(GLFW_KEY_1, [&](auto const &event) {
        if (event.action == GLFW_PRESS)
            simState.scene1Setup();
    })
            | io::Key(GLFW_KEY_2, [&](auto const &event) {
        if (event.action == GLFW_PRESS)
            simState.scene2Setup();
    })
            | io::Key(GLFW_KEY_3, [&](auto const &event) {
        if (event.action == GLFW_PRESS)
            simState.scene3Setup();
    })
            | io::Key(GLFW_KEY_4, [&](auto const &event) {
        if (event.action == GLFW_PRESS)
            simState.scene4Setup();
    })
            | io::Key(GLFW_KEY_5, [&](auto const &event) {
        if (event.action == GLFW_PRESS)
            simState.scene5Setup();
    })
            | io::Key(GLFW_KEY_6, [&](auto const &event) {
        if (event.action == GLFW_PRESS)
            simState.scene6Setup();
    });


    glClearColor(1.f, 1.f, 1.f, 1.f);
    float u = 0.;



    createShapes();
    auto planePt1 = createRenderable(
        Triangle(Point1(-simState.planeSize, 0.0f, -simState.planeSize), Point2(-simState.planeSize, 0.0f, simState.planeSize), Point3(simState.planeSize, 0.0f, simState.planeSize)),
        Phong(Colour(1., 1., 0.1529), LightPosition(0., 0., 10.))
    );

    auto planePt2 = createRenderable(
        Triangle(Point1(simState.planeSize, 0.0f, simState.planeSize), Point2(simState.planeSize, 0.0f, -simState.planeSize), Point3(-simState.planeSize, 0.0f, -simState.planeSize)),
        Phong(Colour(1., 1., 0.1529), LightPosition(0., 0., 10.))
    );

     auto ts = TriangleSoup();
    triangles = createRenderable(ts,Phong(Colour(1., 1., 0.1529), LightPosition(0., 0., 10.)));


    /////////////////////////////////////////////////////
    //////////////////// Render Loop ////////////////////
    /////////////////////////////////////////////////////
    window.run([&](float frameTime) {
        view.projection.updateAspectRatio(window.width(), window.height());

        //Physics calculations
        for( int i = 0 ; i < simState.simsPerFrame ; i++){
            physicsCalculation();
        }

        if(simState.scene == 1 || simState.scene == 2){
            //Draw top plane
            auto m = translate(mat4f{1.f}, vec3f{0.0, simState.planeHight, 0.0});
            draw(planePt1, view, m);
            draw(planePt2, view, m);
        }

        if(simState.scene == 3 || simState.scene == 6){
            //Draw top plane
            auto m = translate(mat4f{1.f}, vec3f{0.0, simState.tableHight, 0.0});
            if(simState.scene == 3)
                m = scale(m, vec3f{2.0, 1.0, 2.0});
            draw(planePt1, view, m);
            draw(planePt2, view, m);
        }


        u += frameTime;

        if(simState.scene < 4){
            //Draw masses
            for (int i = 0; i < simState.masses.size(); i++) {
                auto m = translate(mat4f{1.f}, simState.masses[i].location);
                addInstance(spheres, m);
            }
            draw(spheres, view);


            //Draw springs
            for (int i = 0; i < simState.springs.size(); i++){
                //Get spring vector
                vec3 massLocDiff = simState.masses[simState.springs[i].mass2Index].location - simState.masses[simState.springs[i].mass1Index].location;
                float len = glm::length(massLocDiff);

                //Translate
                auto m = mat4f{1.f};
                m = translate(m, simState.masses[simState.springs[i].mass1Index].location);


                float angle1;
                float angle2;
                //Rotate along y
                vec2 xz;
                if(massLocDiff.x != 0 || massLocDiff.z != 0){xz = normalize(vec2(massLocDiff.x, massLocDiff.z));}
                else{xz = vec2(1.0f, 0.0f);}
                if(massLocDiff.x > 0)
                    angle1 = asin(xz[1]);
                else
                    angle1 = M_PI-asin(xz[1]);
                m = rotate(m, angle1, vec3f{0.0f, -1.0f, 0.0f});


                //Rotate along z
                vec3 rotatedDiff = vec4(massLocDiff, 0.0f)*m;
                vec2 xy;
                if(rotatedDiff.x != 0 || rotatedDiff.y != 0){xy = normalize(vec2(rotatedDiff.x, rotatedDiff.y));}
                else{xy = vec2(1.0f, 0.0f);}
                angle2 = asin(xy.x);
                if(xy.y > 0){angle2 = (1*M_PI)-angle2;}
                m = rotate(m, angle2, vec3f{0.0f, 0.0f, 1.0f});


                //Scale
                m = scale(m, vec3f{1.0f, len, 1.0f});

                addInstance(cylinders, m);
            }
            draw(cylinders, view);
        }
        else{
            ts.triangles().clear();

            Mass mass;
            // Loop over all objects in your simulation/animation
            for(int i = 0; i < simState.masses.size()-simState.sceneObjWidth; i++) {
                if(i%simState.sceneObjWidth != (simState.sceneObjWidth-1)){
                   // Get a reference to the object
                   mass = simState.masses[i];
                   auto p1 = vec3f(mass.location.x, mass.location.y, mass.location.z);
                   mass = simState.masses[i+simState.sceneObjWidth];
                   auto p2 = vec3f(mass.location.x, mass.location.y, mass.location.z);
                   mass = simState.masses[i+1];
                   auto p3 = vec3f(mass.location.x, mass.location.y, mass.location.z);

                   // Turn that object into a Triangle (or triangles!)
                   auto t = Triangle(Point1(p1), Point2(p2), Point3(p3));

                   // Add that triangle to the triangle soup
                   ts.push_back(t);

                   // Get a reference to the object
                   mass = simState.masses[i+simState.sceneObjWidth];
                   p1 = vec3f(mass.location.x, mass.location.y, mass.location.z);
                   mass = simState.masses[i+simState.sceneObjWidth+1];
                   p2 = vec3f(mass.location.x, mass.location.y, mass.location.z);
                   mass = simState.masses[i+1];
                   p3 = vec3f(mass.location.x, mass.location.y, mass.location.z);

                   // Turn that object into a Triangle (or triangles!)
                   t = Triangle(Point1(p1), Point2(p2), Point3(p3));

                   // Add that triangle to the triangle soup
                   ts.push_back(t);
                }
            }

            updateRenderable(ts, Phong(Colour(1., 1., 0.1529), LightPosition(0., 0., 10.)), triangles);
            draw(triangles, view);
        }
    });
    exit(EXIT_SUCCESS);
}





    /////////////////////////////////////////////////////
    /////////////////// Physics Stuff ///////////////////
    /////////////////////////////////////////////////////

// F = -k(x - l) - bv
// F/m = a
// v(t+dt) = v(t) + a(t)dt
// x(t+dt) = x(t) + v(t+dt)dt

//Have each spring apply force to masses
//Have masses update their location

vec3 v = vec3(10.0f, 0.0f, 0.0f);      //Wind vector

void physicsCalculation(){
    updateForces();
    updateMasses();
}

void updateForces(){
    //Spring force
    for(int i = 0; i < simState.springs.size(); i++){
        vec3 massLocDiff = simState.masses[simState.springs[i].mass2Index].location - simState.masses[simState.springs[i].mass1Index].location;
        float len = glm::length(massLocDiff);
        vec3 direction = normalize(massLocDiff);

        vec3 F = -simState.k*(len - simState.springs[i].restDistance)*direction;         //Base force

        simState.masses[simState.springs[i].mass1Index].totalForce += -F;
        simState.masses[simState.springs[i].mass2Index].totalForce += F;
    }
}

void updateMasses(){
    //If scene 5, add wind force
    if(simState.scene == 5){
        int maxIndex = (simState.numMasses-simState.sceneObjWidth);
        for (int i = 0 ; i < maxIndex ; i++){
            if(i%simState.sceneObjWidth != simState.sceneObjWidth-1){
                Mass mass1 = simState.masses[i];
                Mass mass2 = simState.masses[i+simState.sceneObjWidth];
                Mass mass3 = simState.masses[i+1];

                vec3 p = (mass1.velocity + mass2.velocity + mass3.velocity);

                p = vec3(p.x/3, p.y/3, p.z/3);
                vec3 vr = v-p;

                //std::cout << "v: (" << v.x << "," << v.y << "," << v.z << ")"<< std::endl;

                vec3 normal = normalize(cross((mass2.location - mass1.location),(mass3.location - mass1.location)));
                vec3 vn = dot(normal, normalize(vr))* v;
                vec3 vt = v - vn;

                //std::cout << "vn: (" << vn.x << "," << vn.y << "," << vn.z << ")"<< std::endl;

                float A = (length(mass2.location - mass1.location)*length(mass3.location - mass1.location))/2;
                //float A = 0.5;
                float alphan = 1.0f;
                float alphat = alphan;

                vec3 Fn = alphan*A*v*vn;
                vec3 Ft = alphat*A*vt;

                //std::cout << "Fn: " << length(Fn) << "   Ft: " << length(Ft) << std::endl;
                //std::cout << "Fn: (" << Fn.x << "," << Fn.y << "," << Fn.z << ")"<< std::endl;

                vec3 extraF = Fn + Ft;
                if(length(extraF) > 20){
                    //std::cout << "extraF: " << length(extraF) << std::endl;
                }
                if(extraF.x < 0){
                    extraF = vec3(-extraF.x, extraF.y, extraF.z);
                    //std::cout << "extraF: " << length(extraF) << std::endl;
                }

                //std::cout << "extraF: " << length(extraF) << std::endl;
                //std::cout << "extraF: (" << extraF.x << "," << extraF.y << "," << extraF.z << ")"<< std::endl;

                simState.masses[i].totalForce += extraF;
                simState.masses[i+simState.sceneObjWidth].totalForce += extraF;
                simState.masses[i+1].totalForce += extraF;
            }
        }
    }


    for (int i = 0 ; i < simState.masses.size() ; i++){
        if(simState.masses[i].dynamic){
            simState.masses[i].totalForce = simState.masses[i].totalForce - simState.b*simState.masses[i].velocity;
            //std::cout << "Mass " << i << " has force: (" << masses[i].totalForce.x << ", " << masses[i].totalForce.y << ", " << masses[i].totalForce.z << ") applied to it" << std::endl;
            vec3 a = simState.masses[i].totalForce/simState.masses[i].weight;
            a += simState.g;
            simState.masses[i].velocity = simState.masses[i].velocity + (a*simState.deltT);
            collisionResolution(i);
            simState.masses[i].location = simState.masses[i].location + (simState.masses[i].velocity*simState.deltT);
        }

        simState.masses[i].totalForce = vec3(0.0f, 0.0f, 0.0f);
    }
}


void collisionResolution(int i){
    //Cube collision
    if(simState.scene == 3 &&
            simState.masses[i].location.y <= simState.tableHight &&
            simState.masses[i].velocity.y < 0)     //Collision with plane
        simState.masses[i].velocity.y = 0.0f;

    float maxDepth = 1.0f;

    //Table collision
    if(simState.scene == 6){
        //Collission with top of table
        if(     (simState.masses[i].location.y <= simState.tableHight) &&
                simState.masses[i].location.y > (simState.tableHight-maxDepth) &&
                (simState.masses[i].location.x < simState.planeSize && simState.masses[i].location.x > -simState.planeSize) &&
                (simState.masses[i].location.z < simState.planeSize && simState.masses[i].location.z > -simState.planeSize)){
            if(simState.masses[i].velocity.y < 0){
                simState.masses[i].velocity.y = 0.0f;
            }
        }

        //Left side of table
        if(     (simState.masses[i].location.y <= simState.tableHight) &&
                (simState.masses[i].location.x < simState.planeSize && simState.masses[i].location.x > -simState.planeSize) &&
                simState.masses[i].location.x < -simState.planeSize+maxDepth &&
                (simState.masses[i].location.z < simState.planeSize && simState.masses[i].location.z > -simState.planeSize)){
            if(simState.masses[i].velocity.x > 0){
                simState.masses[i].velocity.x = 0.0f;
            }
        }

        //Right side of table
        if(     (simState.masses[i].location.y <= simState.tableHight) &&
                (simState.masses[i].location.x < simState.planeSize && simState.masses[i].location.x > -simState.planeSize) &&
                simState.masses[i].location.x < simState.planeSize-maxDepth &&
                (simState.masses[i].location.z < simState.planeSize && simState.masses[i].location.z > -simState.planeSize)){
            if(simState.masses[i].velocity.x < 0){
                simState.masses[i].velocity.x = 0.0f;
            }
        }

        //Front side of table
        if(     (simState.masses[i].location.y <= simState.tableHight) &&
                (simState.masses[i].location.x < simState.planeSize && simState.masses[i].location.x > -simState.planeSize) &&
                (simState.masses[i].location.z < simState.planeSize && simState.masses[i].location.z > -simState.planeSize) &&
                simState.masses[i].location.z < -simState.planeSize+maxDepth){
            if(simState.masses[i].velocity.z > 0){
                simState.masses[i].velocity.z = 0.0f;
            }
        }

        //Back side of table
        if(     (simState.masses[i].location.y <= simState.tableHight) &&
                (simState.masses[i].location.x < simState.planeSize && simState.masses[i].location.x > -simState.planeSize) &&
                (simState.masses[i].location.z < simState.planeSize && simState.masses[i].location.z > -simState.planeSize) &&
                simState.masses[i].location.z > simState.planeSize-maxDepth){
            if(simState.masses[i].velocity.z < 0){
                simState.masses[i].velocity.z = 0.0f;
            }
        }
    }
}



        /////////////////////////////////////////////////////
        //////////////////// Shape Stuff ////////////////////
        /////////////////////////////////////////////////////

void createShapes(){
    //Setup shapeswindVec
    spheres = createInstancedRenderable(
        Sphere(
            Radius(1.0f)
        ),
        Phong(
            Colour(1., 1., 0.1529),
            LightPosition(0., 0.0, 20.0)
        )
    );

    cylinders = createInstancedRenderable(
        Cylinder(
            Point1(0.0f, 0.0f, 0.0f),
            Point2(0.0f, -1.0f, 0.0f),
            Radius(0.4f)
        ),
        Phong(
            Colour(1., 1., 0.1529),
            LightPosition(0., 0.0, 20.0)
        )
    );
}

