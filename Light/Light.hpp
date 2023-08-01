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

		GLuint stateAddress;

	// Constructor
	public:
		Light(glm::vec3 pos = glm::vec3(0.f), glm::vec3 color = glm::vec3(1.f), float multiplier = 10.f, float ambientStr = 0.1f,
			glm::vec3 ambientColor = glm::vec3(1.f), float specStr = 0.5f, float specPhong = 16.f);

	public:
		void applyToShader(GLuint* shaderProgram, glm::vec3 cameraPos);
		// Will be defined in child class since they have unique implementations.
		virtual void applyUniqueValuesToShader(GLuint* shaderProgram) = 0;
		void moveLight(float x_mod, float y_mod, float z_mod);
		void changeIntensity(float delta);

		virtual void turnOff() = 0;

	// Getters and Setters
	public:
		void setPos(glm::vec3 pos);
		void setColor(glm::vec3 color);
		void setMultiplier(float multiplier);

		void setIntensity(float intensity);

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