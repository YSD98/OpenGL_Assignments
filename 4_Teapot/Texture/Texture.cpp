//
//  Texture.cpp
//  OpenGLDraw
//
//  Created by rythmos on 16/09/2022.
//

#include "Texture.h"
#include <iostream>

Texture::Texture(GLenum TextureTarget, const std::string& FileName)
{
    m_TextureTarget = TextureTarget;
    m_FileName = FileName;
}

bool Texture::LoadTexture()
{
    stbi_set_flip_vertically_on_load(1);
    int width = 0;
    int height = 0;
    int bpp = 0;
    
    unsigned char* imageData = stbi_load(m_FileName.c_str(), &width, &height, &bpp, 0);
    
    if(!imageData)
    {
        printf("Cant load texure from %s - %s\n", m_FileName.c_str(), stbi_failure_reason());
        exit(0);
    }
    
    printf("Widhth %d, Height %d, Bpp %d\n", width, height, bpp);
    
    glGenTextures(1, &m_TextureObj);
    glBindTexture(m_TextureTarget, m_TextureObj);
    
    if(m_TextureTarget == GL_TEXTURE_2D)
    {
        glTexImage2D(m_TextureTarget, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    }
    else
    {
        printf("Support of the texture %x is not supported", m_TextureTarget);
    }
    
    glGenerateMipmap(m_TextureTarget);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameterf(m_TextureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP);
    //glTexParameterf(m_TextureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    return true;
}

void Texture::BindTexture(GLenum TextureUnit)
{
    glActiveTexture(m_TextureTarget);
    glBindTexture(m_TextureTarget, m_TextureObj);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(cy::Vec3f)*teaPot.NV(), &teaPot.V(0), GL_STATIC_DRAW);

}
