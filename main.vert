#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

// Output data ; will be interpolated for each fragment.
out vec3 FragPos;
out vec3 Normal;
out vec3 lightPos;

// Values that stay constant for the whole mesh.
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 Light;


void main() {

	// Output position of the vertex, in clip space : MVP * position
    gl_Position = Projection * View * Model * vec4(aPos, 1);
    lightPos = Light;
    FragPos = vec3(Model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(Model))) * aNormal;
}
