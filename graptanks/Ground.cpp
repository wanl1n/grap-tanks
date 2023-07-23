#include "Ground.hpp"

using namespace models;

Ground::Ground() {
    this->pos = glm::vec3(0, -35, 0); // Take parameter as position.

    // Since model is initially flipped (facing backwards), flip the model.
    this->rotation = glm::vec3(1, 0, 0); // Set axis of rotation to y-axis.
    this->theta = 0.0f; // Rotate around y-axis by 180 degrees to flip the model.

    // Since model is too big, scale it down to 0.1f.
    this->scale = glm::vec3(10.0f, 1.0f, 10.0f);

    this->initialize();
}

void Ground::initialize() {
    int img_width, img_height, color_channels;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* text_bytes = stbi_load("3D/ground textures/AddWater_basecolor.png", // Texture path
        &img_width, // Width of the texture
        &img_height, // height of the texture
        &color_channels, // color channel
        0);


    glGenTextures(1, &this->texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    // If the texture is too small, just clamp it to the edge.
    glTexParameteri(GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_CLAMP_TO_EDGE);


    // Set the texture attributes using the width, height, and texture bytes taken from the loaded texture.
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        img_width,
        img_height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        text_bytes
    );

    // Generate mipmaps/ smaller versions of the texture for faster rendering at further distances.
    glGenerateMipmap(GL_TEXTURE_2D);
    // Free the memmory.
    stbi_image_free(text_bytes);

    glEnable(GL_DEPTH_TEST);

    // Load the 3D model obj file.
    std::string path = "3D/GroundPlane.obj";
    std::vector<tinyobj::shape_t> shape;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;
    tinyobj::attrib_t attributes;

    // Set its attributes from the obj file.
    bool success = tinyobj::LoadObj(
        &attributes,
        &shape,
        &material,
        &warning,
        &error,
        path.c_str()
    );

    // Loading the Position and UV data of the object.
    for (int i = 0; i < shape[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData = shape[0].mesh.indices[i];
        this->fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3]);
        this->fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 1]);
        this->fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 2]);
        this->fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2]);
        this->fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2 + 1]);

    }

    // Generating buffers.
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    // Insert the model vertices (including UV) into the VBO for rendering).
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GLfloat) * this->fullVertexData.size(),
        this->fullVertexData.data(),
        GL_STATIC_DRAW
    );
    // Tell it how to interpret the aboce data.
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(GL_FLOAT),
        (void*)0
    );

    // Create a UV pointer (3 because its at index 3).
    this->uvPtr = 3 * sizeof(float);
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(GLfloat),
        (void*)this->uvPtr
    );

    glEnableVertexAttribArray(0);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Ground::draw(GLuint* shaderProgram) {
    // Create the transformation matrix and apply the transformation attributes at draw.\

    glm::mat4 transformation_matrix = glm::translate(glm::mat4(1.0f), this->pos);
    transformation_matrix = glm::scale(transformation_matrix, this->scale);
    transformation_matrix = glm::rotate(transformation_matrix,
        glm::radians(this->theta),
        this->rotation);

    // Update the shader with the new transform of the object.
    unsigned int transformLoc = glGetUniformLocation(*shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

    GLuint tex0Address = glGetUniformLocation(*shaderProgram, "tex0");
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glUniform1i(tex0Address, 0);

    glBindVertexArray(this->VAO);

    // Get the shaderprogram to be used.
    glUseProgram(*shaderProgram);
    // Draw the object (XYZUV in arrays) with texture.
    glDrawArrays(GL_TRIANGLES, 0, this->fullVertexData.size() / 5);

}

void Ground::updatePosition(glm::vec3 pos) {
    this->pos = pos;
}