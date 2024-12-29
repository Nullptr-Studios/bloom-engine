#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 color;

layout(location = 0) out vec2 fragTexCoord;
layout(location = 1) out vec4 fragColor;

layout(set = 0, binding = 0) uniform GlobalUBO {
    mat4 projectionMatrix;
    vec4 colorUniform;
} ubo;
layout(push_constant) uniform Push {
    mat4 transform;
    vec3 color;
} push;

void main() {
    gl_Position = ubo.projectionMatrix * push.transform * vec4(position, 1.0);
    fragColor = color;
    fragTexCoord = texCoord;
}
