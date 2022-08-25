#include<iostream>
#include<GL\glew.h>
#include<GLFW\glfw3.h>
#include<glm.hpp>
#include<gtx/transform.hpp>

#include "cyGL.h"
#include "cyMatrix.h"
#include "cyVector.h"
#include "cyTriMesh.h"
#include "ShaderUtils.h"

using namespace std;

GLFWwindow* window;
GLuint VertexArrayID;
GLuint vertexBuffer;
GLuint colorbuffer;
GLuint elementBuffer;
GLuint uniformMVP;
GLuint programId;

cy::TriMesh teaPot;
cy::Matrix4f mat4mvp;

void SetMVPMatrix()
{
    cy::Matrix4f Projection = cy::Matrix4f::Perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    //Projection.OrthogonalizeX();

    cy::Matrix4f Views = cy::Matrix4f::View(
        cy::Vec3<float>(0.0f, -9.0, 4.0),
        cy::Vec3<float>(0.0f, 0.0f, 0.0f),
        cy::Vec3 <float>(0.0f, 1.0f, 0.0f)
    );

    cy::Matrix4f Model = cy::Matrix4f(1.0f);

    mat4mvp = Projection * Views * Model;
    uniformMVP = glGetUniformLocation(programId, "mvp");
}

void LoadModel()
{
    SetMVPMatrix();
    if (!teaPot.LoadFromFileObj("4_Teapot\\teapot.obj"))
    {
        printf("Failed loading the model");
    }
}

int main()
{

    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initiaize GLFW\n");
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1024, 800, "game", NULL, NULL);

    if (window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //---------------------------------------------------------

    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initiate GLEW\n");
        return -1;
    }
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    static const GLfloat g_vertex_buffer_data[] = {
        -1,-1,0,
        1,-1,0,
        0,1,0

    };
    static const GLfloat g_color_buffer_data[] = {

       1.0f,0.0f,0.0f,
       0.0f,1.0f,0.0f,
       0.0f,0.0f,0.0f

    };

    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    LoadModel();

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cy::Vec3f) * teaPot.NV(), &teaPot.V(0), GL_STATIC_DRAW);

    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cy::Vec3f) * teaPot.NF(), &teaPot.F(0), GL_STATIC_DRAW);


    programId = LoadShader("4_Teapot\\TeaPot.vert", "4_Teapot\\TeaPot.frag");

    uniformMVP = glGetUniformLocation(programId, "mvp");

    cy::GLDebugCallback callback;
    callback.Register();
    callback.IgnoreNotifications();
    do
    {

        glClear(GL_COLOR_BUFFER_BIT);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programId);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
        glDrawElements(GL_TRIANGLES, teaPot.NF() * 3, GL_UNSIGNED_INT, 0);      //no. of faces * no. of components(x,y,z)

        glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &mat4mvp[0]);

        //glDrawArrays(GL_POINTS, 0, teaPot.NV());

        glDisableVertexAttribArray(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
}