#version 450

layout (location = 0) in vec2 fragTexCoord;
layout (location = 1) in vec4 fragColor;

layout (location = 0) out vec4 outColor;

layout (set = 0, binding = 0) uniform sampler2D texSampler;
layout(push_constant) uniform Push {
	mat4 transform;
	vec3 color;
} push;

void main() {
	outColor = texture(texSampler, fragTexCoord) * fragColor;
}