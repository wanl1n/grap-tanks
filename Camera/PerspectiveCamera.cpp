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

void PerspectiveCamera::calcMouseRotate(float pitch, float yaw, glm::vec3 tankPos) {
	
	// Limit angles
	if (pitch >= 89.9f) pitch = 89.9f;
	if (pitch <= -89.9f) pitch = -89.9f;

	glm::vec3 rotation;
	glm::vec3 radius = glm::vec3(150.f);

	// Polar to coordinate calculation and using SOH CAH TOA
    // first triangle in xyz cartesian plane: (points are in origin, xz plane, and y axis)
    // hypotenuse = camera forward (normalized) = length of 1
    // y = sin of pitch angle
    // xz plane = cos of pitch angle
	rotation.y = radius.y * sin(glm::radians(pitch));

	// second triangle in xyz cartesian plane: (points are in origin, x axis, and z axis)
	// hypotenuse = xz plane = cos of pitch angle
	// x = cos of yaw * cos of pitch
	// z = sin of yaw * cos of pitch
	rotation.x = radius.x * cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	rotation.z = (radius.z + 50) * sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	this->pos = tankPos + glm::vec3(rotation.x, rotation.y, -rotation.z);
	this->center = tankPos;

	this->worldUp = glm::normalize(glm::vec3(0.f, 1.f, 0.f));

	this->viewMatrix = glm::lookAt(this->pos, this->center, this->worldUp);
}

void PerspectiveCamera::calcKeyRotate(glm::vec3 offset) {

	this->center.x += offset.x;
	this->center.y += offset.y;
	this->center.z += offset.z;

	float theta_tot = 360.f;
	float radius = 10.f;

	float yaw = glm::radians((center.x / (width / 2)) * theta_tot);
	float pitch = glm::radians((center.y / (height / 2)) * theta_tot);

	// Limiting the degree in case of flipping.
	float limit = theta_tot - 0.1f;
	if (yaw > limit) yaw = limit;
	if (yaw < -limit) yaw = -limit;
	if (pitch > limit) pitch = limit;
	if (pitch < -limit) pitch = -limit;

	// Finally get the direction in each axis by using Polar to Cartesian point conversion.
	float xAxisRot = radius * sin(yaw) * cos(pitch);
	//float xAxisRot = this->center.x + radius * sin(offset.x);
	float yAxisRot = radius * sin(pitch);
	float zAxisRot = radius * cos(yaw) * cos(pitch);
	//float zAxisRot = this->center.z + radius * (1 - cos(offset.x));

	// Update the camera center with the new calculated point.
	// Finally, make sure to add the strafing movement of the camera to the x-axis.
	glm::vec3 cameraCenter = glm::vec3(xAxisRot, yAxisRot, zAxisRot);

	// Next, calculate the position change based on where the camera center is.
	glm::vec3 worldUp = glm::normalize(glm::vec3(0, 1.f, 0));

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

void PerspectiveCamera::move(glm::vec3 offset) {
	this->pos += offset;
}