#include<iostream>
#include<GL/glew.h>														// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include<GLFW/glfw3.h>

#include "ShaderUtils.h";

using namespace std;

GLuint vertexBuffer;													//This will identify our vertex buffer
GLuint VertexArrayID;
GLuint colorbuffer;

int main()
{
	glewExperimental = true;											// Needed for core profile
	if (!glfwInit())													// Initialise GLFW
	{
		cout << "Failed to initialize GLFW \n";
		return -1;
	}
	
	//glfwWindowHint(GLFW_SAMPLES, 4);									// 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);						// We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);						// We want OpenGL 3.3
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);				// To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);		// We don't want the old OpenGL 

	//-----------------Open a window and create its OpenGL context-------// 
	GLFWwindow* window;													// (In the accompanying source code, this variable is global for simplicity)
	window = glfwCreateWindow(900, 600, "Triangle 1", NULL, NULL);		
	if (window == NULL)
	{
		cout << "Failed to open GLFW window.If you have an Intel GPU, they are not 3.3 compatible\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);										// Initialize GLEW
	glewExperimental = true;											// Needed in core profile

	if (glewInit() != GLEW_OK) {
		cout <<"Failed to initialize GLEW\n";
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);				// Ensure we can capture the escape key being pressed below

	static const GLfloat g_vertex_buffer_data[] = {						// An array of 3 vectors which represents 3 vertices
		-0.5, -0.5, 0.0,												//First Vertex
		 0.5, -0.5, 0.0,												//Second Vertex
		 0.0, 0.5, 0.0,													//Third Vertex
	};

	static const GLfloat g_color_buffer_data[] = {						//Colour Array
		1.0f, 0.4f, 0.5f,												//rgb for first vertex
		0.49f, 0.73f, 0.91f,											//rgb for Second vertex
		1.0f, 1.0f, 0.5f,												//rgb for Third vertex
	};

	glGenVertexArrays(1, &VertexArrayID);								
	glBindVertexArray(VertexArrayID);

	glGenBuffers(1, &vertexBuffer);										// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);						// The following commands will talk about our 'vertexbuffer' buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);// Give our vertices to OpenGL.

	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	GLuint programId = LoadShader("2_Triangles\\2.vert", "2_Triangles\\2.frag");
	//GLint intensityUniformLocation = glGetUniformLocation(programId, "intensity");

	do
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programId);

		glEnableVertexAttribArray(0);									// 1st attribute buffer : vertices
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(
			0,															// attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,															// size
			GL_FLOAT,													// type
			GL_FALSE,													// normalized?
			0,															// stride
			(void*)0													// array buffer offset
		);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


		//glUniform1f(intensityUniformLocation, 0.1f);

		glDrawArrays(GL_TRIANGLES, 0, 3);								// Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
}
