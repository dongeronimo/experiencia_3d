#pragma once
#include <vtk_glew.h>
#include <vtkOpenGL.h>

class MyTexture
{
private:
	GLuint textureId;
public:
	MyTexture(int textureSize, void* buffer);
	~MyTexture();
	GLuint GetId()
	{
		return textureId;
	}
};