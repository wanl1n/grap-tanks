/* Include files */
#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#include "MainInclude.hpp"

// Models
#include "Model/Model.hpp"

// Cameras
#include "Camera/MyCamera.hpp"
#include "Camera/PerspectiveCamera.hpp"
#include "Camera/OrthoCamera.hpp"

// Lights
#include "Light/Light.hpp"
#include "Light/DirectionLight.hpp"
#include "Light/PointLight.hpp"

#include <iostream>

/* Namespaces */
using namespace models;
using namespace cameras;
using namespace lights;

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

    /* Load the vertex shader file into a string stream */
    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;
    // Add the file stream to the string stream
    vertBuff << vertSrc.rdbuf();
    // Convert the stream to a character array
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    /* Load the fragment shader file into a string stream */
    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;
    // Add the file stream to the string stream
    fragBuff << fragSrc.rdbuf();
    // Convert the stream to a character array
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    /* Create a vertex shader */
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Assign the source to the vertex shader
    glShaderSource(vertexShader, 1, &v, NULL);
    // Compile the vertex shader
    glCompileShader(vertexShader);

    /* Create a fragment shader */
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Assign the source to the fragment shader
    glShaderSource(fragShader, 1, &f, NULL);
    // Compile the fragment shader
    glCompileShader(fragShader);

    /* Create the shader program */
    GLuint shaderProgram = glCreateProgram();
    // Attach the compiled vertex shader
    glAttachShader(shaderProgram, vertexShader);
    // Attach the compiled fragment shader
    glAttachShader(shaderProgram, fragShader);
    // Finalize the compilation of the shaders
    glLinkProgram(shaderProgram);
    // Use the shader program
    glUseProgram(shaderProgram);


    /* Load the vertex shader file into a string stream */
    std::fstream sky_vertSrc("Shaders/skybox.vert");
    std::stringstream sky_vertBuff;
    // Add the file stream to the string stream
    sky_vertBuff << sky_vertSrc.rdbuf();
    // Convert the stream to a character array
    std::string sky_vertS = sky_vertBuff.str();
    const char* sky_v = sky_vertS.c_str();

    /* Load the fragment shader file into a string stream */
    std::fstream sky_fragSrc("Shaders/skybox.frag");
    std::stringstream sky_fragBuff;
    // Add the file stream to the string stream
    sky_fragBuff << sky_fragSrc.rdbuf();
    // Convert the stream to a character array
    std::string sky_fragS = sky_fragBuff.str();
    const char* sky_f = sky_fragS.c_str();

    /* Create a vertex shader */
    GLuint sky_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Assign the source to the vertex shader
    glShaderSource(sky_vertexShader, 1, &sky_v, NULL);
    // Compile the vertex shader
    glCompileShader(sky_vertexShader);

    /* Create a fragment shader */
    GLuint sky_fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Assign the source to the fragment shader
    glShaderSource(sky_fragShader, 1, &sky_f, NULL);
    // Compile the fragment shader
    glCompileShader(sky_fragShader);

    /* Create the skybox program */
    GLuint skyboxProgram = glCreateProgram();
    // Attach the compiled vertex shader
    glAttachShader(skyboxProgram, sky_vertexShader);
    // Attach the compiled fragment shader
    glAttachShader(skyboxProgram, sky_fragShader);
    // Finalize the compilation of the shaders
    glLinkProgram(skyboxProgram);

    /*
      7--------6
     /|       /|
    4--------5 |
    | |      | |
    | 3------|-2
    |/       |/
    0--------1
    */
    //Vertices for the cube
    float skyboxVertices[]{
        -1.f, -1.f, 1.f, //0
        1.f, -1.f, 1.f,  //1
        1.f, -1.f, -1.f, //2
        -1.f, -1.f, -1.f,//3
        -1.f, 1.f, 1.f,  //4
        1.f, 1.f, 1.f,   //5
        1.f, 1.f, -1.f,  //6
        -1.f, 1.f, -1.f  //7
    };

    //Skybox Indices
    unsigned int skyboxIndices[]{
        1,2,6,
        6,5,1,

        0,4,7,
        7,3,0,

        4,5,6,
        6,7,4,

        0,3,2,
        2,1,0,

        0,1,5,
        5,4,0,

        3,7,6,
        6,2,3
    };

    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);

    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    std::string faceSkybox[]{
        "Skybox/right.png", // right
        "Skybox/left.png", //left
        "Skybox/top.png", // up
        "Skybox/bottom.png", // down
        "Skybox/front.png", // front 
        "Skybox/back.png", // back
    };

    unsigned int skyboxTex;
    glGenTextures(1, &skyboxTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

    // Avoid pixelation
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Prevent tiling
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++) {
        int w, h, skyCChannel;
        stbi_set_flip_vertically_on_load(false);

        // Load skybox
        unsigned char* data = stbi_load(faceSkybox[i].c_str(),
            &w,
            &h,
            &skyCChannel,
            0);

        if (data) {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                GL_RGBA,
                w,
                h,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                data
            );
        }

        stbi_image_free(data);
    }

    stbi_set_flip_vertically_on_load(true);

    /* Light Sources */
    // Direction Light: From the Moon 
    DirectionLight directionLight = DirectionLight(lightModel.getPosition(), glm::vec3(0.f), lightModel.getColor(), 0.5f);
    // Point Light: Position is at front of tank
    PointLight pointLight = PointLight(glm::vec3(tank.getPosition().x - 6.5f, tank.getPosition().y + 23.f, tank.getPosition().z - 57.f), glm::vec3(1.f), 200.f);

    /* Camera */
    PerspectiveCamera thirdPersonPerspectiveCamera = PerspectiveCamera(60.f, height, width, 0.1f, 300.f,
        glm::vec3(0.f, 0.f, -10.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, -5.f));
    PerspectiveCamera firstPersonPerspectiveCamera = PerspectiveCamera(60.f, height, width, 0.1f, 500.f,
        glm::vec3(0.f, 0.f, 100.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 5.f));
    OrthoCamera orthoCamera = OrthoCamera(-100.f, 100.f, -100.f, 100.f, -100.f, 1000.f,
        glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, -10.f, 0.f));
    MyCamera* camera = &thirdPersonPerspectiveCamera;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the depth buffer as well
        glUseProgram(skyboxProgram);

        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);

        glm::mat4 skyView = glm::mat4(1.f);
        // Strip off the translation of the camera since we only need the rotation.
        skyView = glm::mat4(glm::mat3(camera->getViewMatrix()));

        unsigned int skyProjectionLoc = glGetUniformLocation(skyboxProgram, "projection");
        glUniformMatrix4fv(skyProjectionLoc, 1, GL_FALSE, glm::value_ptr(camera->getProjMatrix()));

        unsigned int skyViewLoc = glGetUniformLocation(skyboxProgram, "view");
        glUniformMatrix4fv(skyViewLoc, 1, GL_FALSE, glm::value_ptr(skyView));
     
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        glUseProgram(shaderProgram);

        /* Camera (view/projection) to be used */
        if (usePerspective) {
            if (perspective == 3) {
                double xpos, ypos;
                camera = &thirdPersonPerspectiveCamera;
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
                camera = &firstPersonPerspectiveCamera;
            }
        }
        if (useOrtho) {
            camera = &orthoCamera;
        }

        // If projection camera exists, use the projection camera's values
        if (camera != NULL) {
            /* VIEW MATRIX */
            camera->updateViewMatrix();
            unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
            glUniformMatrix4fv(viewLoc,
                1,
                GL_FALSE,
                glm::value_ptr(camera->getViewMatrix())
            );

            /* PROJECTION MATRIX */
            unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
            glUniformMatrix4fv(projLoc,
                1,
                GL_FALSE,
                glm::value_ptr(camera->getProjMatrix())
            );
        }

        /* Draw Light Object */
        GLuint tex_existsAddress = glGetUniformLocation(shaderProgram, "tex_exists");
        glUniform1f(tex_existsAddress, false);

        GLuint colorAddress = glGetUniformLocation(shaderProgram, "color");
        glUniform4fv(colorAddress, 1, glm::value_ptr(lightModel.getColor()));
        lightModel.draw(&shaderProgram);

        /* Light Object Position*/
        lightModel.draw(&shaderProgram);

        // Reset shader program 
        glUniform4fv(colorAddress, 1, glm::value_ptr(tank.getColor()));
        glUniform1f(tex_existsAddress, true);

        /* Light Sources */
        // Direction Light
        directionLight.setMultiplier(directionLight.getMultiplier());

        /* Lighting in Shader Program */
        /* Direction Light */
        // Exists
        GLuint dl_existsAddress = glGetUniformLocation(shaderProgram, "dl_exists");
        glUniform1f(dl_existsAddress, true);
        // Direction
        GLuint dl_directionAddress = glGetUniformLocation(shaderProgram, "dl_direction");
        glUniform3fv(dl_directionAddress, 1, glm::value_ptr(directionLight.getDirection()));
        // Color
        GLuint dl_colorAddress = glGetUniformLocation(shaderProgram, "dl_color");
        glUniform3fv(dl_colorAddress, 1, glm::value_ptr(directionLight.getColor()));
        // Multiplier
        GLuint dl_multiplierAddress = glGetUniformLocation(shaderProgram, "dl_multiplier");
        glUniform1f(dl_multiplierAddress, directionLight.getMultiplier());
        // AmbientStr
        GLuint dl_ambientStrAddress = glGetUniformLocation(shaderProgram, "dl_ambientStr");
        glUniform1f(dl_ambientStrAddress, directionLight.getAmbientStr());
        // AmbientColor
        GLuint dl_ambientColorAddress = glGetUniformLocation(shaderProgram, "dl_ambientColor");
        glUniform3fv(dl_ambientColorAddress, 1, glm::value_ptr(directionLight.getAmbientColor()));
        // SpecStr
        GLuint dl_specStrAddress = glGetUniformLocation(shaderProgram, "dl_specStr");
        glUniform1f(dl_specStrAddress, directionLight.getSpecStr());
        // SpecPhong
        GLuint dl_specPhongAddress = glGetUniformLocation(shaderProgram, "dl_specPhong");
        glUniform1f(dl_specPhongAddress, directionLight.getSpecPhong());

        /* Point Light */
        // Exists
        GLuint pl_existsAddress = glGetUniformLocation(shaderProgram, "pl_exists");
        glUniform1f(pl_existsAddress, true);
        // Pos
        GLuint pl_posAddress = glGetUniformLocation(shaderProgram, "pl_pos");
        glUniform3fv(pl_posAddress, 1, glm::value_ptr(pointLight.getPos()));
        // Color
        GLuint pl_colorAddress = glGetUniformLocation(shaderProgram, "pl_color");
        glUniform3fv(pl_colorAddress, 1, glm::value_ptr(pointLight.getColor()));
        // Multiplier
        GLuint pl_multiplierAddress = glGetUniformLocation(shaderProgram, "pl_multiplier");
        glUniform1f(pl_multiplierAddress, pointLight.getMultiplier());
        // AmbientStr
        GLuint pl_ambientStrAddress = glGetUniformLocation(shaderProgram, "pl_ambientStr");
        glUniform1f(pl_ambientStrAddress, pointLight.getAmbientStr());
        // AmbientColor
        GLuint pl_ambientColorAddress = glGetUniformLocation(shaderProgram, "pl_ambientColor");
        glUniform3fv(pl_ambientColorAddress, 1, glm::value_ptr(pointLight.getAmbientColor()));
        // SpecStr
        GLuint pl_specStrAddress = glGetUniformLocation(shaderProgram, "pl_specStr");
        glUniform1f(pl_specStrAddress, pointLight.getSpecStr());
        // SpecPhong
        GLuint pl_specPhongAddress = glGetUniformLocation(shaderProgram, "pl_specPhong");
        glUniform1f(pl_specPhongAddress, pointLight.getSpecPhong());

        /* Update Point Light Intensity */
        float intensity = 0.f;
        if (pointLightIntensity == 1)
            intensity = lowIntensity;
        else if (pointLightIntensity == 2)
            intensity = medIntensity;
        else
            intensity = highIntensity;
        glUniform1f(pl_multiplierAddress, pointLight.getMultiplier() * intensity);

        /* Draw Main Object */
        /* Update Main Object */
        if (controllingMainObj) {
            tank.setRotation(glm::vec3(tank.getRotation().x + x_axis_mod,
                tank.getRotation().y + y_axis_mod,
                tank.getRotation().z + z_axis_mod));
            x_axis_mod = 0.f;
            y_axis_mod = 0.f;
            z_axis_mod = 0.f;
        }
        tank.draw(&shaderProgram);

        // Reset lighting to render lightModel unaffected by light
        glUniform1f(dl_existsAddress, false);
        glUniform1f(pl_existsAddress, false);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}