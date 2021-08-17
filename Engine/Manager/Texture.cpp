#include "Texture.h"
#include <cassert>
#include <iostream>


#include "WindowManager.h"

#include <glm/glm.hpp>
std::map<std::string, TextureData*> Texture::s_resourceMap;

TextureData::TextureData(GLenum textureTarget, int width, int height, int numTextures, unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* format, bool clamp, GLenum* attachments)
{
	m_textureID = new GLuint[numTextures];
	m_textureTarget = textureTarget;
	m_numTextures = numTextures;
	m_pixels = new unsigned char[width * height];
	
	m_width = width;
	m_height = height;

	m_frameBuffer = 0;
	m_renderBuffer = 0;
	
	InitTextures(data, filters, internalFormat, format, clamp);

	InitRenderTargets(attachments);
}

TextureData::~TextureData()
{

	if(*m_textureID) glDeleteTextures(m_numTextures, m_textureID);
	if(m_frameBuffer) glDeleteFramebuffers(1, &m_frameBuffer);
	if(m_renderBuffer) glDeleteRenderbuffers(1, &m_renderBuffer);
	if(m_textureID) delete[] m_textureID;

}

void TextureData::InitTextures(unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* format, bool clamp)
{
	glGenTextures(m_numTextures, m_textureID);
	for(int i = 0; i < m_numTextures; i++)
	{
		
		glBindTexture(m_textureTarget, m_textureID[i]);
			
		glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, filters[i]);
		glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, filters[i]);
		
		if(clamp)
		{
			glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glTexImage2D(m_textureTarget, 0, internalFormat[i], m_width, m_height, 0, format[i], GL_UNSIGNED_BYTE, data[i]);

		if(filters[i] == GL_NEAREST_MIPMAP_NEAREST ||
			filters[i] == GL_NEAREST_MIPMAP_LINEAR ||
			filters[i] == GL_LINEAR_MIPMAP_NEAREST ||
			filters[i] == GL_LINEAR_MIPMAP_LINEAR)
		{
			glGenerateMipmap(m_textureTarget);
			GLfloat maxAnisotropy;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
			glTexParameterf(m_textureTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, glm::clamp(0.0f, 8.0f, maxAnisotropy ));
		}
		else
		{
			glTexParameteri(m_textureTarget, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(m_textureTarget, GL_TEXTURE_MAX_LEVEL, 0);
		}
	}
}

