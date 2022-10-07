//
//  AUtils.h
//  OpenGLDraw
//
//  Created by Ashwin on 04/08/2022.
//

#ifndef AUtils_h
#define AUtils_h

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <GLFW/glfw3.h>

// Include GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
using namespace glm;

#include "cyGL.h"
#include "cyMatrix.h"
#include "cyVector.h"
#include "cyTriMesh.h"


extern GLFWwindow *window;

cy::Vec3f position = cy::Vec3f(0.0, -5.0, 4.0);

float horizontalAngle = 3.14f;
float verticalAngle = 0.0f;
float initialFOV = 45.0f;
float speed = 3.0f;
float mouseSpeed = 0.00005f;


cy::Matrix4f ViewMatrix;
cy::Matrix4f ProjectionMatrix;

cy::Matrix4f getViewMatrix()
{
    return ViewMatrix;
}

cy::Matrix4f getProjectionMatrix()
{
    return ProjectionMatrix;
}

void ComputeMatricesFromInputs()
{
    //cache current time.
    static double sLastTime = glfwGetTime();
    
    //Compute DeltaTime
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - sLastTime);
    
    //Get Mouse Position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    
    //Set to center of the screen for next frame.
    glfwSetCursorPos(window, 1024/2, 800/2);
    
    //Compute new orientation
    horizontalAngle += mouseSpeed * float(1024/2 - xpos);
    verticalAngle += mouseSpeed * float(800/2 - ypos);
    
    //Spherical to cartesian co-ordinate conversion
    cy::Vec3f direction(cos(verticalAngle) * sin(horizontalAngle),
                        sin(verticalAngle),
                        cos(verticalAngle) * cos(horizontalAngle));
    
    //right vector
    cy::Vec3f right = cy::Vec3f(sin(horizontalAngle - 3.14f/2.0f),
                               0,
                               cos(horizontalAngle - 3.14f/2.0f));
    
    cy::Vec3f up = right ^ direction;
    
    
    // Move forward
        if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
            position += direction * deltaTime * speed;
        }
        // Move backward
        if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
            position -= direction * deltaTime * speed;
        }
        // Strafe right
        if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
            position += right * deltaTime * speed;
        }
        // Strafe left
        if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
            position -= right * deltaTime * speed;
        }
    
    float FoV = initialFOV;// - 5 * glfwGetMouseWheel();
    
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    ProjectionMatrix = cy::Matrix4f::Perspective(45.0f, 4.0f/3.0f, 0.1f, 100.0f);
        // Camera matrix
        ViewMatrix = cy::Matrix4f::View(
                                        position, // Camera
                                        position+direction, // target
                                        up);  // Head is up (set to 0,-1,0 to look upside-down)


        // For the next frame, the "last time" will be "now"
        sLastTime = currentTime;

}

GLuint LoadShaders(const char* vertexShaderFilePath, const char* fragmentShaderFilePath)
{
    GLuint vsID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fsID = glCreateShader(GL_FRAGMENT_SHADER);
    
    std::string vsShaderSource;
    std::ifstream vsStream(vertexShaderFilePath, std::ios::in);
    
    if(vsStream.is_open())
    {
        std::stringstream sstr;
        sstr << vsStream.rdbuf();
        vsShaderSource = sstr.str();
        vsStream.close();
    }else
    {
        printf("Impossible to open the vertex shader file please check");
        getchar();
        return 0;
    }
    
    std::string fsShaderSource;
    std::ifstream fsStream(fragmentShaderFilePath, std::ios::in);
    
    if(fsStream.is_open())
    {
        std::stringstream sstr;
        sstr << fsStream.rdbuf();
        fsShaderSource = sstr.str();
        fsStream.close();
    }else{
        printf("Impossible to open the fragment shader file please check");
        getchar();
        return 0;
    }
    
    GLint result = GL_FALSE;
    int infoLogLength;
    
    printf("compiling shader : %s\n", vertexShaderFilePath);
    
    char const *vsShaderData = vsShaderSource.c_str();
    glShaderSource(vsID, 1, &vsShaderData, NULL);
    glCompileShader(vsID);
    
    glGetShaderiv(vsID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vsID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if ( infoLogLength > 0 )
    {
        std::vector<char> VertexShaderErrorMessage(infoLogLength+1);
        glGetShaderInfoLog(vsID, infoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragmentShaderFilePath);
    char const * FragmentSourcePointer = fsShaderSource.c_str();
    glShaderSource(fsID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(fsID);

    // Check Fragment Shader
    glGetShaderiv(fsID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fsID, GL_INFO_LOG_LENGTH, &infoLogLength);
    
    if ( infoLogLength > 0 )
    {
        std::vector<char> FragmentShaderErrorMessage(infoLogLength+1);
        glGetShaderInfoLog(fsID, infoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }

    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, vsID);
    glAttachShader(ProgramID, fsID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &infoLogLength);
    
    if ( infoLogLength > 0 )
    {
        std::vector<char> ProgramErrorMessage(infoLogLength+1);
        glGetProgramInfoLog(ProgramID, infoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }
    
    glDetachShader(ProgramID, vsID);
    glDetachShader(ProgramID, fsID);
    
    glDeleteShader(vsID);
    glDeleteShader(fsID);

    return ProgramID;
}




#endif /* AUtils_h */
