#include<iostream>
#include<GL\glew.h>
#include<GLFW\glfw3.h>
#include<glm.hpp>
#include<gtx/transform.hpp>
#include<gtc/matrix_transform.hpp>

#include "Headers/cyGL.h"
#include "Headers/cyMatrix.h"
#include "Headers/cyVector.h"
#include "Headers/cyTriMesh.h"
#include "Headers/ShaderUtils.h"

GLFWwindow* window;

GLuint programId;
GLuint lightID;

GLuint vertexBuffer;
GLuint colorbuffer;
GLuint elementBuffer;
GLuint normalsBuffer;

GLuint uniformMVP;
GLuint uniformModel;
GLuint uniformView;

cy::Matrix4f Views;
cy::Matrix4f mat4mvp;
cy::Matrix4f mat4Model;
cy::Matrix4f mat4View;

cy::TriMesh teaPot;

void Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    glUseProgram(programId);
    glPointSize(2.8f);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glDrawElements(GL_TRIANGLES, teaPot.NF() * 3, GL_UNSIGNED_INT, 0);      //no. of faces * no. of components(x,y,z)

    glfwSwapBuffers(window);
    glfwPollEvents();
}
void SetMVPMatrix()
{
    cy::Matrix4f Projection = cy::Matrix4f::Perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    //Projection.OrthogonalizeX();

    Views = cy::Matrix4f::View(
        cy::Vec3<float>(0.0f, -5.0, 4.0),
        cy::Vec3<float>(0.0f, 0.0f, 0.0f),
        cy::Vec3 <float>(0.0f, 1.0f, 0.0f)
    );

    cy::Matrix4f Model = cy::Matrix4f(2.5f);

    mat4mvp = Projection * Views * Model;
    uniformMVP = glGetUniformLocation(programId, "mvp");
    uniformModel = glGetUniformLocation(programId, "m");
    uniformView = glGetUniformLocation(programId, "v");

    //Get handle for Light
    glUseProgram(programId);
    lightID = glGetUniformLocation(programId, "lightPositionWorldSpace");
}

void LoadModel()
{
    SetMVPMatrix();
    if (!teaPot.LoadFromFileObj("5_Teapot Shading\\Models\\teapot.obj"))
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

    window = glfwCreateWindow(800, 600, "Shading", NULL, NULL);

    if (window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initiate GLEW\n");
        return -1;
    }
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    //--------------------------------------------------

    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f,-1.0f,0.0f,
        1.0f,-1.0f,0.0f,
        0.0f,1.0f,0.0f
    };
    static const GLfloat g_color_buffer_data[] = {
       1.0f,0.0f,0.0f,
       0.0f,1.0f,0.0f,
       0.0f,0.0f,0.0f
    };

    programId = LoadShader("5_Teapot Shading\\Shaders\\TeaPot.vert", "5_Teapot Shading\\Shaders\\TeaPot.frag");
    LoadModel();

    //VAO creation
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    //GenBuffer, Bind and Set Buffer data of model
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cy::Vec3f) * teaPot.NV(), &teaPot.V(0), GL_STATIC_DRAW);

    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cy::Vec3f) * teaPot.NF(), &teaPot.F(0), GL_STATIC_DRAW);

    glGenBuffers(1, &normalsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cy::Vec3f) * teaPot.NVN(), &teaPot.VN(0), GL_STATIC_DRAW);

    do
    {
        mat4View = Views;
        mat4Model = cy::Matrix4f(1.0f);

        glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &mat4mvp[0]);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, &mat4Model[0]);
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, &mat4View[0]);

        glm::vec3 lightPos = glm::vec3(4, 4, 4);
        glUniform3f(lightID, lightPos.x, lightPos.y, lightPos.z);

        Draw();

    }while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
}