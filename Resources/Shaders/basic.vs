#version 400

layout(location = 0) in vec3 vertexPosition_modelspace;

void main(void)
{
	pos = vertexPosition_modelspace;
	gl_Position = vec4(pos, 1.0f);
}