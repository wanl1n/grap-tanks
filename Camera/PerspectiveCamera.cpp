#include "PerspectiveCamera.hpp"

using namespace cameras;

PerspectiveCamera::PerspectiveCamera(float FOV, float window_height, float window_width, float near, float far,
	glm::vec3 pos, glm::vec3 worldUp, glm::vec3 center)
	: MyCamera(pos, worldUp, center)
{
	this->FOV = FOV;
	this->height = window_height;
	this->width = window_width;
	this->near = near;
	this->far = far;

	// Create the ProjMatrix
	this->projMatrix = glm::perspective(
		glm::radians(this->FOV),
		this->height / this->width,
		near,
		far
	);
}

void PerspectiveCamera::calcMouseRotate(float pitch, float yaw) {
	
	// Limit angles
	if (pitch >= 89.9f) pitch = 89.9f;
	if (pitch <= -89.9f) pitch = -89.9f;

	glm::vec3 rotation;
	double radius = 150.f;

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
	rotation.z = (radius + 50) * sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	this->pos = glm::vec3(rotation.x, rotation.y, -rotation.z);

	this->worldUp = glm::normalize(glm::vec3(0.f, 1.f, 0.f));

	this->viewMatrix = glm::lookAt(this->pos, this->center, this->worldUp);
}

void PerspectiveCamera::calcKeyRotate(glm::vec2 offset) {
	this->center.x += offset.x;
	this->center.y += offset.y;

	this->viewMatrix = glm::lookAt(this->pos, this->center, this->worldUp);
}

void PerspectiveCamera::zoom(float delta) {
	this->FOV += delta;

	// Limit the FOV
	if (this->FOV <= 6.f) this->FOV = 6.f;
	if (this->FOV >= 180.f) this->FOV = 180.f;

	// Create the ProjMatrix
	this->projMatrix = glm::perspective(
		glm::radians(this->FOV),
		this->height / this->width,
		near,
		far
	);
}
