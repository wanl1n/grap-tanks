#include "MyCamera.hpp"

using namespace cameras;

MyCamera::MyCamera(glm::vec3 pos, glm::vec3 worldUp, glm::vec3 center) 
	:  pos(pos), worldUp(worldUp), center(center) {
	this->viewMatrix = glm::mat4(1.f);
	this->projMatrix = glm::mat4(1.f);
}

void MyCamera::updateViewMatrix() {
	this->viewMatrix = glm::lookAt(this->pos, this->center, this->worldUp);
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