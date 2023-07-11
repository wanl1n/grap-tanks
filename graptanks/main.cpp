/*
	Programming Challenge 1
	By: Zachary Que
	3D model source: https://www.cgtrader.com/free-3d-models/exterior/skyscraper/skyscraper-empire-state-building
*/

/* HEADERS */
// glad Header
#include <glad/glad.h>
// GLFW Header
#include <GLFW/glfw3.h>
// Headers to load shaders
#include <string>
#include <iostream>
// GLM Headers for linear transformations
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// Add in tiny_obj_loader
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

/* Model Class */
class Model3D {
    // Atrributes
    public:
        float pos[3];
        float scale[3];
        float rotate[3];
    // Constructor
    public:
        Model3D() {
            // Initialize x y z positions
            this->pos[0] = 0.f;
            this->pos[1] = 0.f;
            this->pos[2] = 0.f; 

            // Initialize x y z scales
            this->scale[0] = 0.75f;
            this->scale[1] = 0.75f;
            this->scale[2] = 0.75f;

            // Initialize x y z rotation in deg
            this->rotate[0] = 0.f;
            this->rotate[1] = 0.f;
            this->rotate[2] = 0.f;
        }
    // Behavior
    public:
        void setPosition(float xpos, float ypos, float zpos) {
            this->pos[0] = xpos;
            this->pos[1] = ypos - 5.f; // 3d model offset
            this->pos[2] = zpos;
        }
        void draw(int vertices, GLuint* shaderProgram) {
            /* Identity matrix */
            glm::mat4 identity_matrix = glm::mat4(1.f);

            /* Transformation */
            glm::mat4 transformation_matrix = glm::translate(identity_matrix, glm::vec3(this->pos[0], this->pos[1], this->pos[2]));
            transformation_matrix = glm::scale(transformation_matrix, glm::vec3(this->scale[0], this->scale[1], this->scale[2]));
            transformation_matrix = glm::rotate(transformation_matrix, glm::radians(this->rotate[0]), glm::vec3(1.f, 0.f, 0.f));
            transformation_matrix = glm::rotate(transformation_matrix, glm::radians(this->rotate[1]), glm::vec3(0.f, 1.f, 0.f));
            transformation_matrix = glm::rotate(transformation_matrix, glm::radians(this->rotate[2]), glm::vec3(0.f, 0.f, 1.f));
            // Get the variable view from vertex shader
            unsigned int transformationLoc = glGetUniformLocation(*shaderProgram, "transform");
            glUniformMatrix4fv(transformationLoc, // Address of the transform variable
                1, // How many values are we modifying
                GL_FALSE, // Transpose?
                glm::value_ptr(transformation_matrix)); // Pointer to the transformation matrix

            /* Draw the models using EBO */
            glDrawElements(GL_TRIANGLES, // Type of primitive to use
                           vertices, // Number of vertices
                           GL_UNSIGNED_INT, // Data type of index
                           0);
        }
};

/** GLOBAL SCOPE VARIABLES **/
/* Window variables */
float width = 600.f; // width of window
float height = 600.f; // height of window

/* Model variables */
std::vector<Model3D*> models; // where the models will be stored

/* Camera variables (initial positions) */
// Camera Eye (camera pos)
glm::vec3 eye = glm::vec3(0.f, 0.f, 15.f); // 15.f for z so model is not too close to camera
// World up direction
glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f); // positive y axis
// center 
glm::vec3 center = glm::vec3(0.f, 0.f, -5.f); // xy at origin
// Camera forward vector
glm::vec3 forward = center - eye; // formula to get forward vector

/* Flags */
bool movingForward = false;
bool movingBackward = false;
bool strafingRight = false;
bool strafingLeft = false;
bool rotatingRight = false;
bool rotatingLeft = false;
bool rotatingUp = false;
bool rotatingDown = false;

/* Rotation variables */
float radius = 20.f;
float pitch = 0.f; // initial pitch is 0 degrees (positive x axis)
float yaw = 270.f; // initial yaw is 270 degrees (-90 degrees of positive x axis; negative z axis)

/* Mouse variables */
double prev_xpos = width / 2;
double prev_ypos = height / 2;

/* Time variables */
double timeSinceLastSpawn = 0.0;

