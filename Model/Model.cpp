#include "Model.hpp"

using namespace models;

// Initialize all the attributes of model
Model::Model(std::string strObjPath, const char* pathTex, const char* pathNorm,
	glm::vec3 pos, glm::vec3 scale, glm::vec3 rotate, glm::vec4 color)
	: pos(pos), scale(scale), rotate(rotate), color(color)
{
	// Loading the .obj file.
	this->loadModelData(strObjPath);

	// If these textures exist, load them too.
	if (pathTex != "") this->texture = loadTexture(pathTex, GL_TEXTURE0);
	if (pathNorm != "") this->norm_tex = loadTexture(pathNorm, GL_TEXTURE1);

	// Set the initial value of the sticker texture to 0. -- BONUS
	this->sticker_tex = 0;

	// Enable Depth Testing
	glEnable(GL_DEPTH_TEST);
}


// Loads the model data through the accepted path.
void Model::loadModelData(std::string path) {

	// use tinyobj to load the file.
	std::vector<tinyobj::shape_t> shape;
	std::vector<tinyobj::material_t> material;
	std::string warning, error;
	tinyobj::attrib_t attributes;

	bool success = tinyobj::LoadObj(
		&attributes,
		&shape,
		&material,
		&warning,
		&error,
		path.c_str()
	);

	//Vector to hold our tangents
	std::vector<glm::vec3> tangents;
	//Vector to hold our bitangents
	std::vector<glm::vec3> bitangents;

	for (int i = 0; i < shape[0].mesh.indices.size(); i += 3) {

		// Get the 3 vertex data for the triangle
		tinyobj::index_t vData1 = shape[0].mesh.indices[i];
		tinyobj::index_t vData2 = shape[0].mesh.indices[i + 1];
		tinyobj::index_t vData3 = shape[0].mesh.indices[i + 2];

		// Position of vertex 1
		glm::vec3 v1 = glm::vec3(attributes.vertices[vData1.vertex_index * 3],
			attributes.vertices[vData1.vertex_index * 3 + 1],
			attributes.vertices[vData1.vertex_index * 3 + 2]);
		// Position of vertex 2
		glm::vec3 v2 = glm::vec3(attributes.vertices[vData2.vertex_index * 3],
			attributes.vertices[vData2.vertex_index * 3 + 1],
			attributes.vertices[vData2.vertex_index * 3 + 2]);
		//position of vertex 3
		glm::vec3 v3 = glm::vec3(attributes.vertices[vData3.vertex_index * 3],
			attributes.vertices[vData3.vertex_index * 3 + 1],
			attributes.vertices[vData3.vertex_index * 3 + 2]);

		// Position of uv 1
		glm::vec2 uv1 = glm::vec2(attributes.texcoords[vData1.texcoord_index * 2],
			attributes.texcoords[vData1.texcoord_index * 2+ 1] );
		// Position of uv 2
		glm::vec2 uv2 = glm::vec2(attributes.texcoords[vData2.texcoord_index * 2],
			attributes.texcoords[vData2.texcoord_index * 2 + 1]);
		//position of uv 3
		glm::vec2 uv3 = glm::vec2(attributes.texcoords[vData3.texcoord_index * 2],
			attributes.texcoords[vData3.texcoord_index * 2 + 1]);

		glm::vec3 deltaPos1 = v2 - v1;
		glm::vec3 deltaPos2 = v3 - v1;

		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		float r = 1.0f / ((deltaUV1.x * deltaUV2.y) - (deltaUV1.y * deltaUV2.x));

		// Tangent (T)
		glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
		// Bitangent (B)
		glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

		tangents.push_back(tangent);
		tangents.push_back(tangent);
		tangents.push_back(tangent);

		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
	}

	// Loading the UV data of the object.
	for (int i = 0; i < shape[0].mesh.indices.size(); i++) {
		tinyobj::index_t vData = shape[0].mesh.indices[i];

		// This will generate an array with 3 consecutive points 
		// as a position coordinate ordered in the vector.
		fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3]);// X
		fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 1]);// Y
		fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 2]);// Z

		// Add normals here
		fullVertexData.push_back(attributes.normals[vData.normal_index * 3]);// X
		fullVertexData.push_back(attributes.normals[vData.normal_index * 3 + 1]);// Y
		fullVertexData.push_back(attributes.normals[vData.normal_index * 3 + 2]);// Z

		// UV
		fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2]);
		fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2 + 1]);

		fullVertexData.push_back(tangents[i].x);
		fullVertexData.push_back(tangents[i].y);
		fullVertexData.push_back(tangents[i].z);

		fullVertexData.push_back(bitangents[i].x);
		fullVertexData.push_back(bitangents[i].y);
		fullVertexData.push_back(bitangents[i].z);
		// Result: 3 Pos, 3 Norm, 2 UV, 3 Pos, 3 Norm, 2 UV, and so on.
	}

	attribSize = 5;
	if (!attributes.normals.empty()) { attribSize += 3; }

	// Generate the buffers.
	generateBuffers();
}

