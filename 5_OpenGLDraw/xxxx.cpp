#include<stdio.h>
#include<stdlib.h>
#include<glew.h>
#include<glfw3.h>
#include"shaderutil.h"
#include<glm.hpp>
#include<gtx/transform.hpp>
#include<gtc/matrix_transform.hpp>
#include<vec4.hpp>

#include "cyGL.h"
#include "cyMatrix.h"
#include "cyVector.h"
#include "cyTriMesh.h"

GLFWwindow* window;
GLuint programID;
GLuint vertexBuffer;
GLuint colorBuffer;
GLuint uniformMVP;
GLuint uniformModel;
GLuint uniformView;
GLuint lightID;

GLuint ebuffer;
GLuint normalsBuffer;

cy::Matrix4f mat4mvp;
cy::Matrix4f mat4Model;
cy::Matrix4f mat4View;

cy::Matrix4f Views;

cy::TriMesh teaPot;


void Draw()
{
    // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    glUseProgram(programID);
    glPointSize(2.8f);

    //Draw Triangles
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    //EnableVAO for normals.
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    //glDrawArrays(GL_POINTS, 0, 3);

    //Use elements(indices to triangulate)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebuffer);
    glDrawElements(GL_TRIANGLES, teaPot.NF() * 3, GL_UNSIGNED_INT, 0);


    //Draw the teapot vertices.
    //glDrawArrays(GL_POINTS, 0, teaPot.NV());

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void SetMVPMatrix()
{
    cy::Matrix4f Projection = cy::Matrix4f::Perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

    Views = cy::Matrix4f::View(
        cy::Vec3<float>(0.0f, -5.0, 4.0), // Camera is at         cy::Vec3<float>(0.0f, -5.0, 4.0), // Camera is at
        cy::Vec3<float>(0.0f, 0.0f, 0.0f), // target
        cy::Vec3 <float>(0.0f, 1.0f, 0.0f)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    //Make orthographic projection.
    //Projection.OrthogonalizeX();

    cy::Matrix4f Model = cy::Matrix4f(2.5f);
    mat4mvp = Projection * Views * Model;

    uniformMVP = glGetUniformLocation(programID, "mvp");
    uniformView = glGetUniformLocation(programID, "v");
    uniformModel = glGetUniformLocation(programID, "m");

    //Get handle for Light
    glUseProgram(programID);
    lightID = glGetUniformLocation(programID, "lightPositionWorldSpace");


}

void LoadModel()
{
    SetMVPMatrix();

    if (!teaPot.LoadFromFileObj("teapot.obj"))
    {
        printf("Failed loading the model");
    }
}

int mains()
{
    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "Teapot with Light", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    //Enable Alpha Blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    static const GLfloat gVertexBufferData[] =
    {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };

    static const GLfloat gColorBuffer[] =
    {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
    };

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    programID = LoadShader("vertex.vert", "fragment.frag");

    LoadModel();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    //VAO creation
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //GenBuffer, Bind and Set Buffer data of model
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cy::Vec3f) * teaPot.NV(), &teaPot.V(0), GL_STATIC_DRAW);

    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gColorBuffer), gColorBuffer, GL_STATIC_DRAW);

    //Gen buffers for Elements.
    glGenBuffers(1, &ebuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * teaPot.NF() * 3, &teaPot.F(0), GL_STATIC_DRAW);

    //Gen Normals Buffer
    glGenBuffers(1, &normalsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned int) * teaPot.NVN() * 3, &teaPot.VN(0), GL_STATIC_DRAW);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    do
    {
        //ComputeMatricesFromInputs();
        //SetMVPMatrix();

        mat4Model = Views;
        mat4View = cy::Matrix4f(1.0f);

        //call display function here.
        glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, &mat4mvp[0]);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, &mat4Model[0]);
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, &mat4View[0]);

        glm::vec3 lightPos = glm::vec3(4, 4, 4);
        glUniform3f(lightID, lightPos.x, lightPos.y, lightPos.z);


        Draw();
    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);
}
