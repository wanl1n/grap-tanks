#version 330 core

out vec4 FragColor;

in vec3 texCoord;

uniform samplerCube skybox;
uniform bool nightVision;

void main() {
	
	if (nightVision)
		FragColor = texture(skybox, texCoord) * vec4(0, 1, 0, 1);
	else
		FragColor = texture(skybox, texCoord);

}