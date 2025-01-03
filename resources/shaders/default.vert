#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

layout(location = 0) out vec3 fragNormal;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragColor;

layout(set = 0, binding = 0) uniform GlobalUBO {
    mat4 projectionMatrix;
    vec3 lightDirection;
    float ambientIntensity;
} ubo;

layout(push_constant) uniform Push {
    mat4 transform;
    // TODO: pass tint on the material descriptorset instead and put here mat4 normalMatrix
    vec3 tint;
} push;

void main() {
    gl_Position = ubo.projectionMatrix * push.transform * vec4(position, 1.0);
    fragNormal = normal;
    fragTexCoord = texCoord;

    // TODO: This doesnt work with non linear scaling
    vec3 normalWorldSpace = normalize(mat3(push.transform) * normal);
    float lightIntensity = ubo.ambientIntensity + max(dot(normalWorldSpace, normalize(ubo.lightDirection)), 0.0);
    fragColor = push.tint * lightIntensity;
}
