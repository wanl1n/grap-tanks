#ifndef MODELS_SKYBOX_H
#define MODELS_SKYBOX_H

#include "../MainInclude.hpp"
#include "../Shaders/Shader.hpp"

namespace models {

	using namespace shaders;

	class Skybox {

		private:
			float* skyboxVertices;
			unsigned int* skyboxIndices;
		
			std::string* faceSkybox;
			unsigned int skyboxTex;

			GLuint VAO;
			GLuint VBO;
			GLuint EBO;

			Shader shader;

		public:
			Skybox();
			void draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, bool night);
	};
}

#endif