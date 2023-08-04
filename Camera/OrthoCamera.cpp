#include "OrthoCamera.hpp"

using namespace cameras;

// Initialize all the attributes.
OrthoCamera::OrthoCamera(float left, float right, float bottom, float top, float ZNear, float ZFar,
	glm::vec3 pos, glm::vec3 worldUp, glm::vec3 center) 
	: MyCamera(pos, worldUp, center) 
{
	// Create the projMatrix
	this->projMatrix = glm::ortho(left,
		right,
		bottom,
		top,
		ZNear,
		ZFar);
}

// When moving, move the center with the position.
void OrthoCamera::move(glm::vec3 offset) {
	this->pos += offset;
	this->center += offset;
}