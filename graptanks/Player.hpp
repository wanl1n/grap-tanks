#ifndef MODELS_PLAYER_H
#define MODELS_PLAYER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "vector"

namespace models {
	class Player {
    private:
        glm::vec3 pos; // position of spawned model.
        glm::vec3 rotation; // axis of rotation
        float theta; // in degrees
        glm::vec3 scale; // scale of the spawned model.

        // Constructor and draw Function.
    public:
        // Initialize all attributes of an instance.
        Player(glm::vec3 pos);

        // Draws the object.
        void draw(GLuint* shaderProgram, std::vector<GLfloat> fullVertexData);

        void updatePosition(glm::vec3 pos);
	};
}

#endif