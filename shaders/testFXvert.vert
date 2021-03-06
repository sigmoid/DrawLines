#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    float xPadding;
    float yPadding;
} ubo;

layout(location = 0) in vec2 inPosition;

layout(location = 0) out vec2 texCoords;

void main() {
    gl_Position = vec4((inPosition * 2.0) - vec2(1,1), 0.0, 1.0);
    texCoords = inPosition;
}
