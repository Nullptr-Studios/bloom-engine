#version 450

layout (location = 0) in vec2 fragTexCoord;
layout (location = 1) in vec4 fragColor;

layout (location = 0) out vec4 outColor;

layout (set = 1, binding = 0) uniform sampler2D albedo;
layout (set = 1, binding = 1) uniform sampler2D RMO;
layout (set = 1, binding = 2) uniform sampler2D normal;
layout (set = 1, binding = 3) uniform sampler2D emission;
layout(push_constant) uniform Push {
	mat4 transform;
	vec3 tint;
} push;

void main() {
	outColor = texture(albedo, fragTexCoord);
}