#version 400

in vec2 UV;
out vec4 outputColor;

uniform sampler2D diffuse;

void main(void)
{
    outputColor = vec4(texture(diffuse, UV).rgb, 1.0f);
}