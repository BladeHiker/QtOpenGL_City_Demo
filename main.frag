#version 330 core
out vec4 FragColor;
// Interpolated values from the vertex shaders
in vec3 FragPos;
in vec3 Normal;
in vec3 lightPos;

// Values that stay constant for the whole mesh.
// uniform sampler2D myTextureSampler;
uniform vec3 viewPos;
uniform int Highlight;

void main() {
	// Output color = color of the texture at the specified UV
    // gl_FragColor = texture2D(myTextureSampler, UV) * ((dis.x + dis.y + dis.z) / 3.0 + 0.1);
    vec3 lightColor = vec3(0.85, 0.86, 0.89);
    vec3 lightColor1 = vec3(0.73, 0.8, 0.95);
    vec3 topColor = vec3(0.38, 0.73, 0.8);
    vec3 backColor = vec3(0.02, 0.2, 0.39);
    vec3 objectColor = (100 - FragPos.y * 0.5) / 100 * backColor + FragPos.y * 0.5 / 100 * topColor;
    if(FragPos.x < 10 && FragPos.x > -10 && FragPos.z < 10 && FragPos.z > -10) {
        objectColor += vec3(0.1882, 0.0941, 0.0078) * (20 - abs(FragPos.x) + abs(FragPos.y)) / 500;
    }

    if(abs(FragPos.x) > 1650 || abs(FragPos.z) > 1650) {
        objectColor = vec3(0.67, 0.93, 0.89);
    }

        // ambient
    float ambientStrength = 0.6;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor1;

    float specularStrength = 0.2;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    if(FragPos.x < 10 && FragPos.x > -10 && FragPos.z < 10 && FragPos.z > -10) {
        result = (ambient * 0.5 + diffuse * 0.02 + specular * 0.2) * objectColor;
    }
    FragColor = vec4(result, 1.0);

    if(abs(FragPos.y - Highlight) < 1 && abs(FragPos.x) < 1500 && abs(FragPos.z) < 1500) {
        FragColor = vec4(vec3(1), (Highlight / 500.0 + 0.5));
    }
}