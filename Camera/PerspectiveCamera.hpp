#ifndef CAMERAS_PERSPECTIVE_CAMERA_HPP
#define CAMERAS_PERSPECTIVE_CAMERA_HPP

/* Include Files */
#include "../MainInclude.hpp"

// Parent Class
#include "MyCamera.hpp"

namespace cameras {
	class PerspectiveCamera : public MyCamera {
	// Constructors
	public:
		PerspectiveCamera(float FOV, float window_height, float window_width, float near, float far,
			glm::vec3 pos = glm::vec3(0.f), glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f), glm::vec3 center = glm::vec3(0.f, 0.f, -5.f));
	// Behavior
	public:
		void calculateRotation(float pitch, float yaw);
	};
}

#endif