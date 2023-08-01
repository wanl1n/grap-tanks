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

/* Lighting */
// Cycle through intensity of point light using F Key
bool adjustPointLightIntensity = false;
int pointLightIntensity = 1;
float lowIntensity = 1.f;
float medIntensity = 5.f;
float highIntensity = 10.f;

// Camera Perspectives
bool useOrtho = false;
bool usePerspective = true;
bool swapPerspective = false;
int perspective = 3;

bool controllingMainObj = true;

bool rotatePosXAxis = false;
bool rotateNegXAxis = false;

bool rotatePosYAxis = false;
bool rotateNegYAxis = false;

bool rotatePosZAxis = false;
bool rotateNegZAxis = false;

bool space = false;

float x_axis_mod = 0.f;
float y_axis_mod = 0.f;
float z_axis_mod = 0.f;

bool changeProjection = false;

float point_intensity_mod = 0.f;
float direction_intensity_mod = 0.f;

/* Camera global variables */
float radius = 50.f;
float pitch = 0.f; // initial pitch is 0 degrees (positive x axis)
float yaw = 270.f; // initial yaw is 270 degrees (-90 degrees of positive x axis; negative z axis)

/* Mouse variables */
double prev_xpos = width / 2;
double prev_ypos = height / 2;

/* Orbit variables */
float orbitPitch = 0.f;
float orbitYaw = 270.f;
float orbitRoll = 0.f;
bool orbitPosX = false;
bool orbitNegX = false;
bool orbitPosY = false;
bool orbitNegY = false;
bool orbitPosZ = false;
bool orbitNegZ = false;

/* Keyboard Input Function */
void Key_Callback(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mod)
{
    float speed = 0.2f;

    /* Press Key */
    // 1 Key
    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        usePerspective = true;
        useOrtho = true;
        swapPerspective = true;
    }
    // 2 Key
    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        usePerspective = false;
        useOrtho = true;
        swapPerspective = false;
    }

    // F Key
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
        adjustPointLightIntensity = true;

    /* Release Key */
    // 1 Key
    if (key == GLFW_KEY_1 && action == GLFW_RELEASE) {
        usePerspective = true;
        useOrtho = false;
        swapPerspective = false;
    }
    // 2 Key
    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        usePerspective = false;
        useOrtho = true;
        swapPerspective = false;
    }
    // F Key
    if (key == GLFW_KEY_F && action == GLFW_RELEASE)
        adjustPointLightIntensity = false;

    // W
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        rotatePosXAxis = true;
        if (!controllingMainObj)
            orbitPosX = true;
    }
    // S
    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        rotateNegXAxis = true;
        if (!controllingMainObj)
            orbitNegX = true;
    }
    // A
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        rotatePosYAxis = true;
        if (!controllingMainObj)
            orbitPosY = true;
    }
    // D
    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        rotateNegYAxis = true;
        if (!controllingMainObj)
            orbitNegY = true;
    }
    // Q
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        rotatePosZAxis = true;
        if (!controllingMainObj)
            orbitPosZ = true;
    }
    // E
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        rotateNegZAxis = true;
        if (!controllingMainObj)
            orbitNegZ = true;
    }
    // Space
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        space = true;
    // 1
    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        usePerspective = true;
        useOrtho = false;
    }
    // 2
    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        usePerspective = false;
        useOrtho = true;
    }
    //// arrow up
    //if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    //    increasePointLightIntensity = true;
    //// arrow down
    //if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    //    decreasePointLightIntensity = true;
    //// arrow left
    //if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    //    decreaseDirectionLightIntensity = true;
    //// arrow right
    //if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    //    increaseDirectionLightIntensity = true;

    /* Release */
     // W
    if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
        rotatePosXAxis = false;
        orbitPosX = false;
    }
    // S
    if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
        rotateNegXAxis = false;
        orbitNegX = false;
    }
    // A
    if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
        rotatePosYAxis = false;
        orbitPosY = false;
    }
    // D
    if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
        rotateNegYAxis = false;
        orbitNegY = false;
    }
    // Q
    if (key == GLFW_KEY_Q && action == GLFW_RELEASE) {
        rotatePosZAxis = false;
        orbitPosZ = false;
    }
    // E
    if (key == GLFW_KEY_E && action == GLFW_RELEASE) {
        rotateNegZAxis = false;
        orbitNegZ = false;
    }
    // Space
    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
        space = false;
    //// arrow up
    //if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
    //    increasePointLightIntensity = false;
    //// arrow down
    //if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
    //    decreasePointLightIntensity = false;
    //// arrow left
    //if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
    //    decreaseDirectionLightIntensity = false;
    //// arrow right
    //if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
    //    increaseDirectionLightIntensity = false;

    /* Update */
    // Point Light Intensity
    if (adjustPointLightIntensity) {
        pointLightIntensity++;
        if (pointLightIntensity >= 4)
            pointLightIntensity = 1;

        std::cout << "[F Key Pressed]: Adjusting Point Light Intensity" << std::endl;
    }
    // Swap between first and third person perspective
    if (usePerspective) {
        if (swapPerspective) {
            if (perspective == 1) perspective = 3;
            else if (perspective == 3) perspective = 1;
            std::cout << "[1 Key Pressed]: Changing Perspective Camera" << std::endl;
        }
    }


    // Change controlled object
    if (space)
        controllingMainObj = !controllingMainObj;

    // Main Obj
    if (controllingMainObj) {
        // Rotate along X axis
        if (rotatePosXAxis)
            x_axis_mod -= speed * 10;
        if (rotateNegXAxis)
            x_axis_mod += speed * 10;
        // Rotate along Y axis
        if (rotatePosYAxis)
            y_axis_mod += speed * 10;
        if (rotateNegYAxis)
            y_axis_mod -= speed * 10;
        // Rotate along Z axis
        if (rotatePosZAxis)
            z_axis_mod += speed * 10;
        if (rotateNegZAxis)
            z_axis_mod -= speed * 10;
    }
    // Controlling Light Source
    else
        //// Point light intensity
        //if (increasePointLightIntensity)
        //    point_intensity_mod += (speed / 10.f);
        //// Point light intensity
        //if (decreasePointLightIntensity)
        //    point_intensity_mod -= (speed / 10.f);
        //// Direction light intensity
        //if (increaseDirectionLightIntensity)
        //    direction_intensity_mod += (speed / 10000.f);
        //// Direction light intensity
        //if (decreaseDirectionLightIntensity)
        //    direction_intensity_mod -= (speed / 10000.f);

        // Orbit along X axis
        if (orbitPosX)
            orbitPitch += speed * 50;
    if (orbitNegX)
        orbitPitch -= speed * 50;
    // Rotate along Y axis
    if (orbitPosY)
        orbitYaw += speed * 50;
    if (orbitNegY)
        orbitYaw -= speed * 50;
    // Rotate along Z axis
    if (orbitPosZ)
        orbitRoll += speed * 50;
    if (orbitNegZ)
        orbitRoll -= speed * 50;
}

