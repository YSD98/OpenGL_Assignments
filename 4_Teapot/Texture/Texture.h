//
//  Texture.hpp
//  OpenGLDraw
//
//  Created by rythmos on 16/09/2022.
//


#include <string>
#include <GL/glew.h>
#include "stb_image.h"	

class Texture
{
public:
    Texture(GLenum TextureTarget, const std::string& FileName);
    bool LoadTexture();
    void BindTexture(GLenum TextureUnit);
    
private:
    std::string m_FileName;
    GLenum m_TextureTarget;
    GLuint m_TextureObj;
    
};
