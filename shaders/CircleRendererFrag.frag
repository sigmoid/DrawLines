#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragUV;
layout(location = 2) in float fragInnerRadius;

layout(location = 0) out vec4 outColor;

void main() {
    float circleVal = 0;

    float dist = distance(fragUV, vec2(0.5,0.5));

    if(dist > fragInnerRadius && dist < 0.5)
    {
        circleVal = 1;
    }

    outColor = fragColor * circleVal;
}
