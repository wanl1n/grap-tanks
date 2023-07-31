#include "Model.hpp"

using namespace models;

Model::Model(std::string strObjPath, std::string strTexturePath,
	glm::vec3 pos, glm::vec3 scale, glm::vec3 rotate, glm::vec4 color)
	: pos(pos), scale(scale), rotate(rotate), color(color)
{
	/* Initialize variables for tiny obj loader */
	// Relative path to the mesh
	std::string path = strObjPath;
	// Will contain the mesh's shapes
	std::vector<tinyobj::shape_t> shapes;
	// Will contain the mesh's materials
	std::vector<tinyobj::material_t> materials;
	// Some error messages might popup
	std::string warning, error;
	// Basic attributes related to the mesh
	tinyobj::attrib_t attributes;

	/* load the mesh */
	bool success = tinyobj::LoadObj(
		&attributes,
		&shapes,
		&materials,
		&warning,
		&error,
		path.c_str()
	);

	/* Initialize the Array of Vertex Data */
	// Loop thorugh all thee vertex indices
	for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
		// Assign the index data for easier access
		tinyobj::index_t vData = shapes[0].mesh.indices[i];

		// X
		this->fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3)]);
		// Y
		this->fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3) + 1]);
		// Z
		this->fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3) + 2]);

		// Normals
		this->fullVertexData.push_back(attributes.normals[(vData.normal_index * 3)]);
		this->fullVertexData.push_back(attributes.normals[(vData.normal_index * 3) + 1]);
		this->fullVertexData.push_back(attributes.normals[(vData.normal_index * 3) + 2]);

		// U
		this->fullVertexData.push_back(attributes.texcoords[(vData.texcoord_index * 2)]);
		// V
		this->fullVertexData.push_back(attributes.texcoords[(vData.texcoord_index * 2) + 1]);
	}

	/* Initialize VAO / VBO */
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);

	/* Bind the VAO */
	glBindVertexArray(this->VAO);

	/* Create an Array Buffer to store vertex positions */
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	/* Add the size of our Vertex Array in bytes and the contents to the buffer */
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(GLfloat) * fullVertexData.size(),
		fullVertexData.data(),
		GL_STATIC_DRAW
	);

	/* Position Data */
	glVertexAttribPointer(
		0, // Index 0 is vertex position
		3, // X Y Z 
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(GL_FLOAT),
		(void*)0
	);

	/* Normal Data */
	GLintptr normalPtr = 3 * sizeof(float);
	glVertexAttribPointer(
		1, // Index 1 for normals
		3, // Normals has 3 values
		GL_FLOAT, // Data type of normals
		GL_FALSE,
		8 * sizeof(GL_FLOAT),
		(void*)normalPtr
	);

	/* UV Data */
	GLintptr uvptr = 6 * sizeof(float);
	glVertexAttribPointer(
		2, // Index 2 for tex coordinates / UV
		2, // UV is 2 floats (U,V)
		GL_FLOAT, // Data type of array
		GL_FALSE,
		8 * sizeof(GL_FLOAT),
		(void*)uvptr // Add in the offset
	);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (strTexturePath.length() != 0) {
		/* Variablles for our texture */
		int img_width, img_height, colorChannels;
		// Fix the flipped texture (by default it is flipped)
		stbi_set_flip_vertically_on_load(true);
		/* Load Texture */
		unsigned char* tex_bytes =
			stbi_load(strTexturePath.c_str(), // texture path
				&img_width, // fills out the width
				&img_height, // fills out the height
				&colorChannels, //fills out the color channel
				0);

		/* Setup Texture */
		// Generate a reference
		glGenTextures(1, &this->texture);
		// Set the current texture we're working on to Texture 0
		glActiveTexture(GL_TEXTURE);
		// Bind our next tasks to Tex0
		// To our current reference similar to what we're doing to VBOs
		glBindTexture(GL_TEXTURE_2D, this->texture);

		// If texture is png, color format is RGBA
		if (strTexturePath.find("png") != std::string::npos) {
			/* Assign Texture to Reference */
			// Assign the loaded texture to the OpenGL reference
			glTexImage2D(
				GL_TEXTURE_2D,
				0, // Texture 0
				GL_RGBA, // Target color format of the texture
				img_width, // Texture width
				img_height, // Texture height
				0,
				GL_RGBA, // Color format of the textyre
				GL_UNSIGNED_BYTE,
				tex_bytes // Loaded textures in bytes
			);
		}
		// If texture is jpg, color format is RGB
		else if (strTexturePath.find("jpg") != std::string::npos) {
			/* Assign Texture to Reference */
			// Assign the loaded texture to the OpenGL reference
			glTexImage2D(
				GL_TEXTURE_2D,
				0, // Texture 0
				GL_RGB, // Target color format of the texture
				img_width, // Texture width
				img_height, // Texture height
				0,
				GL_RGB, // Color format of the textyre
				GL_UNSIGNED_BYTE,
				tex_bytes // Loaded textures in bytes
			);
		}

		/* Clean up texture (free memory) */
		// Generate the mipmaps to the current texture
		glGenerateMipmap(GL_TEXTURE_2D);
		// Free up the loaded bytes
		stbi_image_free(tex_bytes);

		// Enable Depth Testing
		glEnable(GL_DEPTH_TEST);
	}
}

glm::vec3 Model::getPosition() {
	return this->pos;
}

glm::vec3 Model::getColor() {
	return this->color;
}

glm::vec3 Model::getRotation() {
	return this->rotate;
}

void Model::setPosition(glm::vec3 pos) {
	this->pos = pos;
}

void Model::setColor(glm::vec4 color) {
	this->color = color;
}

void Model::setRotation(glm::vec3 rotate) {
	this->rotate = rotate;
}

void Model::draw(GLuint* shaderProgram) {

	/* TRANSFORMATION MATRIX */
	glm::mat4 identity_matrix4 = glm::mat4(1.f);
	// Translate
	glm::mat4 transform = glm::translate(identity_matrix4, this->pos);
	// Scale
	transform = glm::scale(transform, glm::vec3(this->scale));
	// Rotate
	transform = glm::rotate(transform, glm::radians(this->rotate.x), glm::vec3(1.f, 0.f, 0.f));
	transform = glm::rotate(transform, glm::radians(this->rotate.y), glm::vec3(0.f, 1.f, 0.f));
	transform = glm::rotate(transform, glm::radians(this->rotate.z), glm::vec3(0.f, 0.f, 1.f));

	unsigned int transformLoc = glGetUniformLocation(*shaderProgram, "transform");
	glUniformMatrix4fv(transformLoc,
		1,
		GL_FALSE,
		glm::value_ptr(transform)
	);

	// Get the location of the tex 0 in the fragment shader
	GLuint tex0Address = glGetUniformLocation(*shaderProgram, "tex0");
	// Tell OpenGl to use the texture
	glBindTexture(GL_TEXTURE_2D, texture);
	// Use the texture at index 0
	glUniform1i(tex0Address, 0);

	/* Draw */
	glBindVertexArray(VAO);
	// Draw using the vertex array
	glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 8);
}