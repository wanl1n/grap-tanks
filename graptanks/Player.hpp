#ifndef MODELS_PLAYER_H
#define MODELS_PLAYER_H

#include "main.hpp"

#include "vector"

namespace models {
	class Player {
    private:
        glm::vec3 pos; // position of spawned model.
        glm::vec3 rotation; // axis of rotation
        float theta; // in degrees
        glm::vec3 scale; // scale of the spawned model.
        std::vector<GLfloat> fullVertexData; // vertex data of the model
        GLuint texture;
        GLuint VAO;
        GLuint VBO;
        GLintptr uvPtr;

        // Constructor and draw Function.
    public:
        // Initialize all attributes of an instance.
        Player(glm::vec3 pos);

    public:
        // Draws the object.
        void initialize();
        void draw(GLuint* shaderProgram);
        void updatePosition(glm::vec3 pos);
        void updateRotation(float rotation);
	};
}

#endif