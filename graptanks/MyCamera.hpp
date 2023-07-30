#ifndef CAMERAS_MY_CAMERA_HPP
#define CAMERAS_MY_CAMERA_HPP

#include "main.hpp"

namespace cameras {
	class MyCamera {
		private:
			glm::vec3 persPos;
			glm::vec3 cameraCenter;
			glm::vec3 worldUp;
			glm::vec3 orthoPos;
			glm::vec3 activePos;
			bool orthoActive;

			glm::mat4 viewMatrix;

			glm::mat4 activeProjection;
			glm::mat4 perspective;
			glm::mat4 orthographic;


		public:
			MyCamera();

		public:
			void move(glm::vec3 movement);
			glm::mat4 initializeOrthographic();
			glm::mat4 initializePerspective();
			void enableOrthographic();
			void enablePerspective();
			void run(GLuint* shaderProgram, double x_cursor_pos, double y_cursor_pos);
			
		public:
			glm::mat4 getViewMatrix();
			glm::vec3 getCameraPos();
			void setCenter();
			void moveForward();
			void moveBack();
			void moveLeft();
			void moveRight();
	};
}
#endif