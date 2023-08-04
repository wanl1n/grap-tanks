#ifndef LIGHTS_DIRECTION_LIGHT_HPP
#define LIGHTS_DIRECTION_LIGHT_HPP

/* Include Files */
#include "../MainInclude.hpp"

// Parent Class
#include "Light.hpp"

namespace lights {
	class DirectionLight : public Light {
		// Atributes
		private: 
			glm::vec3 pointsAt;

			glm::vec3 direction;

		// Constructor
		public:
			DirectionLight(glm::vec3 pos = glm::vec3(0.f), 
							glm::vec3 pointsAt = glm::vec3(0.f), 
							glm::vec3 color = glm::vec3(1.f), 
							float multipler = 1.f, 
							float ambientStr = 0.1f,
							glm::vec3 ambientColor = glm::vec3(1.f), 
							float specStr = 0.5f, 
							float specPhong = 16.f);

		public:
			void applyUniqueValuesToShader(GLuint* shaderProgram);
			void turnOff();

		// Getters/Setters
		public:
			glm::vec3 getDirection();
	};
}

#endif