#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 clr;
layout(location = 2) in vec3 vertexNormalModelSpace;

out vec4 vColor;
out vec3 normalCameraSpace;
out vec3 positionWorldSpace;
out vec3 eyeDirectionCameraSpace;
out vec3 lightDirectionCameraSpace;

uniform mat4 mvp;
uniform mat4 v;
uniform mat4 m;
uniform vec3 lightPositionWorldSpace;

void main()
{
	gl_Position = mvp * vec4(pos * 0.05, 1);
	vColor = clr;

	positionWorldSpace = (m * vec4(pos * 0.05, 1)).xyz;

	vec3 vertexPositionCameraSpace = (v * m * vec4(pos * 0.05, 1)).xyz;
    eyeDirectionCameraSpace = vec3(0,0,0) - vertexPositionCameraSpace;

	vec3 lightPositionCameraSpace = (v * vec4(lightPositionWorldSpace, 1)).xyz;
	lightDirectionCameraSpace = lightPositionCameraSpace + eyeDirectionCameraSpace;

	normalCameraSpace = (v * m * vec4(vertexNormalModelSpace, 0)).xyz;
}