/** FUNCTIONS **/
/* Rotation calculation */
void calculateRotation(float pitch, float yaw) {
    glm::vec3 rotation; // where initial calculations will be stored before normalizing it and assigning it to forward
    // Limit pitch to not be parallel to world up
    if (pitch >= 89.9f)
        pitch = 89.9f;
    if (pitch <= -89.9f)
        pitch = -89.9f;
    // Polar to coordinate calculation and using SOH CAH TOA
    // first triangle in xyz cartesian plane: (points are in origin, xz plane, and y axis)
    // hypotenuse = camera forward (normalized) = length of 1
    // y = sin of pitch angle
    // xz plane = cos of pitch angle
    rotation.y = radius * sin(glm::radians(pitch));
    // second triangle in xyz cartesian plane: (points are in origin, x axis, and z axis)
    // hypotenuse = xz plane = cos of pitch angle
    // x = cos of yaw * cos of pitch
    // z = sin of yaw * cos of pitch
    rotation.x = radius * cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    rotation.z = radius * sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    // Assign resulting rotation to forward
    forward = rotation;

    // new center is vector sum of eye and forward
    center = eye + forward;
}

/* Keyboard Input function */
void Key_Callback(GLFWwindow* window, // Pointer to the window
                  int key, // Keycode of the press
                  int scancode, // Physical position of the press
                  int action, // Either Press / Release
                  int mods) // Which modifier key is held down
{   
    float speed = 0.2f;
    
    /* Press Key */
    // WASD
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        movingForward = true;
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        movingBackward = true;
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        strafingRight = true;
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        strafingLeft = true;
    // Arrow Key
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        rotatingRight = true;
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        rotatingLeft = true;
    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        rotatingUp = true;
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        rotatingDown = true;

    /* Release Key */
    // WASD
    if (key == GLFW_KEY_W && action == GLFW_RELEASE)
        movingForward = false;
    if (key == GLFW_KEY_S && action == GLFW_RELEASE)
        movingBackward = false;
    if (key == GLFW_KEY_D && action == GLFW_RELEASE)
        strafingRight = false;
    if (key == GLFW_KEY_A && action == GLFW_RELEASE)
        strafingLeft = false;
    // Arroy Keys
    if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
        rotatingRight = false;
    if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
        rotatingLeft = false;
    if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
        rotatingUp = false;
    if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
        rotatingDown = false;

    /* Update */
    // If camera is moving forward
    if (movingForward)
        eye += glm::normalize(forward) * speed; // move forward
    // If camera is moving backward
    if (movingBackward)
        eye -= glm::normalize(forward) * speed; // move backward
    // If camera is strafing left/right
    if (strafingRight || strafingLeft) {
        // We need to know the camera's right vector
        // Get the camera forward
        glm::vec3 F = glm::normalize(forward);
        // Get the camera right
        glm::vec3 R = glm::normalize(glm::cross(F, worldUp));
        // If camera is strafing right
        if (strafingRight)
            eye += R * speed; // move eye to positive right vector
        // If camera is strafing left
        if (strafingLeft)
            eye -= R * speed; // move eye to negative right vector
    }
    // If camera is rotating
    if (rotatingUp || rotatingDown || rotatingLeft || rotatingRight) {
        // If rotating up (pitching - rotate around positive x axis)
        if (rotatingUp)
            pitch += speed * 10;
        // If rotating down (pitching - rotate around negative x axis)
        if (rotatingDown)
            pitch -= speed * 10;
        // If rotating left (yawing - rotate around negative y axis)
        if (rotatingRight)
            yaw += speed * 10;
        // If rotating left (yawing - rotate around negative y axis)
        if (rotatingLeft)
            yaw -= speed * 10;

        // Calculate the new camera center
        calculateRotation(pitch, yaw);
    }

    // Pressed pace bar (spawn)
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        // Check for 3 second cooldown
        if (timeSinceLastSpawn >= 3.0) {
            // Create new model
            Model3D* skyscraper = new Model3D;
            skyscraper->setPosition(center.x, center.y, center.z);
            models.push_back(skyscraper);
            glfwSetTime(0); // reset timer (timeSinceLastSpawn) to 0
        }
    }

    // new center is vector sum of eye and forward
    center = eye + forward;
}