void TextureData::InitRenderTargets(GLenum* attachments)
{
	if(attachments == 0)
		return;

	GLenum drawBuffers[32];      //32 is the max number of bound textures in OpenGL
	assert(m_numTextures <= 32); //Assert to be sure no buffer overrun should occur

	bool hasDepth = false;
	
	for(int i = 0; i < m_numTextures; i++)
	{
		if(attachments[i] == GL_DEPTH_ATTACHMENT)
		{
			drawBuffers[i] = GL_NONE;
			hasDepth = true;
		}
		else
			drawBuffers[i] = attachments[i];

		if(attachments[i] == GL_NONE)
			continue;

		if(m_frameBuffer == 0)
		{
			glGenFramebuffers(1, &m_frameBuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[i], m_textureTarget, m_textureID[i], 0);
	}

	if(m_frameBuffer == 0)
		return;

	if(!hasDepth)
	{
		glGenRenderbuffers(1, &m_renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);
	}

	glDrawBuffers(m_numTextures, drawBuffers);

	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Framebuffer creation failed!" << std::endl;
		assert(false);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void TextureData::Bind(int textureNum) const
{
	glBindTexture(m_textureTarget, m_textureID[textureNum]);
}

void TextureData::BindAsRenderTarget() const
{
	glBindTexture(GL_TEXTURE_2D,0);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	glViewport(0, 0, m_width, m_height);

}


void Texture::loadTextureDevIL(const std::string& fileName, GLenum textureTarget, GLfloat filter, GLenum internalFormat, GLenum format, bool clamp, GLenum attachment)
{
	
	
	ILuint imageID;				// Create an image ID as a ULuint
 
	//GLuint textureID;			// Create a texture ID as a GLuint
 
	ILboolean success;			// Create a flag to keep track of success/failure
 
	ILenum error;				// Create a flag to keep track of the IL error state
 
	ilGenImages(1, &imageID); 		// Generate the image ID
 
	ilBindImage(imageID); 			// Bind the image
 
	success = ilLoadImage(m_fileName.c_str()); 	// Load the image file
 
	// If managed to load the image, then start to do things with it...
	if (success)
	{
		// If the image is flipped (i.e. upside-down and mirrored, flip it the right way up!)
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}
		
		success = ilConvertImage(format, IL_UNSIGNED_BYTE);
 
		//if failed the conversion
		if (!success)
		{
			error = ilGetError();
			std::cout << "Image conversion failed - IL reports error: " << error << " - " << iluErrorString(error) << std::endl;
		}
 
		// Generate a new texture
	//	glGenTextures(1, &textureID);
 
		// Bind the texture to a name
	//	glBindTexture(GL_TEXTURE_2D, textureID);

		int width = ilGetInteger(IL_IMAGE_WIDTH);
		int height = ilGetInteger(IL_IMAGE_HEIGHT);
		int colors = ilGetInteger(IL_IMAGE_BPP);

		ILubyte* pixelData = ilGetData();

		unsigned char* data = new unsigned char[(size_t)(width) * (size_t)(height) * (size_t)(colors)];
		memcpy(data, pixelData, (size_t)(width) * (size_t)(height) * (size_t)(colors));

		m_textureData = new TextureData(textureTarget, width, height, 1, &data, &filter, &internalFormat, &format, clamp, &attachment);
		m_textureData->m_pixels = data;

						// Specify the texture specification
	/*	glTexImage2D(GL_TEXTURE_2D, 				// Type of texture
					 0,				// Pyramid level (for mip-mapping) - 0 is the top level
					 ilGetInteger(IL_IMAGE_BPP),	// Image colour depth
					 ilGetInteger(IL_IMAGE_WIDTH),	// Image width
					 ilGetInteger(IL_IMAGE_HEIGHT),	// Image height
					 0,				// Border width in pixels (can either be 1 or 0)
					 ilGetInteger(IL_IMAGE_FORMAT),	// Image format (i.e. RGB, RGBA, BGR etc.)
					 GL_UNSIGNED_BYTE,		// Image data type
					 ilGetData());			// The actual image data itself
		*/
		// Set texture clamping method
/*		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
        glGenerateMipmap(GL_TEXTURE_2D);
 */
	//	m_textureData = new TextureData(GL_TEXTURE_2D);
	//	m_textureData->setTextureId(textureID);
	//	s_resourceMap.insert(std::pair<std::string, TextureData*>(m_fileName, m_textureData));
	
 	}
  	else 
  	{
		error = ilGetError();
		std::cout << "Image load failed - IL reports error: " << error << " - " << iluErrorString(error) << std::endl;
		
  	}
 
 	ilDeleteImages(1, &imageID); 

//	glBindTexture(GL_TEXTURE_2D, 0);
 
//	std::cout << "Texture creation successful." << std::endl;
 
}

Texture::Texture(const std::string& fileName, GLenum textureTarget, GLfloat filter, GLenum internalFormat, GLenum format, bool clamp, GLenum attachment)
{
 	std::string newFileName = ("../Resources/Textures/" + fileName);
	m_fileName = newFileName;
	std::map<std::string, TextureData*>::const_iterator it = s_resourceMap.find(newFileName);
	if(it != s_resourceMap.end())
	{
		m_textureData = it->second;
	}
	else
	{

		loadTextureDevIL(newFileName, textureTarget, filter, internalFormat, format, clamp, attachment);	
		s_resourceMap.insert(std::pair<std::string, TextureData*>(newFileName, m_textureData));
		
	}
}

Texture::Texture(int width, int height, BYTE* data, GLenum textureTarget, GLfloat filter, GLenum internalFormat, GLenum format, bool clamp, GLenum attachment)
{
	m_fileName = "";
	m_textureData = new TextureData(textureTarget, width, height, 1, &data, &filter, &internalFormat, &format, clamp, &attachment);
}

Texture::Texture(const Texture& texture) :
	m_textureData(texture.m_textureData),
	m_fileName(texture.m_fileName)
{

}


Texture::~Texture()
{
	if(m_textureData)
	{
		if(m_fileName.length() > 0)
			s_resourceMap.erase(m_fileName);

		delete m_textureData;
	}
}

void Texture::operator=(Texture texture)
{
	char* temp[sizeof(Texture)/sizeof(char)];
	memcpy(temp, this, sizeof(Texture));
	memcpy(this, &texture, sizeof(Texture));
	memcpy(&texture, temp, sizeof(Texture));
}

void Texture::Bind(unsigned int unit) const
{
	assert(unit >= 0 && unit <= 31);
	glActiveTexture(GL_TEXTURE0 + unit);
	m_textureData->Bind(0);
}

void Texture::BindAsRenderTarget() const
{
	m_textureData->BindAsRenderTarget();
	//glViewport(0, 0, Window::GetWindow()->getSize().x, Window::GetWindow()->getSize().y);
}