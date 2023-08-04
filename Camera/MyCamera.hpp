#ifndef CAMERAS_MY_CAMERA_HPP
#define CAMERAS_MY_CAMERA_HPP

/* Include Files */
#include "../MainInclude.hpp"

namespace cameras {

	class MyCamera {
		// Attributes
		protected:
			glm::vec3 pos; // Camera Pos/Eye
			glm::vec3 worldUp; // World Up Direction
			glm::vec3 center; // Camera Center

			glm::mat4 viewMatrix; // Camera View Matrix;
			glm::mat4 projMatrix; // Projection Matrix (Orhto/Perspective)

		// Constructors
		public:
			MyCamera(glm::vec3 pos = glm::vec3(0.f), 
					 glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f), 
					 glm::vec3 center = normalize(glm::vec3(0.f, 0.f, -5.f)));
	
		// Behaviors
		public:
			void updateShaderViewProj(GLuint* shaderProgram);
			virtual void move(glm::vec3 offset);

		// Getters/Setters
		public:
			glm::vec3 getPos();
			glm::mat4 getViewMatrix();
			glm::mat4 getProjMatrix();

			// For external calculations
			glm::vec3 getCameraCenter();

			void setPos(glm::vec3 pos);
			void setCenterOffset(glm::vec3 offset);
	};
}

#endif