#include "MyCamera.hpp"

using namespace cameras;

// Initialize all the attributes.
MyCamera::MyCamera(glm::vec3 pos, glm::vec3 worldUp, glm::vec3 center) 
	:  pos(pos), worldUp(worldUp), center(center) {
	this->viewMatrix = glm::mat4(1.f);
	this->projMatrix = glm::mat4(1.f);
}

// Passes the attributes to the shader program.
void MyCamera::updateShaderViewProj(GLuint* shaderProgram) {
	this->viewMatrix = glm::lookAt(this->pos, this->center, this->worldUp);

	/* VIEW MATRIX */
	unsigned int viewLoc = glGetUniformLocation(*shaderProgram, "view");
	glUniformMatrix4fv(viewLoc,
		1,
		GL_FALSE,
		glm::value_ptr(this->viewMatrix)
	);

	/* PROJECTION MATRIX */
	unsigned int projLoc = glGetUniformLocation(*shaderProgram, "projection");
	glUniformMatrix4fv(projLoc,
		1,
		GL_FALSE,
		glm::value_ptr(this->projMatrix)
	);
}

glm::vec3 MyCamera::getPos() {
	return this->pos;
}

glm::mat4 MyCamera::getViewMatrix() {
	return this->viewMatrix;
}

glm::mat4 MyCamera::getProjMatrix() {
	return this->projMatrix;
}

glm::vec3 MyCamera::getCameraCenter() {
	return this->center;
}

void MyCamera::setPos(glm::vec3 pos) {
	this->pos = pos;
	this->viewMatrix = glm::lookAt(this->pos, this->center, this->worldUp);
}

void MyCamera::setCenterOffset(glm::vec3 offset) {
	this->center.x += offset.x;
	this->center.y += offset.y;
	this->center.z += offset.z;

	this->viewMatrix = glm::lookAt(this->pos, this->center, this->worldUp);
}

void MyCamera::move(glm::vec3 offset) {
	this->pos += offset;
}