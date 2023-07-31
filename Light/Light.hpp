#ifndef LIGHTS_LIGHT_HPP
#define LIGHTS_LIGHT_HPP

/* Include Files */
#include "../MainInclude.hpp"

namespace lights {
	class Light {
	// Attributes
	protected:
		// Light variables
		glm::vec3 pos;
		glm::vec3 color;
		float multiplier;

		// Ambient variables
		float ambientStr;
		glm::vec3 ambientColor;

		// Specular variables
		float specStr;
		float specPhong;

	// Constructor
	public:
		Light(glm::vec3 pos = glm::vec3(0.f), glm::vec3 color = glm::vec3(1.f), float multiplier = 10.f, float ambientStr = 0.1f,
			glm::vec3 ambientColor = glm::vec3(1.f), float specStr = 0.5f, float specPhong = 16.f);

	// Getters and Setters
	public:
		void setPos(glm::vec3 pos);
		void setColor(glm::vec3 color);
		void setMultiplier(float multiplier);
		glm::vec3 getPos();
		glm::vec3 getColor();
		float getMultiplier();

		float getAmbientStr();
		glm::vec3 getAmbientColor();

		float getSpecStr();
		float getSpecPhong();
	};
}


#endif