#include "PerspectiveCamera.hpp"

using namespace cameras;

// Initialize all attributes.
PerspectiveCamera::PerspectiveCamera(float FOV, float window_height, float window_width, float near, float far,
	glm::vec3 pos, glm::vec3 worldUp, glm::vec3 center)
	: MyCamera(pos, worldUp, center)
{
	this->FOV = FOV;
	this->height = window_height;
	this->width = window_width;
	this->near = near;
	this->far = far;

	this->panRotate = center;

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
	if (rotation.y < 1) rotation.y = 1;

	// second triangle in xyz cartesian plane: (points are in origin, x axis, and z axis)
	// hypotenuse = xz plane = cos of pitch angle
	// x = cos of yaw * cos of pitch
	// z = sin of yaw * cos of pitch
	rotation.x = radius.x * cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	rotation.z = (radius.z + 50) * sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	// Set its position relative to the tank position.
	this->pos = tankPos + glm::vec3(rotation.x, rotation.y, -rotation.z);
	// Make sure its always looking at the tank.
	this->center = tankPos;

	this->worldUp = glm::normalize(glm::vec3(0.f, 1.f, 0.f));

	this->viewMatrix = glm::lookAt(this->pos, this->center, this->worldUp);
}

void PerspectiveCamera::calcKeyRotate(glm::vec3 offset) {

	// First add the offset to the total rotation (*2 for speed).
	this->panRotate += offset * 2.f;

	// Rotation limit para di mabreak ang neck
	if (this->panRotate.y > 35.f) {
		this->panRotate.y = 35.f;
	}
	if (this->panRotate.y < -35.f) {
		this->panRotate.y = -35.f;
	}
	
	// Degree of rotation for every % of the theta
	float theta_tot = 90.f; float radius = 40.f;

	// Yaw and pitch rotation of the center.
	float yaw = glm::radians((panRotate.x / (radius)) * theta_tot);
	float pitch = glm::radians((panRotate.y / (radius)) * theta_tot);

	// Limiting the degree in case of flipping.
	float limit = theta_tot - 0.01f;
	if (yaw > limit) yaw = limit;
	if (yaw < -limit) yaw = -limit;
	if (pitch > limit) pitch = limit;
	if (pitch < -limit) pitch = -limit;

	// Finally get the direction in each axis by using Polar to Cartesian point conversion.
	float xAxisRot = radius * sin(yaw) * cos(pitch);
	float yAxisRot = radius * sin(pitch);
	float zAxisRot = radius * cos(yaw) * cos(pitch);

	// Update the camera center with the new calculated point and the position of the camera as offset
	this->center = this->pos + glm::vec3(xAxisRot, yAxisRot, -zAxisRot);

	this->viewMatrix = glm::lookAt(this->pos, this->center, this->worldUp);
}

// Rotates on the x-axis (for turning)
void PerspectiveCamera::rotateWithTank(float yrot) {
	this->center = this->pos - glm::vec3(cos(glm::radians(yrot)), this->center.y, sin(glm::radians(yrot))) * 50.0f;
	this->center.x -= 10.f;

}

// For setting the center of the camera.
void PerspectiveCamera::setCenter(glm::vec3 offset) {
	this->center = offset * 2.0f;

}

// Increase or decrease the FOV to zoom in and out.
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