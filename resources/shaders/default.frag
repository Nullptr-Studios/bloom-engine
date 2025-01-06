#version 450
#define MAX_POINT_LIGHTS 16

layout(location = 0) in vec2 f_texCoord;
layout(location = 1) in vec3 f_worldCoord;
layout(location = 2) in vec3 f_worldNormals;

layout (location = 0) out vec4 outColor;

struct PointLight {
	vec4 position; // XYZ position, W radius
	vec4 color; // RGB color, A intensity
};

struct DirectionalLight {
	vec4 direction; // XYZ direction, W padding
	vec4 color; // RGB color, A intensity
};

layout(set = 0, binding = 0) uniform GlobalUBO {
	mat4 projectionMatrix;
	vec4 indirectLight; // RGB color, A intensity
  DirectionalLight directialLight;
  PointLight pointLights[MAX_POINT_LIGHTS];
  int lightCount;
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
	vec3 diffuseLight = ubo.indirectLight.rgb * ubo.indirectLight.a;
	vec3 surfaceNormals = normalize(mat3(push.transform) * f_worldNormals);
	for (int i = 0; i < ubo.lightCount; i++) {
		PointLight light = ubo.pointLights[i];
		vec3 lightDirection = light.position.xyz - f_worldCoord;
		float attenuation = max(0.0f, (1.0f / dot(lightDirection, lightDirection)) - 1.0f / (light.position.w * light.position.w));
		float cosIncidence = max(dot(normalize(surfaceNormals), normalize(lightDirection)), 0.0f);
		vec3 intensity = light.color.rgb * light.color.a * attenuation;

		diffuseLight += intensity * cosIncidence;
	}

	// Disabling texture support for now
	outColor = /*texture(albedo, fragTexCoord) */ vec4(diffuseLight, 1.0f) * materialUbo.tint;
}
