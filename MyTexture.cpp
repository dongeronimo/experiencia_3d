#include "MyTexture.h"

MyTexture::MyTexture(int textureSize, void* buffer)
{
	glGenTextures(1, &textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RED,
		textureSize,
		textureSize,
		0,
		GL_RED,
		GL_UNSIGNED_BYTE,
		buffer
	);
}

MyTexture::~MyTexture()
{
	glDeleteTextures(1, &textureId);
}