void CreateProgram(const char* pathVert, const char* pathFrag, GLuint* shaderProgram) {

    // Vertex shader for positioning
    std::fstream vertSrc(pathVert);
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    // Fragment shader for coloring
    std::fstream fragSrc(pathFrag);
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    // Creating the vertex shader for use in the program.
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &v, NULL);
    glCompileShader(vertShader);

    // Creating the fragment shader for use in the program.
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);

    // Creating the shader program.
    *shaderProgram = glCreateProgram();
    glAttachShader(*shaderProgram, vertShader);
    glAttachShader(*shaderProgram, fragShader);

    // Links the shader to the program.
    glLinkProgram(*shaderProgram);

    // Clean up
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
}

int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow((int)width, (int)height, "Zachary Que", NULL, NULL);
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
    DirectionLight directionLight = DirectionLight(lightModel.getPosition(), glm::vec3(0.f), lightModel.getColor(), 0.5f);
    // Point Light: Position is at front of tank
    PointLight pointLight = PointLight(glm::vec3(tank.getPosition().x - 6.5f, tank.getPosition().y + 23.f, tank.getPosition().z - 57.f), glm::vec3(1.f), 200.f);

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

        skybox.draw(mainCamera->getViewMatrix(), mainCamera->getProjMatrix());
        
        glUseProgram(*litShader.getShaderProgram());

        /* Camera (view/projection) to be used */
        if (usePerspective) {
            if (perspective == 3) {
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

                thirdPersonPerspectiveCamera.calculateRotation(pitch, yaw);
            }
            if (perspective == 1) {
                firstPersonPerspectiveCamera.setPos(glm::vec3(tank.getPosition().x, tank.getPosition().y + 30.f, tank.getPosition().z - 10.f));
                firstPersonPerspectiveCamera.setCenter(glm::vec3(firstPersonPerspectiveCamera.getPos().x, firstPersonPerspectiveCamera.getPos().y, firstPersonPerspectiveCamera.getPos().z - 40.f));
                mainCamera = &firstPersonPerspectiveCamera;
            }
        }
        if (useOrtho) {
            mainCamera = &orthoCamera;
        }

        // If projection camera exists, use the projection camera's values
        if (mainCamera != NULL) mainCamera->updateShaderViewProj(litShader.getShaderProgram());

        /* Draw Light Object */
        lightModel.draw(litShader.getShaderProgram(), false);

        /* Light Sources */
        /* Update Point Light Intensity */
        if (pointLightIntensity == 1)       pointLight.setIntensity(lowIntensity);
        else if (pointLightIntensity == 2)  pointLight.setIntensity(medIntensity);
        else                                pointLight.setIntensity(highIntensity);

        /* Lighting in Shader Program */
        directionLight.applyUniqueValuesToShader(litShader.getShaderProgram());
        pointLight.applyUniqueValuesToShader(litShader.getShaderProgram());

        /* Draw Main Object */
        /* Update Main Object */
        if (controllingMainObj) {
            tank.setRotation(glm::vec3(tank.getRotation().x + x_axis_mod,
                tank.getRotation().y + y_axis_mod,
                tank.getRotation().z + z_axis_mod));
            // Point Light is attached to tank
            if (x_axis_mod != 0 || y_axis_mod != 0 || z_axis_mod != 0) {
                glm::vec3 newPos = glm::vec3(tank.getPosition() + tank.getRotation());
                pointLight.setPos(glm::vec3(newPos.x, newPos.y, newPos.z));
            }

            std::cout << pointLight.getPos().x << ", " << pointLight.getPos().y << ", " << pointLight.getPos().z << std::endl;
            x_axis_mod = 0.f;
            y_axis_mod = 0.f;
            z_axis_mod = 0.f;
        }
        tank.draw(litShader.getShaderProgram(), true);

        // Reset lighting to render lightModel unaffected by light
        pointLight.turnOff();
        directionLight.turnOff();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}