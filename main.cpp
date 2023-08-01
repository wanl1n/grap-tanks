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

/* Namespaces */
using namespace models;
using namespace cameras;
using namespace lights;
using namespace shaders;

/* Global Variables */
float width = 1280.f;
float height = 720.f;
float speed = 0.2f;

/* Lighting */
// Cycle through intensity of point light using F Key
bool adjustheadlightsIntensity = false;
int headlightsIntensity = 1;
float lowIntensity = 1.f;
float medIntensity = 5.f;
float highIntensity = 10.f;

bool space = false;

/* Camera global variables */
float radius = 50.f;
float pitch = 0.f; // initial pitch is 0 degrees (positive x axis)
float yaw = 270.f; // initial yaw is 270 degrees (-90 degrees of positive x axis; negative z axis)
float yrot = 90.f;

/* Mouse variables */
double prev_xpos = width / 2;
double prev_ypos = height / 2;

// ----- Player Controls ----- //
bool drivingTank = true;        // using 3rd person perspective camera
bool usingBinoculars = false;   // using 1st person perspective camera
bool usingOverhead = false;

// Tank Controls
bool movingForward = false;     // W
bool movingBackward = false;    // S
bool turningLeft = false;       // A
bool turningRight = false;      // D

// Binocular Controls
bool lookingUp = false;         // W
bool lookingDown = false;       // S
bool lookingLeft = false;       // A
bool lookingRight = false;      // D
bool zoomingIn = false;         // Q
bool zoomingOut = false;        // E
// --------------------------- //

