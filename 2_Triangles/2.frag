#version 330 core

out vec4 color;

in vec4 vColor;

void main()
{
	//color = vec4(vColor, 1);	
	color = vColor;	
	color.w = 1.0;
}