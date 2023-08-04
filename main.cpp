/* Include files */
#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#include "MainInclude.hpp"

// Models
#include "Model/Model.hpp"
#include "Model/Skybox.hpp"

// Cameras
#include "Camera/MyCamera.hpp"
#include "Camera/PerspectiveCamera.hpp"
#include "Camera/OrthoCamera.hpp"

// Lights
#include "Light/Light.hpp"
#include "Light/DirectionLight.hpp"
#include "Light/PointLight.hpp"

// Shaders
#include "Shaders/Shader.hpp"

// Player
#include "Player/Player.hpp"

// Namespaces of the other classes
using namespace models;
using namespace cameras;
using namespace lights;
using namespace shaders;
using namespace players;

/* Global Variables */
float width = 720.f;
float height = 720.f;
float speed = 1.5f;

Player player = Player(NULL);

/* Camera global variables */
float radius = 50.f;
float pitch = 0.f; // initial pitch is 0 degrees (positive x axis)
float yaw = 270.f; // initial yaw is 270 degrees (-90 degrees of positive x axis; negative z axis)
float yrot = 90.f;

/* Mouse variables */
double prev_xpos = width / 2;
double prev_ypos = height / 2;

/* Keyboard Input Function */
void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mod) {

    // Switching Camera Controls
    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        if (player.isDrivingTank()) {
            player.setDrivingTank(false); 
            player.setUsingBinoculars(true);
            player.setResettingView(true);
            std::cout << "Switched to Binoculars view." << std::endl; 
        }
        
        else if (player.isUsingBinoculars() || player.isUsingDrone()) { 
            player.setDrivingTank(true);
            player.setUsingBinoculars(false);
            player.setUsingDrone(false);
            std::cout << "Switched to third person view." << std::endl; 
        }
    }

    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        player.setUsingDrone(true);
        player.setDrivingTank(false);
        player.setUsingBinoculars(false);
        std::cout << "Switched to Bird's eye view." << std::endl; 
    }

    // Movement Controls
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        if (player.isDrivingTank()) player.setMovingForward(true);
        else if (player.isUsingBinoculars()) player.setLookingUp(true);
        else if (player.isUsingDrone()) player.setDroningForward(true);
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        if (player.isDrivingTank()) player.setMovingBackward(true);
        else if (player.isUsingBinoculars()) player.setLookingDown(true);
        else if (player.isUsingDrone()) player.setDroningBackward(true);
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        if (player.isDrivingTank()) player.setTurningLeft(true);
        else if (player.isUsingBinoculars()) player.setLookingLeft(true);
        else if (player.isUsingDrone()) player.setDroningLeft(true);
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        if (player.isDrivingTank()) player.setTurningRight(true);
        else if (player.isUsingBinoculars()) player.setLookingRight(true);
        else if (player.isUsingDrone()) player.setDroningRight(true);
    }
    if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
        player.setMovingForward(false);
        player.setLookingUp(false);
        player.setDroningForward(false);
    }
    if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
        player.setMovingBackward(false);
        player.setLookingDown(false);
        player.setDroningBackward(false);
    }
    if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
        player.setTurningLeft(false);
        player.setLookingLeft(false);
        player.setDroningLeft(false);
    }
    if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
        player.setTurningRight(false);
        player.setLookingRight(false);
        player.setDroningRight(false);
    }
    
    // Zooming in and out.
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) { if (player.isUsingBinoculars()) player.setZoomingIn(true); std::cout << "Binoculars Zooming In." << std::endl; }
    if (key == GLFW_KEY_E && action == GLFW_PRESS) { if (player.isUsingBinoculars()) player.setZoomingOut(true); std::cout << "Binoculars Zooming Out." << std::endl; }
    if (key == GLFW_KEY_Q && action == GLFW_RELEASE) player.setZoomingIn(false);
    if (key == GLFW_KEY_E && action == GLFW_RELEASE) player.setZoomingOut(false);

    // Point Light Intensity Controls
    if (key == GLFW_KEY_F && action == GLFW_PRESS) player.setAdjustingHeadlights(true);
        
}

