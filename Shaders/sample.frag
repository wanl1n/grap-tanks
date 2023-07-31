#version 330 core

// Color to be returned
out vec4 FragColor;

// Texture to be passed
uniform sampler2D tex0;

// Texture exists
uniform bool tex_exists;

/* Direction Light */
uniform bool dl_exists;
uniform vec3 dl_direction;
uniform vec3 dl_color;
uniform float dl_multiplier;

uniform	float dl_ambientStr;
uniform vec3 dl_ambientColor;

uniform float dl_specStr;
uniform float dl_specPhong; 

/* Point Light */
uniform bool pl_exists;
uniform vec3 pl_pos;
uniform vec3 pl_color;
uniform float pl_multiplier;

uniform float pl_ambientStr;
uniform vec3 pl_ambientColor;

uniform float pl_specStr;
uniform float pl_specPhong;

// Camera position
uniform vec3 cameraPos;

// Color
uniform vec4 color;

// Should recieve the texCoord from the vertex shader
in vec2 texCoord;

// Receive the processed normals from the vertex shader
in vec3 normCoord;

// Receive the position of the vertex to the vertex shader 
in vec3 fragPos;


void main() {
  
    // Normalize the received normals
    vec3 normal = normalize(normCoord);

    // Get our view direction from the camera to the fragment
    vec3 viewDir = normalize(cameraPos - fragPos);

    /* Calculate Direction light */
    // light direction
    vec3 dl_lightDir = normalize(-dl_direction);
    // ambient
    vec3 dl_ambient = dl_ambientColor * dl_ambientStr;
    // diffuse 
    float dl_diff = max(dot(normal, dl_lightDir), 0.f);
    vec3 dl_diffuse = dl_diff * dl_color;
    // specular
    vec3 dl_relectDir = reflect(-dl_lightDir, normal);
    float dl_spec = pow(max(dot(dl_relectDir, viewDir), 0.1f), dl_specPhong);
    vec3 dl_specColor = dl_spec * dl_specStr * dl_color;

    /* Calculate Point Light */
    // light direction
    vec3 pl_lightDir = normalize(pl_pos - fragPos);
    // ambient 
    vec3 pl_ambient = pl_ambientColor * pl_ambientStr;
    // diffuse
    float pl_diff = max(dot(normal, dl_lightDir), 0.f);
    vec3 pl_diffuse = pl_diff * pl_color;
    // specular
    vec3 pl_relectDir = reflect(-pl_lightDir, normal);
    float pl_spec = pow(max(dot(pl_relectDir, viewDir), 0.1f), pl_specPhong);
    vec3 pl_specColor = pl_spec * pl_specStr * pl_color;
    // Point Light Intensity
    float distance = length(pl_pos - fragPos);
    float intensity = 1.f / (distance * distance);
    // multiplier
    intensity *= pl_multiplier;

    // Initial frag color
    FragColor = vec4(0.f);
   
   // If color exists
   if (any(lessThan(color.xyz, vec3(1.f)))) {
    if (any(greaterThan(color.xyz, vec3(0.f))))
       FragColor = color;
   }

    // Lit Objects (with texture)
    if (tex_exists) {
        // If direction light exists
        if (dl_exists) 
            if (any(greaterThan((dl_specColor + dl_diffuse + dl_ambient), vec3(0.f))))
                FragColor += vec4(dl_multiplier * (dl_specColor + dl_diffuse + dl_ambient), 1.f) * texture(tex0, texCoord); 
    
        // If point light exists
        if (pl_exists) 
            if (any(greaterThan((pl_specColor + pl_diffuse + pl_ambient), vec3(0.f))))
                FragColor += vec4(intensity * (pl_specColor + pl_diffuse + pl_ambient), 1.f) * texture(tex0, texCoord); 
       
    }
}