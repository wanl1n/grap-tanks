#version 330 core

// location == 0 is from VertexAttributePointr first attribute
layout(location = 0) in vec3 aPos;

// The normals has attribute position 1
// Access the notmals and assigns it to vertexNormal
layout(location = 1) in vec3 vertexNormal;

// The tex coord / UV is at 0
layout(location = 2) in vec2 aTex;

// Tangent and Bitangent locations
layout(location = 3) in vec3 m_tan;
layout(location = 4) in vec3 m_btan;

// Pass the tex coord to fragment shader
out vec2 texCoord;

// Pass the processed normals to the fragment shader later
out vec3 normCoord;

// Pass the position of the vertex to the fragment shader later
out vec3 fragPos;

// this is the matrix that will 
// convert tangent space to object space
out mat3 TBN;

uniform mat4 transform; 

uniform mat4 projection;

uniform mat4 view;

void main() {
	gl_Position = projection * view * transform * vec4(aPos, 1.0);

	// Assign the UV
	texCoord = aTex;

	// Get the Normal Matrix and convert it to a 3x3 matrix
	// Apply the normal matrix to the normal data.
	mat3 modelMat = mat3(transpose(inverse(transform)));
	normCoord = modelMat * vertexNormal;

	// Tangent
	vec3 T = normalize(modelMat * m_tan);
	//Bitangent
	vec3 B = normalize(modelMat * m_btan);
	//Normal
	vec3 N = normalize(normCoord);
	//Assign the TBN output
	TBN = mat3(T,B,N);

	//The position is just your transformation matrix
	//applied to the vertex as a vector 3
	fragPos = vec3(transform * vec4(aPos, 1.0));
}