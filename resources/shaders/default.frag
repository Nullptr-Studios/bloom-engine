#version 450

layout(location = 0) in vec2 f_texCoord;
layout(location = 1) in vec3 f_worldCoord;
layout(location = 2) in vec3 f_worldNormals;

layout (location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform GlobalUBO {
	mat4 projectionMatrix;
	vec4 lightDirection;
	vec4 lightColor;
	vec4 indirectLight;
} ubo;

layout (set = 1, binding = 0) uniform MaterialUBO{
	bool hasAlbedo;
	bool hasRMO;
	bool hasNormals;
	bool hasEmission;
	float roughness;
	float metallic;
	float emissionStrength;
	float occlusionStrength;
	vec4 tint;
} materialUbo;
layout (set = 1, binding = 1) uniform sampler2D albedo;
layout (set = 1, binding = 2) uniform sampler2D RMO;
layout (set = 1, binding = 3) uniform sampler2D normal;
layout (set = 1, binding = 4) uniform sampler2D emission;

layout(push_constant) uniform Push {
	mat4 transform;
	mat4 normalMatrix;
} push;

void main() {
	// Light stuff
	vec3 directionToLight = ubo.lightDirection.xyz - f_worldCoord.xyz;
	float attenuation = 1.0 / dot(directionToLight, directionToLight);
	vec3 normalWorldSpace = normalize(mat3(push.transform) * f_worldNormals);
	vec3 lightColor = ubo.lightColor.rgb * ubo.lightColor.a * attenuation;
	vec3 indirectLight = ubo.indirectLight.rgb * ubo.indirectLight.a;
	vec3 diffuseLight = lightColor * max(dot(normalize(normalWorldSpace), normalize(directionToLight)), 0.0);

	// Disabling texture support for now
	outColor = /*texture(albedo, fragTexCoord) */ vec4(indirectLight + diffuseLight, 1.0f) * materialUbo.tint;
}