/* Keyboard Input Function */
void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mod) {

    // Switching Camera Controls
    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        if (drivingTank) { drivingTank = false; 
                           usingBinoculars = true; 
                           std::cout << "Switched to Binoculars view." << std::endl; }
        
        else if (usingBinoculars || usingOverhead) { drivingTank = true; 
                                                     usingBinoculars = false; 
                                                     usingOverhead = false; 
                                                     std::cout << "Switched to third person view." << std::endl; }
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        usingOverhead = true; drivingTank = false; usingBinoculars = false;
        std::cout << "Switched to Bird's eye view." << std::endl; 
    }

    // Movement Controls
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        if (drivingTank) movingForward = true;
        else if (usingBinoculars) lookingUp = true;
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        if (drivingTank) movingBackward = true;
        else if (usingBinoculars) lookingDown = true;
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        if (drivingTank) turningLeft = true;
        else if (usingBinoculars) lookingLeft = true;
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        if (drivingTank) turningRight = true;
        else if (usingBinoculars) lookingRight = true;
    }
    if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
        movingForward = false;
        lookingUp = false;
    }
    if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
        movingBackward = false;
        lookingDown = false;
    }
    if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
        turningLeft = false;
        lookingLeft = false;
    }
    if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
        turningRight = false;
        lookingRight = false;
    }
    
    // Zooming in and out.
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) { if (usingBinoculars) zoomingIn = true; }
    if (key == GLFW_KEY_E && action == GLFW_PRESS) { if (usingBinoculars) zoomingOut = true; }
    if (key == GLFW_KEY_Q && action == GLFW_RELEASE) zoomingIn = false;
    if (key == GLFW_KEY_E && action == GLFW_RELEASE) zoomingOut = false;

    // Space -- placeholder
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        space = true;
    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
        space = false;

    // Point Light Intensity Controls
    if (key == GLFW_KEY_F && action == GLFW_PRESS) adjustheadlightsIntensity = true;

    // Point Light Intensity
    if (adjustheadlightsIntensity) {
        adjustheadlightsIntensity = false;

        headlightsIntensity++;
        if (headlightsIntensity >= 4)
            headlightsIntensity = 1;

        std::cout << "[F Key Pressed]: Adjusting Point Light Intensity to " << headlightsIntensity << std::endl;
    }

    if (zoomingIn) std::cout << "Binoculars Zooming In." << std::endl;
    if (zoomingOut) std::cout << "Binoculars Zooming Out." << std::endl;
    
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

    /* User Input */
    glfwSetKeyCallback(window, Key_Callback);

    glViewport(0, 0, (int)width, (int)height);

    /* Loading of 3D Models */
    // Main Tank Model
    Model tank = Model("3D/Tank/M1A1.obj", "3D/Tank/plastictexture.jpg", glm::vec3(0.f, -30.f, 0.f), glm::vec3(0.25f), glm::vec3(0.f, 180.f, 0.f));
    // Place Light Source Model away from Main Object
    Model lightModel = Model("3D/Light.obj", "", glm::vec3(-100.f, 90.f, -3.f), glm::vec3(10.f), glm::vec3(0.f), glm::vec4(238.f / 255.f, 228.f / 255.f, 170.f / 255.f, 1.f));

    Shader litShader = Shader("Shaders/sample.vert", "Shaders/sample.frag");

    Skybox skybox = Skybox();

    /* Light Sources */
    // Direction Light: From the Moon 
    DirectionLight moonlight = DirectionLight(lightModel.getPosition(), glm::vec3(0.f), lightModel.getColor(), 5.f);
    // Point Light: Position is at front of tank
    PointLight headlights = PointLight(glm::vec3(tank.getPosition().x - 6.5f, tank.getPosition().y + 23.f, tank.getPosition().z - 57.f), glm::vec3(1.f), 200.f);

    /* Camera */
    PerspectiveCamera thirdPersonPerspectiveCamera = PerspectiveCamera(60.f, height, width, 0.1f, 300.f,
                                                                        glm::vec3(0.f, 0.f, -10.f), 
                                                                        glm::vec3(0.f, 1.f, 0.f), 
                                                                        glm::vec3(0.f, 0.f, -5.f));
    PerspectiveCamera firstPersonPerspectiveCamera = PerspectiveCamera(60.f, height, width, 0.1f, 500.f,
                                                                        glm::vec3(0.f, 0.f, 100.f), 
                                                                        glm::vec3(0.f, 1.f, 0.f), 
                                                                        glm::vec3(0.f, 0.f, 5.f));
    OrthoCamera orthoCamera = OrthoCamera(-100.f, 100.f, -100.f, 100.f, -500.f, 1000.f,
                                            glm::vec3(0.f, 0.f, 0.f), 
                                            glm::vec3(0.f, 0.f, 1.f), 
                                            glm::vec3(0.f, -10.f, 0.f));
    MyCamera* mainCamera = &thirdPersonPerspectiveCamera;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the depth buffer as well

        // ----------------------- UPDATING VALUES -------------------------- //
        // Camera Switching Controls
        if (usingOverhead) {

            mainCamera = &orthoCamera;

            headlights.setColor(glm::vec3(1, 1, 1));
            moonlight.setColor(glm::vec3(1, 1, 1));
        }
        if (drivingTank) {

            double xpos, ypos;
            mainCamera = &thirdPersonPerspectiveCamera;
            glfwGetCursorPos(window, &xpos, &ypos);

            // Calculate how much the mouse moved in x direction
            double x_mod = xpos - prev_xpos;
            // Calculate how much the mouse moved in y direction
            double y_mod = prev_ypos - ypos; // as we move to top y decreases

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
        if (usingBinoculars) {
            firstPersonPerspectiveCamera.setPos(glm::vec3(tank.getPosition().x, tank.getPosition().y + 30.f, tank.getPosition().z - 10.f));
            firstPersonPerspectiveCamera.setCenterOffset(-40.f);

            mainCamera = &firstPersonPerspectiveCamera;

            headlights.setColor(glm::vec3(0, 1, 0));
            moonlight.setColor(glm::vec3(0, 1, 0));
        }

        // If Main Camera exists, update the projection and view matrix
        if (mainCamera != NULL) mainCamera->updateShaderViewProj(litShader.getShaderProgram());
        
        // Update Tank Headlights Intensity
        if (headlightsIntensity == 1)       headlights.setMultiplier(lowIntensity);
        else if (headlightsIntensity == 2)  headlights.setMultiplier(medIntensity);
        else                                headlights.setMultiplier(highIntensity);

        // Update the Tank Movements
        // 1. Driving Tank / Third person perspective view
        if (drivingTank) {
            // Di pa to final - dapat based on camera center yung forward and backward --------------------- !!!
            glm::vec3 moveDirection = glm::vec3(cos(glm::radians(yrot)), 0, sin(glm::radians(yrot)));

            if (movingForward) { 
                tank.move(-moveDirection); 
                //thirdPersonPerspectiveCamera.move(-moveDirection);
            }
            if (movingBackward) { 
                tank.move(moveDirection); 
                //thirdPersonPerspectiveCamera.move(moveDirection);
            }

            // Turning around
            if (turningRight) { 
                tank.rotateBy(glm::vec3(0, -speed, 0));
                yrot += speed;
                // Make sure the binoculars rotate with the tank.
                firstPersonPerspectiveCamera.setPos(tank.getPosition());
                firstPersonPerspectiveCamera.calcKeyRotate(glm::vec3(speed, 0, 0));
                firstPersonPerspectiveCamera.setPos(glm::vec3(tank.getPosition().x, tank.getPosition().y + 30.f, tank.getPosition().z - 10.f));
            }
            if (turningLeft) { 
                tank.rotateBy(glm::vec3(0, speed, 0)); 
                yrot -= speed;
                // Make sure the binoculars rotate with the tank.
                firstPersonPerspectiveCamera.setPos(tank.getPosition());
                firstPersonPerspectiveCamera.calcKeyRotate(glm::vec3(-speed, 0, 0));
                firstPersonPerspectiveCamera.setPos(glm::vec3(tank.getPosition().x, tank.getPosition().y + 30.f, tank.getPosition().z - 10.f));
            }

            // Also update the light from the tank.
            headlights.setPos(firstPersonPerspectiveCamera.getPos());
        }
        // 2. Using Binoculars / First Person perspective view
        else if (usingBinoculars) {
            if (lookingUp) firstPersonPerspectiveCamera.calcKeyRotate(glm::vec3(0, speed, 0));
            if (lookingDown) firstPersonPerspectiveCamera.calcKeyRotate(glm::vec3(0, -speed, 0));
            if (lookingRight) firstPersonPerspectiveCamera.calcKeyRotate(glm::vec3(speed, 0, 0));
            if (lookingLeft) firstPersonPerspectiveCamera.calcKeyRotate(glm::vec3(-speed, 0, 0));
            if (zoomingIn) firstPersonPerspectiveCamera.zoom(-speed);
            if (zoomingOut) firstPersonPerspectiveCamera.zoom(speed);
        }

        // ---------------------- RENDERING OBJECTS ------------------------- //
        // First draw the skybox
        skybox.draw(mainCamera->getViewMatrix(), mainCamera->getProjMatrix());

        // Reset the shader program for the objects.
        glUseProgram(*litShader.getShaderProgram());

        // Moon / Directional Light Source
        lightModel.draw(litShader.getShaderProgram(), false);

        // Applying the lighting to the shader program for the objects.
        moonlight.applyUniqueValuesToShader(litShader.getShaderProgram());
        headlights.applyUniqueValuesToShader(litShader.getShaderProgram());

        /* Draw Main Object */
        
        tank.draw(litShader.getShaderProgram(), true);

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