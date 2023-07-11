#version 330 core // Version number

layout(location = 0) in vec3 aPos;

// Linear transformation matrix (model matrix)
uniform mat4 transform;

// Projection matrix
uniform mat4 projection;

// Camera matrix (view matrix)
uniform mat4 view;

void main() {

	gl_Position = projection * view * transform * vec4(aPos, 1.0);
}