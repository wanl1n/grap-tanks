#include "Light.hpp"

using namespace lights;

// Initialize attributes
Light::Light(glm::vec3 pos, glm::vec3 color, float multiplier, float ambientStr,
	glm::vec3 ambientColor, float specStr, float specPhong)
	: pos(pos), color(color), ambientColor(ambientColor) {

	this->multiplier = multiplier;
	this->ambientStr = ambientStr;
	this->specStr = specStr;
	this->specPhong = specPhong;
}

// Passes the common properties to the shader program.
void Light::applyToShader(GLuint* shaderProgram, glm::vec3 cameraPos) {

	// Child classes will have a definition for this function 
	// as they have their own unique values to pass.
	this->applyUniqueValuesToShader(shaderProgram);

	// Also pass the camera position
	GLuint cameraPosAddress = glGetUniformLocation(*shaderProgram, "cameraPos");
	glUniform3fv(cameraPosAddress, 1, glm::value_ptr(cameraPos));
}

// Accepts offset values and adds them to the light position.
void Light::moveLight(float x_mod, float y_mod, float z_mod) {
    this->pos += glm::vec3(x_mod, y_mod, z_mod);
}

// Adds the accepted value to the intensity multiplier to alter.
void Light::changeIntensity(float delta) {
    this->multiplier += delta;

    // Limit the intensity so it doesn't go negative and take light away.
    if (this->multiplier <= 0) this->multiplier = 0.01f;
}

// Limit the multiplier so it doesn't go to the negatives.
void Light::setMultiplier(float multiplier) {
	this->multiplier = multiplier;

	if (this->multiplier <= 0.f) this->multiplier = 0.01f;
}

void Light::setPos(glm::vec3 pos) {
	this->pos = pos;
}

void Light::setColor(glm::vec3 color) {
	this->color = color;
	this->ambientColor = color;
}

void Light::setIntensity(float intensity) {
	this->multiplier *= intensity;
}


glm::vec3 Light::getPos() {
	return this->pos;
}

glm::vec3 Light::getColor() {
	return this->color;
}

float Light::getMultiplier() {
	return this->multiplier;
}

float Light::getAmbientStr() {
	return this->ambientStr;
}

glm::vec3 Light::getAmbientColor() {
	return this->ambientColor;
}

float Light::getSpecStr() {
	return this->specStr;
}

float Light::getSpecPhong() {
	return this->specPhong;
}