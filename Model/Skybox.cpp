#include "Skybox.hpp"

using namespace models;

// Create a skybox upon initialization.
Skybox::Skybox() : shader("Shaders/skybox.vert", "Shaders/skybox.frag"){
    //Vertices for the cube
    float skyboxVertices[]{
        -1.f, -1.f, 1.f, //0
        1.f, -1.f, 1.f,  //1
        1.f, -1.f, -1.f, //2
        -1.f, -1.f, -1.f,//3
        -1.f, 1.f, 1.f,  //4
        1.f, 1.f, 1.f,   //5
        1.f, 1.f, -1.f,  //6
        -1.f, 1.f, -1.f  //7
    }; this->skyboxVertices = skyboxVertices;

    //Skybox Indices
    unsigned int skyboxIndices[]{
        1,2,6,
        6,5,1,

        0,4,7,
        7,3,0,

        4,5,6,
        6,7,4,

        0,3,2,
        2,1,0,

        0,1,5,
        5,4,0,

        3,7,6,
        6,2,3
    }; this->skyboxIndices = skyboxIndices;

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    std::string faceSkybox[]{
        "Skybox/right.png", // right
        "Skybox/left.png", //left
        "Skybox/top.png", // up
        "Skybox/bottom.png", // down
        "Skybox/front.png", // front 
        "Skybox/back.png", // back
    }; this->faceSkybox = faceSkybox;

    glGenTextures(1, &this->skyboxTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->skyboxTex);

    // Avoid pixelation
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Prevent tiling
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++) {
        int w, h, skyCChannel;
        stbi_set_flip_vertically_on_load(false);

        // Load skybox
        unsigned char* data = stbi_load(faceSkybox[i].c_str(),
            &w,
            &h,
            &skyCChannel,
            0);

        if (data) {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                GL_RGBA,
                w,
                h,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                data
            );
        }

        stbi_image_free(data);
    }

    stbi_set_flip_vertically_on_load(true);
}

// Draw the created shader.
void Skybox::draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, bool night) {
    
    // First set the right shader
    glUseProgram(*this->shader.getShaderProgram());

    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);

    glm::mat4 skyView = glm::mat4(1.f);
    // Strip off the translation of the camera since we only need the rotation.
    skyView = glm::mat4(glm::mat3(viewMatrix));

    // Pass the skybox properties to the skybox shader.
    unsigned int skyProjectionLoc = glGetUniformLocation(*this->shader.getShaderProgram(), "projection");
    glUniformMatrix4fv(skyProjectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    unsigned int skyViewLoc = glGetUniformLocation(*this->shader.getShaderProgram(), "view");
    glUniformMatrix4fv(skyViewLoc, 1, GL_FALSE, glm::value_ptr(skyView));

    // Tell shader if its night vision mode or not.
    unsigned int nightVisionLoc = glGetUniformLocation(*this->shader.getShaderProgram(), "nightVision");
    glUniform1i(nightVisionLoc, night);

    // Draw
    glBindVertexArray(this->VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->skyboxTex);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}