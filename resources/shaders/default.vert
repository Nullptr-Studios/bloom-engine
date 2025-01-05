#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

layout(location = 0) out vec3 fragNormal;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragColor;

layout(set = 0, binding = 0) uniform GlobalUBO {
    mat4 projectionMatrix;
    vec4 lightDirection;
    vec4 lightColor;
    vec4 indirectLight;
} ubo;

layout(push_constant) uniform Push {
    mat4 transform;
    // TODO: pass tint on the material descriptorset instead and put here mat4 normalMatrix
    vec3 tint;
} push;

void main() {
    vec4 worldPosition = push.transform * vec4(position, 1.0);
    gl_Position = ubo.projectionMatrix * worldPosition;

    // Light stuff
    vec3 directionToLight = ubo.lightDirection.xyz - worldPosition.xyz;
    float attenuation = 1.0 / dot(directionToLight, directionToLight);
    vec3 normalWorldSpace = normalize(mat3(push.transform) * normal);
    vec3 lightColor = ubo.lightColor.rgb * ubo.lightColor.a * attenuation;
    vec3 indirectLight = ubo.indirectLight.rgb * ubo.indirectLight.a;
    vec3 diffuseLight = lightColor * max(dot(normalize(normalWorldSpace), normalize(directionToLight)), 0.0);

    fragColor = push.tint * (diffuseLight + indirectLight);
    fragNormal = normal;
    fragTexCoord = texCoord;
}
