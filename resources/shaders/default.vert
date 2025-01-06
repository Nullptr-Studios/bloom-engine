#version 450
#define MAX_POINT_LIGHTS 16

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

layout(location = 0) out vec2 f_texCoord;
layout(location = 1) out vec3 f_worldCoord;
layout(location = 2) out vec3 f_worldNormals;

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
