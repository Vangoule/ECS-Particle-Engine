#version 400

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUv;
layout(location = 2) in vec3 vertexNormal;

out vec2 UV;

uniform mat4 MVP;

void main(void)
{
	gl_Position =  MVP * vec4(vertexPosition_modelspace, 1.0f);
	UV = vertexUv;
}