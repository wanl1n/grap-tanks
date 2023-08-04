#include "DirectionLight.hpp"

using namespace lights;

DirectionLight::DirectionLight(glm::vec3 pos, glm::vec3 pointsAt, glm::vec3 color, float multiplier, float ambientStr,
	glm::vec3 ambientColor, float specStr, float specPhong) 
	: Light(pos, color, multiplier, ambientStr,
		ambientColor, specStr, specPhong)
{
	// Compute Direction 
    this->direction = this->pos;

    // Set the ambient strength to 0.05f so it's not too strong.
    this->ambientStr = 0.05f;
}

// Pass the light properties to the shader. 
// (Has unique values so it doesn't overlap with the other light sources)
void DirectionLight::applyUniqueValuesToShader(GLuint* shaderProgram) {

    /* Direction Light */
    // Exists
    stateAddress = glGetUniformLocation(*shaderProgram, "dl_exists");
    glUniform1f(stateAddress, true);
    // Direction
    GLuint dl_directionAddress = glGetUniformLocation(*shaderProgram, "dl_direction");
    glUniform3fv(dl_directionAddress, 1, glm::value_ptr(this->direction));

    // Passes the light color to the direction light color in the shader program.
    GLuint lightColorAddress = glGetUniformLocation(*shaderProgram, "dl_color");
    glUniform3fv(lightColorAddress, 1, glm::value_ptr(this->color));

    // Passes the intensity multiplier to the direction light intensity multiplier in the shader program.
    GLuint intensityMultiplierAddress = glGetUniformLocation(*shaderProgram, "dl_multiplier");
    glUniform1f(intensityMultiplierAddress, this->multiplier);

    // Passes the ambient strength to the shader program.
    GLuint ambientStrAddress = glGetUniformLocation(*shaderProgram, "dl_ambientStr");
    glUniform1f(ambientStrAddress, this->ambientStr);
    // Passes the ambient color to the direction ambient color in the shader program.
    GLuint ambientColorAddress = glGetUniformLocation(*shaderProgram, "dl_ambientColor");
    glUniform3fv(ambientColorAddress, 1, glm::value_ptr(ambientColor));

    // Passes the specular light strength to the shader program.
    GLuint specStrAddress = glGetUniformLocation(*shaderProgram, "dl_specStr");
    glUniform1f(specStrAddress, specStr);
    // Passes the specular phong to the shader program.
    GLuint specPhongAddress = glGetUniformLocation(*shaderProgram, "dl_specPhong");
    glUniform1f(specPhongAddress, specPhong);
}

// Turn off the light (for resetting)
void DirectionLight::turnOff() {
    glUniform1f(stateAddress, false);
}

// Getter for direction
glm::vec3 DirectionLight::getDirection() {
	return this->direction;
}