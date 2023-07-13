#include "Player.hpp"

using namespace models;

Player::Player(glm::vec3 pos) {
    this->pos = pos; // Take parameter as position.

    // Since model is initially flipped (facing backwards), flip the model.
    this->rotation = glm::vec3(0, 1, 0); // Set axis of rotation to y-axis.
    this->theta = 180.f; // Rotate around y-axis by 180 degrees to flip the model.

    // Since model is too big, scale it down to 0.1f.
    this->scale = glm::vec3(0.1f, 0.1f, 0.1f);
}

void Player::draw(GLuint* shaderProgram, std::vector<GLfloat> fullVertexData) {
    // Create the transformation matrix and apply the transformation attributes at draw.
    glm::mat4 transformation_matrix = glm::translate(glm::mat4(1.0f), this->pos);
    transformation_matrix = glm::scale(transformation_matrix, this->scale);
    transformation_matrix = glm::rotate(transformation_matrix,
        glm::radians(this->theta),
        this->rotation);

    // Update the shader with the new transform of the object.
    unsigned int transformLoc = glGetUniformLocation(*shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

    // Get the shaderprogram to be used.
    glUseProgram(*shaderProgram);
    // Draw the object (XYZUV in arrays) with texture.
    glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 5);
}

void Player::updatePosition(glm::vec3 pos) {

}