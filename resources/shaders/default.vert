#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

layout(location = 0) out vec2 f_texCoord;
layout(location = 1) out vec3 f_worldCoord;
layout(location = 2) out vec3 f_worldNormals;

layout(set = 0, binding = 0) uniform GlobalUBO {
    mat4 projectionMatrix;
    vec4 lightDirection;
    vec4 lightColor;
    vec4 indirectLight;
} ubo;

layout(push_constant) uniform Push {
    mat4 transform;
    mat4 normalMatrix;
} push;

void main() {
    vec4 worldPosition = push.transform * vec4(position, 1.0);

    // Outputs
    gl_Position = ubo.projectionMatrix * worldPosition;
    f_texCoord = texCoord;
    f_worldCoord = worldPosition.xyz;
    f_worldNormals = normal;
}
