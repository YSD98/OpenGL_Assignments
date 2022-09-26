#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

GLuint LoadShader(const char* vertex_file_path, const char* fragment_file_path)
{
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	string vertexShaderCode;
	ifstream VertexShaderStream(vertex_file_path, ios::in);
	if (VertexShaderStream.is_open())
	{
		stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		vertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		cout << "Impossible to open" << endl; 
		return 0;
	}

	string fragmentShaderCode;
	ifstream FragmentShaderStream(fragment_file_path, ios::in);
	if (FragmentShaderStream.is_open())
	{
		stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		fragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	printf("Compiling shader %s", vertex_file_path);
	char const* VertexSourcePointer = vertexShaderCode.c_str();
	glShaderSource(vertexShaderId, 1, &VertexSourcePointer, NULL);
	glCompileShader(vertexShaderId);
	
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		vector<char> vertexShaderMessage(InfoLogLength + 1);
		glGetShaderInfoLog(vertexShaderId, InfoLogLength, NULL, &vertexShaderMessage[0]);
		printf("%s\n", &vertexShaderMessage[0]);
	}

	printf("Compliling shader %s", fragment_file_path);
	char const* FragmentSourcePointer = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderId, 1, &FragmentSourcePointer, NULL);
	glCompileShader(fragmentShaderId);

	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		vector<char> fragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(fragmentShaderId, InfoLogLength, NULL, &fragmentShaderErrorMessage[0]);
		printf("%s\n", &fragmentShaderErrorMessage[0]);
	}

	printf("linking program\n");
	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);

	glGetProgramiv(programId, GL_LINK_STATUS, &Result);
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(programId, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(programId, vertexShaderId);
	glDetachShader(programId, fragmentShaderId);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	return programId;
}