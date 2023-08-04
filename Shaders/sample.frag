#version 330 core

// Color to be returned
out vec4 FragColor;

// Texture to be passed
uniform sampler2D tex0;
uniform sampler2D norm_tex;
uniform sampler2D sticker;

// Texture exists
uniform bool tex_exists;
uniform bool sticker_exists;

/* Direction Light Properties */
uniform bool dl_exists;
uniform vec3 dl_direction;
uniform vec3 dl_color;
uniform float dl_multiplier;

uniform	float dl_ambientStr;
uniform vec3 dl_ambientColor;

uniform float dl_specStr;
uniform float dl_specPhong; 

/* Point Light Properties */
uniform bool pl_exists;
uniform vec3 pl_pos;
uniform vec3 pl_color;
uniform float pl_multiplier;

uniform float pl_ambientStr;
uniform vec3 pl_ambientColor;

uniform float pl_specStr;
uniform float pl_specPhong;

uniform vec3 sl_dir;

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

in mat3 TBN;

// Calculate the lighting of the directional light on the color of the object/model.
vec4 calcDirLight(vec3 normal, vec3 viewDir) {

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

    return vec4(dl_multiplier * (dl_specColor + dl_diffuse + dl_ambient), 1.f);
}

// Calculate the lighting of the point light on the color of the object/model.
vec4 calcPointLight(vec3 normal, vec3 viewDir) {
	
    /* Calculate Point Light */
    // light direction
    vec3 pl_lightDir = normalize(pl_pos - fragPos);

    // ambient 
    vec3 pl_ambient = pl_ambientColor * pl_ambientStr;

    // diffuse
    float pl_diff = max(dot(normal, pl_lightDir), 0.f);
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

	// Return the resulting fragment texture with applied point light lighting.
	return vec4(intensity * (pl_specColor + pl_diffuse + pl_ambient), 1.f);
}

// BONUS: Spot light
vec4 calcSpotLight(vec3 normal, vec3 viewDir) {
    
    // Set the intensity of the spot light
    float intensity = 0.8f;

    // Calculate the direction of the spot light.
    vec3 LightToPixel = normalize(pl_pos - fragPos);
    float SpotFactor = dot(LightToPixel, sl_dir);

    // If the pixel is within the cone
    if (SpotFactor > cos(0.5f)) {
        // Calculate the lighting using the point light
        vec4 Color = calcPointLight(normal, viewDir);
        // Return the calculated lighting
        return intensity * Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - cos(0.5f)));
    }
    else {
        // Outside the range of the spot light: return 0 = no light.
        return vec4(0,0,0,0);
    }
}

void main() {
    
    // If transparent, discard pixel.
    vec4 pixelColor = texture(tex0, texCoord);
    if (pixelColor.a < 0.01) {
        discard;
    }
  
    // Normalize the received normals
    //vec3 normal = normalize(normCoord);
    vec3 normal = texture(norm_tex, texCoord).rgb;
	//converts RGB to XYZ
	// 0 == -1
	// 1 == 1
	normal = normalize(normal * 2.0 - 1.0);
	normal = normalize(TBN * normal);

    // Get our view direction from the camera to the fragment
    vec3 viewDir = normalize(cameraPos - fragPos);

    // Initial frag color
    FragColor = vec4(0.f);
   
   // If color exists
    if (any(lessThan(color.xyz, vec3(1.f)))) {
        if (any(greaterThan(color.xyz, vec3(0.f)))) FragColor = color;
    }

    // Lit Objects (with texture)
    if (tex_exists) {

        // If direction light exists
        if (dl_exists)  FragColor += calcDirLight(normal, viewDir) * texture(tex0, texCoord); 
    
        // If point light exists
        if (pl_exists)  FragColor += calcSpotLight(normal, viewDir) * texture(tex0, texCoord); 
            // BONUS: Spot Light
        // To switch to point light:
        // if (pl_exists)  FragColor += calcPointLight(normal, viewDir) * texture(tex0, texCoord); 

        if (sticker_exists) { 
            if (dl_exists)  FragColor += calcDirLight(normal, viewDir) * texture(sticker, texCoord); 
            if (pl_exists)  FragColor += calcSpotLight(normal, viewDir) * texture(sticker, texCoord); 
        }
    }
}