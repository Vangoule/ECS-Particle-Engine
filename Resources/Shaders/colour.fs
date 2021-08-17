#version 400

in vec3 pos;

out vec4 outputColor;

void main(void)
{
	outputColor = vec4(pos, 1.0f);
}