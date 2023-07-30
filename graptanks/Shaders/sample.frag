#version 330 core

out vec4 FragColor;

uniform float r;
uniform float g;
uniform float b;

// Texture to be passed
uniform sampler2D tex0;

// Should receive the texCoord from the vertex shader.
in vec2 texCoord;

uniform vec4 color;

in vec3 normCoord;
in vec3 fragPos;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform float ambientStr;
uniform vec3 ambientColor;

uniform vec3 cameraPos;
uniform float specStr;
uniform float specPhong;

void main() {
	vec3 ambientCol = ambientColor * ambientStr;
	vec3 normal = normalize(normCoord);
	vec3 lightDir = normalize(lightPos - fragPos);
	//vec3 lightDir = normalize(lightPos);

	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	
	float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);
	vec3 specColor = spec * specStr * lightColor;

	FragColor = vec4(diffuse + ambientCol + specColor,1.0) * texture(tex0, texCoord);
	//FragColor = vec4(diffuse + ambientCol + specColor,1.0) * vec4(0.5f, 0.5f, 0.0f, 1.0f);



	//FragColor = vec4(1.0f + r, 0.75f + g, 0.796f + b, 1.0f); //rgba

	// Assign the texture color using the function
	//FragColor = texture(tex0, texCoord);

	//FragColor = vec4(0.5f, 0.5f, 0.0f, 1.0f);
}