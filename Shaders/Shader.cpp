#include "Shader.hpp"

using namespace shaders;

Shader::Shader(const char* pathVert, const char* pathFrag) {
    this->CreateProgram(pathVert, pathFrag);
}

void Shader::CreateProgram(const char* pathVert, const char* pathFrag) {

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
    this->shaderProgram = glCreateProgram();
    glAttachShader(this->shaderProgram, vertShader);
    glAttachShader(this->shaderProgram, fragShader);

    // Links the shader to the program.
    glLinkProgram(this->shaderProgram);

    // Clean up
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
}

GLuint* Shader::getShaderProgram() {
    return &this->shaderProgram;
}