// Generate the buffers for the vertices and rendering.
void Model::generateBuffers() {

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * fullVertexData.size(), fullVertexData.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GL_FLOAT), (void*)0);

	GLintptr normPtr = 3 * sizeof(float);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (void*)normPtr);

	GLintptr uvPtr = 6 * sizeof(float);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (void*)uvPtr);

	GLintptr tangentPtr = 8 * sizeof(float);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (void*)tangentPtr);

	GLintptr bitangentPtr = 11 * sizeof(float);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (void*)bitangentPtr);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1); // for Normals
	glEnableVertexAttribArray(2); // 2 for UV / Texture
	glEnableVertexAttribArray(3); // 3 for Tangent
	glEnableVertexAttribArray(4); // 4 for Bitangent

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Wala nang ginagalaw sa VBO.
	glBindVertexArray(0); // Wala ka nang ginagalaw na VAO.
}

// Loads the texture of the model given in the path.
GLuint Model::loadTexture(const char* path, GLuint texture_ind) {
	int img_width, img_height, color_channels; // Width, Height, and color channels of the Texture.

	// Fix the flipped texture (by default it is flipped).
	stbi_set_flip_vertically_on_load(true);
	// Load the texture and fill out the variables.
	unsigned char* text_bytes = stbi_load(path, // Texture path
		&img_width, // Width of the texture
		&img_height, // height of the texture
		&color_channels, // color channel
		0);

	GLuint texture;
	// Generate a reference.
	glGenTextures(1, &texture);
	// Set the current texture we're working on to Texture 0.
	glActiveTexture(texture_ind);
	// Bind our next tasks to Tex0 to our current reference similar to VBOs.
	glBindTexture(GL_TEXTURE_2D, texture);
	//If you want to set how the texture maps on a different size model
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_S, // XY = ST (s for x, t for y)
		GL_CLAMP_TO_EDGE //GL_CLAMP_TO_EDGE for stretch, 
	);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_T, // XY = ST (s for x, t for y)
		GL_REPEAT //GL_CLAMP_TO_EDGE for stretch, 
	);

	// Checks how many color channels there are to either set the rgb mode to with or with the alpha.
	unsigned int rgb = GL_RGB;
	if (color_channels == 3) {
		rgb = GL_RGB;
	}
	else if (color_channels == 4) {
		rgb = GL_RGBA;
	}

	//Assign the loaded texture to the OpenGL reference.
	glTexImage2D(
		GL_TEXTURE_2D,
		0, // Texture 0
		rgb, // Target color format of the texture.
		img_width, // Texture width
		img_height, // Texture height
		0,
		rgb, // Color format of the texture
		GL_UNSIGNED_BYTE,
		text_bytes // loaded texture in bytes
	);

	// Generate the mipmaps to the current texture
	glGenerateMipmap(GL_TEXTURE_2D);

	// Free up the loaded bytes.
	stbi_image_free(text_bytes);

	return texture;
}

