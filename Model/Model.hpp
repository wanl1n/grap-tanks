#ifndef MODELS_MODEL_H
#define MODELS_MODEL_H

/* Include Files */
#include "../MainInclude.hpp"

namespace models {
	class Model {
		// Attributes
		protected:
			glm::vec3 pos;
			glm::vec3 scale;
			glm::vec3 rotate;
			glm::vec3 color;

			GLuint VAO;
			GLuint VBO;

			GLuint texture;
			GLuint norm_tex;
			GLuint textureIndex;
			std::vector<GLfloat> fullVertexData;
			GLuint* shaderProgram;

			int attribSize;

			GLuint sticker_tex;

		// Constructors
		public:
			Model(std::string strObjPath, const char* pathTex, const char* pathNorm,
				glm::vec3 pos = glm::vec3(0.f), 
				glm::vec3 scale = glm::vec3(1.f), 
				glm::vec3 rotate = glm::vec3(0.f), 
				glm::vec4 color = glm::vec4(0.f));
			void loadModelData(std::string path);
			GLuint loadTexture(const char* path, GLuint texture_ind);
			void loadSticker();
			void generateBuffers();

		// Behavior
		public:
			glm::vec3 getPosition();
			glm::vec3 getColor();
			glm::vec3 getRotation();

			void setPosition(glm::vec3 pos);
			void move(glm::vec3 offset);
			void setColor(glm::vec4 color);
			void setRotation(glm::vec3 rotate);
			void rotateBy(glm::vec3 offset);

			void draw(GLuint* shaderProgram, bool texExists);

	};
}

#endif