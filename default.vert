#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 Light;

void main() {
	// mat4 s = mat4(vec4(1, 0, 0, 5), vec4(0, 1, 0, 5), vec4(0, 0, 1, 0), vec4(0, 0, 0, 1));
	// gl_Position = Projection * View * Model * vec4(Light.x, Light.y, Light.z, 1.0);
	gl_Position = Projection * View * Model * vec4(aPos + Light, 1);
}