// Loads the sticker texture (the file is constant for single use but this can be easily 
// made reusable by adding it as a parameter similar to the normals)
void Model::loadSticker() {
	int img_width, img_height, color_channels; // Width, Height, and color channels of the Texture.

	// Fix the flipped texture (by default it is flipped).
	stbi_set_flip_vertically_on_load(true);
	// Load the texture and fill out the variables.
	unsigned char* text_bytes = stbi_load("3D/sticker.png", // Texture path
		&img_width, // Width of the texture
		&img_height, // height of the texture
		&color_channels, // color channel
		0);

	// Generate a reference.
	glGenTextures(1, &sticker_tex);
	// Set the current texture we're working on to Texture 0.
	glActiveTexture(GL_TEXTURE2);
	// Bind our next tasks to Tex0 to our current reference similar to VBOs.
	glBindTexture(GL_TEXTURE_2D, sticker_tex);
	//If you want to set how the texture maps on a different size model
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_S, // XY = ST (s for x, t for y)
		GL_CLAMP_TO_EDGE //GL_CLAMP_TO_EDGE for stretch, 
	);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_T, // XY = ST (s for x, t for y)
		GL_REPEAT //GL_CLAMP_TO_EDGE for stretch, 
	);

	// Checks how many color channels there are to either set the rgb mode to with or with the alpha.
	unsigned int rgb = GL_RGB;
	if (color_channels == 3) {
		rgb = GL_RGB;
	}
	else if (color_channels == 4) {
		rgb = GL_RGBA;
	}

	//Assign the loaded texture to the OpenGL reference.
	glTexImage2D(
		GL_TEXTURE_2D,
		0, // Texture 0
		rgb, // Target color format of the texture.
		img_width, // Texture width
		img_height, // Texture height
		0,
		rgb, // Color format of the texture
		GL_UNSIGNED_BYTE,
		text_bytes // loaded texture in bytes
	);

	// Generate the mipmaps to the current texture
	glGenerateMipmap(GL_TEXTURE_2D);

	// Free up the loaded bytes.
	stbi_image_free(text_bytes);
}

// Very straightforward getters and setters
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

void Model::rotateBy(glm::vec3 offset) {
	this->rotate += offset;
}

// The draw function sets the uniforms and draws the object.
void Model::draw(GLuint* shaderProgram, bool texExists) {

	// First check if texture exists.
	GLuint tex_existsAddress = glGetUniformLocation(*shaderProgram, "tex_exists");
	glUniform1f(tex_existsAddress, texExists);

	// Pass the color.
	GLuint colorAddress = glGetUniformLocation(*shaderProgram, "color");
	glUniform4fv(colorAddress, 1, glm::value_ptr(this->color));

	// TRANSFORMATION MATRIX 
	glm::mat4 identity_matrix4 = glm::mat4(1.f);
	// Translate
	glm::mat4 transform = glm::translate(identity_matrix4, this->pos);
	// Scale
	transform = glm::scale(transform, glm::vec3(this->scale));
	// Rotate
	transform = glm::rotate(transform, glm::radians(this->rotate.x), glm::vec3(1.f, 0.f, 0.f)); // X-axis
	transform = glm::rotate(transform, glm::radians(this->rotate.y), glm::vec3(0.f, 1.f, 0.f)); // Y-axis
	transform = glm::rotate(transform, glm::radians(this->rotate.z), glm::vec3(0.f, 0.f, 1.f)); // Z-axis

	// Pass the transform
	unsigned int transformLoc = glGetUniformLocation(*shaderProgram, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	// Set the active texture to 0 and pass it to the shader
	glActiveTexture(GL_TEXTURE0);
	GLuint tex0Address = glGetUniformLocation(*shaderProgram, "tex0");
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(tex0Address, 0);

	// Set the active texture to 1 and pass it to the shader
	glActiveTexture(GL_TEXTURE1);
	GLuint tex1Address = glGetUniformLocation(*shaderProgram, "norm_tex");
	glBindTexture(GL_TEXTURE_2D, norm_tex);
	glUniform1i(tex1Address, 1);

	// First check if the sticker texture exists (if it was loaded)
	if (this->sticker_tex != 0) {
		// Tell shader it exists
		GLuint sticker_existsAddress = glGetUniformLocation(*shaderProgram, "sticker_exists");
		glUniform1f(sticker_existsAddress, true);

		// Pass the sticker texture
		glActiveTexture(GL_TEXTURE2);
		GLuint stickerAddress = glGetUniformLocation(*shaderProgram, "sticker");
		glBindTexture(GL_TEXTURE_2D, this->sticker_tex);
		glUniform1i(stickerAddress, 2);
	}
	else {
		// Tell the shader it does not exist
		GLuint sticker_existsAddress = glGetUniformLocation(*shaderProgram, "sticker_exists");
		glUniform1f(sticker_existsAddress, false);
	}

	/* Draw */
	glBindVertexArray(VAO);
	if (attribSize == 14) std::cout << attribSize << std::endl;
	// Draw using the vertex array
	glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / attribSize);

	// Reset to default
	glUniform1f(tex_existsAddress, true);
}

// Move the model but make sure it doesn't go past the ground
void Model::move(glm::vec3 offset) {
	this->pos += offset;

	if (this->pos.y <= 0) this->pos.y = 0;
}