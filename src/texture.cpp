#include "GL/glew.h"

#include "texture.h"
#include "vendor/stb_image/stb_image.h"
#include "Renderer.h"

Texture::Texture(const std::string& path)
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr)
{
	//flipp the picture cause it will be flipped
	stbi_set_flip_vertically_on_load(1);
	
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 3);
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D,m_RendererID);

	GLCall((GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)); // for scaling
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)); // for scaling
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	
	
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width,m_Height,0, GL_RGBA ,GL_UNSIGNED_BYTE ,m_LocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);

}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	glActiveTexture (GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