int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow((int)width, (int)height, "gdgrappling at straws", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    // Getting User Key Input
    glfwSetKeyCallback(window, Key_Callback);

    // Create Viewport
    glViewport(0, 0, (int)width, (int)height);

    // Loading the objects
    // Main Tank Model
    Model tank = Model("3D/Tank/M1A1.obj", "3D/Tank/TankTex.png", "3D/Tank/TankNorm.png",
                        glm::vec3(0.f, 0.f, 0.f),           // pos
                        glm::vec3(0.25f),                   // scale
                        glm::vec3(0.f, 180.f, 0.f));          // rotate
    // Moon / Direction Light model
    Model lightModel = Model("3D/Moon.obj", "", "",
                            glm::vec3(-100.f, 1000.f, -30.f), // pos
                            glm::vec3(10.f),                // scale
                            glm::vec3(0.f),                 // rotate
                            glm::vec4(238.f / 255.f, 228.f / 255.f, 170.f / 255.f, 1.f)); // color
    // Environment
    Model ground = Model("3D/Ground/Ground.obj", "3D/Ground/AddWater_basecolor.png", "3D/Ground/AddWater_normal.png",
                        glm::vec3(0.f, 0.f, 0.f),           //pos
                        glm::vec3(100.f),                     //scale
                        glm::vec3(0.f, 0.f, 0.f));         //rotate
    Model stone = Model("3D/Obstacles/Rocks/Stone.OBJ", "3D/Obstacles/Rocks/StoneTex.jpg", "",
                        glm::vec3(0.f, -8.f, -200.f),       //pos
                        glm::vec3(5.f),                     //scale
                        glm::vec3(0.f, 0.f, 0.f));          //rotate
    Model ant = Model("3D/Obstacles/Ant/ant1.obj", "3D/Obstacles/Ant/ant_(1).png", "",
                        glm::vec3(-150.f, 0.f, -200.f),     //pos
                        glm::vec3(0.03f),                   //scale
                        glm::vec3(0.f, 60.f, 0.f));         //rotate
    Model grass = Model("3D/Obstacles/Grass/Grass.obj", "3D/Obstacles/Grass/GrassTex.png", "",
                        glm::vec3(-250.f, 0.f, -400.f),      //pos
                        glm::vec3(0.2f),                    //scale
                        glm::vec3(0.f, 60.f, 0.f));         //rotate
    Model mouse = Model("3D/Obstacles/Mouse/Mouse.obj", "3D/Obstacles/Mouse/MouseTex.png", "",
                        glm::vec3(0.f, 0.f, -550.f),        //pos
                        glm::vec3(10000.f),                 //scale
                        glm::vec3(0.f, 60.f, 0.f));         //rotate
    Model flower = Model("3D/Obstacles/Flowers/Flower.obj", "3D/Obstacles/Flowers/FlowerTexA.png", "",
                        glm::vec3(0.f, 0.f, -300.f),        //pos
                        glm::vec3(250.f),                   //scale
                        glm::vec3(0.f, 60.f, 0.f));         //rotate
    Model tractor = Model("3D/Obstacles/Car/Tractor.obj", "3D/Obstacles/Car/TractorTex.jpg", "",
                        glm::vec3(-100.f, 0.f, 0.f),        //pos
                        glm::vec3(3.f),                     //scale
                        glm::vec3(0.f, 60.f, 0.f));         //rotate

    // Save the Tank model to the player.
    player.setModel(&tank);

    // Create a vector of objects around the Player and store all the surrounding objects with the ground at the top.
    std::vector<Model> environment;
    environment.push_back(ground);
    environment.push_back(stone);
    environment.push_back(ant);
    environment.push_back(grass);
    environment.push_back(mouse);
    environment.push_back(flower);
    environment.push_back(tractor);

    // Creating the shader for the objects
    Shader litShader = Shader("Shaders/sample.vert", "Shaders/sample.frag");

    // Creating the skybox
    Skybox skybox = Skybox();


    /* Light Sources */
    // Direction Light: From the Moon 
    DirectionLight moonlight = DirectionLight(lightModel.getPosition(), glm::vec3(0.f), lightModel.getColor(), 0.1f);
    // Point Light: Position is at front of tank
    PointLight headlights = PointLight(glm::vec3(tank.getPosition().x, tank.getPosition().y + 50.f, tank.getPosition().z - 57.f), glm::vec3(1.f), 200.f);

    // Cameras
    PerspectiveCamera thirdPersonPerspectiveCamera = PerspectiveCamera(60.f, height, width, 0.1f, 300.f,
                                                                        glm::vec3(0.f, 0.f, -10.f), 
                                                                        glm::vec3(0.f, 1.f, 0.f), 
                                                                        glm::vec3(0.f, 0.f, -5.f));
    PerspectiveCamera firstPersonPerspectiveCamera = PerspectiveCamera(60.f, height, width, 0.1f, 500.f,
                                                                        glm::vec3(tank.getPosition().x, tank.getPosition().y + 30.f, tank.getPosition().z - 20.f),
                                                                        glm::vec3(0.f, 1.f, 0.f), 
                                                                        glm::vec3(0.f, 0.f, 5.f));
    OrthoCamera orthoCamera = OrthoCamera(-100.f, 100.f, -100.f, 100.f, -500.f, 1000.f,
                                            glm::vec3(0.f, 0.f, 0.f), 
                                            glm::vec3(0.f, 0.f, -1.f), 
                                            glm::vec3(0.f, -10.f, 0.f));
    MyCamera* mainCamera = &thirdPersonPerspectiveCamera;

    // Put the headlights at the first person camera position
    firstPersonPerspectiveCamera.setCenterOffset(glm::vec3(0, 0, -40.f));
    headlights.setPos(firstPersonPerspectiveCamera.getCameraCenter());


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the depth buffer as well

        // ----------------------- UPDATING VALUES -------------------------- //        
        // Camera Switching Controls
        // 1. Bird's eye view / Top View Orthographic Camera
        if (player.isUsingDrone()) {

            mainCamera = &orthoCamera;

            headlights.setColor(glm::vec3(1, 1, 1));
            moonlight.setColor(glm::vec3(1, 1, 1));
        }
        // 2. Third person view / Third person perspective camera
        if (player.isDrivingTank()) {

            double xpos, ypos;
            mainCamera = &thirdPersonPerspectiveCamera;
            glfwGetCursorPos(window, &xpos, &ypos);

            // Calculate how much the mouse moved in x direction
            double x_mod = xpos - prev_xpos;
            // Calculate how much the mouse moved in y direction
            double y_mod = prev_ypos - ypos; 

            // Set new pos
            prev_xpos = xpos;
            prev_ypos = ypos;

            // Compute new pitch and yaw angles
            pitch += (float)y_mod * 0.1f;
            yaw += (float)x_mod * 0.1f;

            thirdPersonPerspectiveCamera.calcMouseRotate(pitch, yaw, tank.getPosition());

            headlights.setColor(glm::vec3(1, 1, 1));
            moonlight.setColor(glm::vec3(1, 1, 1));
        }
        // 3. Binoculars View / First person perspective camera view
        if (player.isUsingBinoculars()) {
            if (player.isResettingView()) {
                player.setResettingView(false);
                firstPersonPerspectiveCamera.setPos(glm::vec3(tank.getPosition().x, tank.getPosition().y + 30.f, tank.getPosition().z - 10.f));
                firstPersonPerspectiveCamera.setCenterOffset(glm::vec3(0, 0, -40.f));
            }
           
            mainCamera = &firstPersonPerspectiveCamera;

            headlights.setColor(glm::vec3(0, 1, 0));
            moonlight.setColor(glm::vec3(0, 1, 0));
        }

        // If Main Camera exists, update the projection and view matrix
        if (mainCamera != NULL) mainCamera->updateShaderViewProj(litShader.getShaderProgram());
        
        // Update Tank Headlights Intensity
        if (player.getHeadlightsIntensity() == 1)       headlights.setMultiplier(player.getLowIntensity());
        else if (player.getHeadlightsIntensity() == 2)  headlights.setMultiplier(player.getMedIntensity());
        else                                            headlights.setMultiplier(player.getHighIntensity());

        // Update the Tank Movements
        // 1. Driving Tank / Third person perspective view
        if (player.isDrivingTank()) {

            // Moving forward and back
            glm::vec3 moveDirection = glm::vec3(cos(glm::radians(yrot)), 0, sin(glm::radians(yrot))) * 2.0f;

            if (player.isMovingForward()) { 
                tank.move(-moveDirection); 
                firstPersonPerspectiveCamera.setCenterOffset(-moveDirection); 
            }
            if (player.isMovingBackward()) { 
                tank.move(moveDirection); 
                firstPersonPerspectiveCamera.setCenterOffset(-moveDirection); 
            }

            // Turning around
            if (player.isTurningRight()) { 
                tank.rotateBy(glm::vec3(0, -speed, 0));
                yrot += speed; // Stores the total rotation of the tank.
                // Make sure the binoculars rotate with the tank.
                firstPersonPerspectiveCamera.setPos(tank.getPosition());
                firstPersonPerspectiveCamera.calcKeyRotate(glm::vec3(speed, 0, 0));
                firstPersonPerspectiveCamera.setPos(glm::vec3(tank.getPosition().x, tank.getPosition().y + 30.f, tank.getPosition().z - 10.f));
            }
            if (player.isTurningLeft()) { 
                tank.rotateBy(glm::vec3(0, speed, 0)); 
                yrot -= speed; // Stores the total rotation of the tank.
                // Make sure the binoculars rotate with the tank.
                firstPersonPerspectiveCamera.setPos(tank.getPosition());
                firstPersonPerspectiveCamera.calcKeyRotate(glm::vec3(-speed, 0, 0));
                firstPersonPerspectiveCamera.setPos(glm::vec3(tank.getPosition().x, tank.getPosition().y + 30.f, tank.getPosition().z - 10.f));
            }

            // Also update the light from the tank.
            headlights.setPos(firstPersonPerspectiveCamera.getCameraCenter() + glm::vec3(0, 10.f, -20.f));
        }
        // 2. Using Binoculars / First Person perspective view
        else if (player.isUsingBinoculars()) {

            if (player.isLookingUp()) firstPersonPerspectiveCamera.calcKeyRotate(glm::vec3(0, speed, 0));
            if (player.isLookingDown()) firstPersonPerspectiveCamera.calcKeyRotate(glm::vec3(0, -speed, 0));
            if (player.isLookingRight()) firstPersonPerspectiveCamera.calcKeyRotate(glm::vec3(speed, 0, 0));
            if (player.isLookingLeft()) firstPersonPerspectiveCamera.calcKeyRotate(glm::vec3(-speed, 0, 0));
            if (player.isZoomingIn()) firstPersonPerspectiveCamera.zoom(-speed);
            if (player.isZoomingOut()) firstPersonPerspectiveCamera.zoom(speed);

            if (player.isLookingUp()) std::cout << "Binoculars View: Looking Up." << std::endl;
            if (player.isLookingDown()) std::cout << "Binoculars View: Looking Down." << std::endl;
            if (player.isLookingRight()) std::cout << "Binoculars View: Looking Right." << std::endl;
            if (player.isLookingLeft()) std::cout << "Binoculars View: Looking Left." << std::endl;
            if (player.isZoomingIn()) std::cout << "Binoculars View: Zooming In." << std::endl;
            if (player.isZoomingOut()) std::cout << "Binoculars View: Zooming Out." << std::endl;
        }
        // 3. Using Overhead drone view / Orthographic Top View
        else if (player.isUsingDrone()) {

            if (player.isDroningForward()) orthoCamera.move(glm::vec3(0, 0, -speed));
            if (player.isDroningBackward()) orthoCamera.move(glm::vec3(0, 0, speed));
            if (player.isDroningLeft()) orthoCamera.move(glm::vec3(-speed, 0, 0));
            if (player.isDroningRight()) orthoCamera.move(glm::vec3(speed, 0, 0));
        }

        // ---------------------- RENDERING OBJECTS ------------------------- //
        // First draw the skybox
        skybox.draw(mainCamera->getViewMatrix(), mainCamera->getProjMatrix(), player.isUsingBinoculars());

        // Reset the shader program for the objects.
        glUseProgram(*litShader.getShaderProgram());

        // Moon / Directional Light Source - Uncomment this to see where the light source is.
        //lightModel.draw(litShader.getShaderProgram(), false);

        // Applying the lighting to the shader program for the objects.
        moonlight.applyUniqueValuesToShader(litShader.getShaderProgram());
        headlights.applyUniqueValuesToShader(litShader.getShaderProgram());

        // Draw Main Object
        tank.draw(litShader.getShaderProgram(), true);

        // Draw the environment
        for(Model model : environment) {
            model.draw(litShader.getShaderProgram(), true);
        }

        // Reset lighting to render lightModel unaffected by light
        headlights.turnOff();
        moonlight.turnOff();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}