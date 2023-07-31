#ifndef MODELS_MODEL_H
#define MODELS_MODEL_H

/* Include Files */
#include "../MainInclude.hpp"

namespace models {
	class Model {
		// Attributes
	private:
		glm::vec3 pos;
		glm::vec3 scale;
		glm::vec3 rotate;
		glm::vec3 color;

		GLuint VAO;
		GLuint VBO;
		GLuint texture;
		std::vector<GLfloat> fullVertexData;

		// Constructors
	public:
		Model(std::string strObjPath, std::string strTexturePath,
			glm::vec3 pos = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f), glm::vec3 rotate = glm::vec3(0.f), glm::vec4 color = glm::vec4(0.f));

		// Behavior
	public:
		glm::vec3 getPosition();
		glm::vec3 getColor();
		glm::vec3 getRotation();
		void setPosition(glm::vec3 pos);
		void setColor(glm::vec4 color);
		void setRotation(glm::vec3 rotate);

		void draw(GLuint* shaderProgram);

	};
}

#endif