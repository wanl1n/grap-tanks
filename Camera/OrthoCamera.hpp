#ifndef CAMERAS_ORTHO_CAMERA_HPP
#define CAMERAS_ORTHO_CAMERA_HPP

/* Include Files */
#include "../MainInclude.hpp"

// Parent Class
#include "MyCamera.hpp"

namespace cameras {
	class OrthoCamera : public MyCamera {
	// Constructors
	public:
		OrthoCamera(float left, float right, float bottom, float top, float ZNear, float ZFar, 
			glm::vec3 pos = glm::vec3(0.f), glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f), glm::vec3 center = glm::vec3(0.f, 0.f, -5.f));
	public:
		void move(glm::vec3 offset);
	};
}

#endif