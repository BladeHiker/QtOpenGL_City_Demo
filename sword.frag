#version 330 core
out vec4 FragColor;
// Interpolated values from the vertex shaders
in vec3 FragPos;
in vec3 Normal;
in vec3 lightPos;

// Values that stay constant for the whole mesh.
// uniform sampler2D myTextureSampler;
uniform vec3 viewPos;

void main() {
	// Output color = color of the texture at the specified UV
    // gl_FragColor = texture2D(myTextureSampler, UV) * ((dis.x + dis.y + dis.z) / 3.0 + 0.1);
    vec3 lightColor = vec3(1);
    vec3 objectColor = vec3(0.83, 0.24, 0.24);
    vec3 lightPos2 = vec3(50, 500, 50);
    vec3 lightPos3 = vec3(-50, 500, -50);
        // ambient
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;
    vec3 result = ambient;
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    result += diffuse;

    lightDir = normalize(lightPos2 - FragPos);
    diff = max(dot(norm, lightDir), 0.0);
    diffuse = diff * lightColor;
    result += diffuse;
    
    lightDir = normalize(lightPos3 - FragPos);
    diff = max(dot(norm, lightDir), 0.0);
    diffuse = diff * lightColor;
    result += diffuse;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    result += specular * 10;

    result = result * objectColor;
    FragColor = vec4(result, 1.0);
}