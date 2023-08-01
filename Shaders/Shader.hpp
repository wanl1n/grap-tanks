#ifndef SHADERS_SHADER_H
#define SHADERS_SHADER_H

#include "../MainInclude.hpp"


namespace shaders {

	class Shader {
		private:
			GLuint shaderProgram;

		public:
			Shader(const char* pathVert, const char* pathFrag);
			void CreateProgram(const char* pathVert, const char* pathFrag);

		public:
			GLuint* getShaderProgram();
	};
}


#endif