#version 330 core

layout(location=0) in vec3 vertexPositionModelSpace;
layout(location=1) in vec4 color;
layout(location=2) in vec3 vertexNormalModelSpace;


out vec4 vColor;
out vec3 positionWorldSpace;
out vec3 normalCameraSpace;
out vec3 eyeDirectionCameraSpace;
out vec3 lightDirectionCameraSpace;

uniform mat4 mvp;
uniform mat4 v;
uniform mat4 m;
uniform vec3 lightPositionWorldSpace;

void main()
{
    //multiply the matrix with the position and make it vec4 by homogenous co-ordinates.
    gl_Position = mvp * vec4(vertexPositionModelSpace * 0.05, 1);
    
    //Set the color
    vColor = color;
    
    positionWorldSpace = (m * vec4(vertexPositionModelSpace * 0.05, 1)).xyz;
    
    vec3 vertexPositionCameraSpace = (v * m * vec4(vertexPositionModelSpace * 0.05, 1)).xyz;
    eyeDirectionCameraSpace = vec3(0,0,0) - vertexPositionCameraSpace;
    
    vec3 lightPositionCameraSpace = (v * vec4(lightPositionWorldSpace, 1)).xyz;
    lightDirectionCameraSpace = lightPositionCameraSpace + eyeDirectionCameraSpace;
    
    normalCameraSpace = (v * m * vec4(vertexNormalModelSpace, 0)).xyz;
    
}
