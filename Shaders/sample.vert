#version 330 core

// location == 0 is from VertexAttributePointr first attribute
layout(location = 0) in vec3 aPos;

// The normals has attribute position 1
// Access the notmals and assigns it to vertexNormal
layout(location = 1) in vec3 vertexNormal;

// The tex coord / UV is at 0
layout(location = 2) in vec2 aTex;

// Pass the tex coord to fragment shader
out vec2 texCoord;

// Pass the processed normals to the fragment shader later
out vec3 normCoord;

// Pass the position of the vertex to the fragment shader later
out vec3 fragPos;

uniform mat4 transform; 

uniform mat4 projection;

uniform mat4 view;

void main() {
	gl_Position = projection * view * transform * vec4(aPos, 1.0);

	// Assign the UV
	texCoord = aTex;

	// Get the normal matrix and convert it to a 3x3 matrix
	// Apply the normal matrix to the normal data
	normCoord = mat3(transpose(inverse(transform))) * vertexNormal;

	// The position is just your transformation matrix
	// applied to the vertex as vector 3
	fragPos = vec3(transform * vec4(aPos, 1.0));
}