/* Mouse position function */
void Mouse_Callback(GLFWwindow* window, // Pointer to the window
                    double xpos, // Mouse x coordinate
                    double ypos) // Mouse y coordinate
{
    float speed = 0.1f;

    // Calculate how much the mouse moved in x direction
    double x_mod = xpos - prev_xpos;
    // Calculate how much the mouse moved in y direction
    double y_mod = prev_ypos - ypos; // as we move to top y decreases

    // Set new pos
    prev_xpos = xpos;
    prev_ypos = ypos;

    // Compute new pitch and yaw angles
    pitch += (float)y_mod * speed;
    yaw += (float)x_mod * speed;

    // Calculate the new camera center
    calculateRotation(pitch, yaw);
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the first 3D model upon run */
    Model3D skyscraper1;
    // compute center
    // forward = center - eye => center = eye + forward
    center = eye + forward;
    skyscraper1.setPosition(center.x, center.y, center.z);
    models.push_back(&skyscraper1);

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

    /* Screen space */
    glViewport(0, 0, (int)width, (int)height);

    /* Process keyboard input */
    glfwSetKeyCallback(window, Key_Callback);

    /* Process mouse location */
    // Set cursor to middle of window upon run
    glfwSetCursorPos(window, width / 2, height / 2);
    glfwSetCursorPosCallback(window, Mouse_Callback);

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

    /* Apply the shaders */
    glUseProgram(shaderProgram);

    /* Initialize variables for tiny obj loader */
    // Relative path to the mesh
    std::string path = "3D/skyscraper.obj";
    // Will contain the mesh's shapes
    std::vector<tinyobj::shape_t> shapes;
    // Will contain the mesh's material
    std::vector<tinyobj::material_t> materials;
    // Some error messages might popup
    std::string warning, error;
    // Basic attributes related to the mesh
    tinyobj::attrib_t attributes;

    /* Load the mesh */
    bool success = tinyobj::LoadObj(&attributes, &shapes, &materials, &warning, &error, path.c_str());

    /* Get the mesh indices for EBO */
    std::vector<GLuint> mesh_indices;
    // Iterate through all the vertices and push the indices to our vector
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
    }

    /* Create VAO / VBO / EBO variables */
    GLuint VAO, VBO, EBO;

    /* Initialize the VAO and VBO and EBO */
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    /* Bind the VAO */
    // any calls after this will automatically point to the VAO
    glBindVertexArray(VAO);

    /* Create an Array Buffer to store our vertex positions */
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    /* Add the size of our Vertex Array in bytes and the contents to the buffer */
    glBufferData(GL_ARRAY_BUFFER, // What data is in the buffer
        sizeof(GL_FLOAT) * attributes.vertices.size(), // Size of the whole buffer in bytes
        &attributes.vertices[0], // Vertex Array
        GL_STATIC_DRAW);

    /* Tell the VAO how to interpret the Array Buffer */
    glVertexAttribPointer(0,
                          3, // Our vertex has 3 properties: X, Y, Z positions
                          GL_FLOAT, // Type of array to expect
                          GL_FALSE, // Normalized?
                          3 * sizeof(float), // Size of vertex data
                          (void*)0);

    /* Create an Element Array Buffer to store the indices */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, // What data is in this buffer
                 sizeof(GLuint) * mesh_indices.size(), // Size of the whole buffer in bytes
                 mesh_indices.data(), // Index Array
                 GL_STATIC_DRAW);

    /* The VAO to use the data above */
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* Projection matirx (perspective) */
    glm::mat4 projection_matrix = glm::perspective(glm::radians(60.f), // FOV
                                            height / width, // Aspect Ratio
                                            0.1f, // Near
                                            100.f); // Far


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Timer */
        timeSinceLastSpawn = glfwGetTime();

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Projection */
        // Get the variable projection from vertex shader
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, // Address of the projection variable
                           1, // How many values are we modifying
                           GL_FALSE, // Transpose? 
                           glm::value_ptr(projection_matrix)); // Pointer to the projection matrix

        /* View */      
        glm::mat4 view_matrix = glm::lookAt(eye, // camera position
                                            center, // where camera is looking at 
                                            worldUp); // up
        // Get the variable view from vertex shader
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, // Address of the view variable
                           1, // How many values are we modifying
                           GL_FALSE, // Transpose?
                           glm::value_ptr(view_matrix)); // Pointer to the view matrix

        /* Bind the VAO to prep for drawing */
        glBindVertexArray(VAO);

        /* Loop through models */
        for (int i = 0; i < models.size(); i++) {
            models[i]->draw(mesh_indices.size(), &shaderProgram); // draw each model
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    /* Clean up */
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    /* Delete all allocated memory */
    // start at index 1 because first model was not created with "new"
    for (int i = 1; i < models.size(); i++) {
        delete models[i];
    }

    glfwTerminate();
    return 0;
}