#include "PointLight.hpp"

using namespace lights;

PointLight::PointLight(glm::vec3 pos, glm::vec3 color, float multiplier, float ambientStr,
	glm::vec3 ambientColor, float specStr, float specPhong)
	: Light(pos, color, multiplier, ambientStr, ambientColor, specStr, specPhong) {

	this->ambientColor = color;
}

// Pretty much the same as in the direction light but this time for the point light properties.
void PointLight::applyUniqueValuesToShader(GLuint* shaderProgram) {
    /* Point Light */
    // Exists
    stateAddress = glGetUniformLocation(*shaderProgram, "pl_exists");
    glUniform1f(stateAddress, true);

    GLuint lightAddress = glGetUniformLocation(*shaderProgram, "pl_pos");
    glUniform3fv(lightAddress, 1, glm::value_ptr(this->pos));

    GLuint lightColorAddress = glGetUniformLocation(*shaderProgram, "pl_color");
    glUniform3fv(lightColorAddress, 1, glm::value_ptr(this->color));

    GLuint intensityMultiplierAddress = glGetUniformLocation(*shaderProgram, "pl_multiplier");
    glUniform1f(intensityMultiplierAddress, this->multiplier);

    // Passes the ambient strength to the shader program.
    GLuint ambientStrAddress = glGetUniformLocation(*shaderProgram, "pl_ambientStr");
    glUniform1f(ambientStrAddress, this->ambientStr);
    // Passes the ambient color to the direction ambient color in the shader program.
    GLuint ambientColorAddress = glGetUniformLocation(*shaderProgram, "pl_ambientColor");
    glUniform3fv(ambientColorAddress, 1, glm::value_ptr(this->ambientColor));

    // Passes the specular light strength to the shader program.
    GLuint specStrAddress = glGetUniformLocation(*shaderProgram, "pl_specStr");
    glUniform1f(specStrAddress, specStr);
    // Passes the specular phong to the shader program.
    GLuint specPhongAddress = glGetUniformLocation(*shaderProgram, "pl_specPhong");
    glUniform1f(specPhongAddress, specPhong);
}

void PointLight::turnOff() {

    glUniform1f(stateAddress, false);
}
