#version 400

layout(location = 0) in vec3 vertexPosition_modelspace;

uniform mat4 MVP;

out vec3 pos;

void main(void)
{
	pos = vertexPosition_modelspace;
	gl_Position = MVP * vec4(pos, 1.0f);
	

}