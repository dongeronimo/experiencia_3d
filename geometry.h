#pragma once
#include <vtk_glew.h>
#include <vtkOpenGL.h>
#include <vector>
#include <array>
#include <vtkSmartPointer.h>
#include <vtkMatrix4x4.h>
#include "shader.h"

class MyGeometry
{
private:
	GLfloat* vertexBufferData;
	size_t vertexBufferDataSize;

	GLfloat* uvBufferData;
	size_t uvBufferDataSize;

	GLuint vertexArrayId;
	GLuint vertexBufferId;
	GLuint uvBufferId;

	size_t numeroDeElementos;
public:
	MyGeometry(std::vector< std::array<float, 3>> vertexes, std::vector<std::array<float,2>> UVs)
	{
		this->numeroDeElementos = vertexes.size();
		//Cópia pro buffer de coord de textura
		uvBufferDataSize = vertexes.size() * 2 * sizeof(float);
		uvBufferData = (float*)malloc(uvBufferDataSize);
		for (int i = 0; i<vertexes.size(); i++)
		{
			std::array<float, 2> currentTuple = UVs[i];
			const size_t tupleSize = sizeof(float) * 2;
			size_t currentDest = (size_t)uvBufferData + tupleSize * i;
			memcpy((void*)currentDest, currentTuple.data(), tupleSize);
		}
		//Cópia pro buffer dos vértices
		vertexBufferDataSize = vertexes.size() * 3 * sizeof(float);
		vertexBufferData = (float*)malloc(vertexBufferDataSize);
		for (int i = 0; i<vertexes.size(); i++)
		{
			std::array<float, 3> currentTuple = vertexes[i];
			const size_t tupleSize = sizeof(float) * 3;
			size_t currentDest = (size_t)vertexBufferData + tupleSize * i;
			memcpy((void*)currentDest, currentTuple.data(), tupleSize);
		}
		//No final disso aqui todos os dados terão sido copiados pra estruturas internas
		//cria o vertex array object e os buffers dele.
		glGenVertexArrays(1, &vertexArrayId);
		glBindVertexArray(vertexArrayId);
		glGenBuffers(1, &vertexBufferId);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
		glBufferData(GL_ARRAY_BUFFER, vertexBufferDataSize, const_cast<GLfloat*>(vertexBufferData), GL_STATIC_DRAW);
		glGenBuffers(1, &uvBufferId);
		glBindBuffer(GL_ARRAY_BUFFER, uvBufferId);
		glBufferData(GL_ARRAY_BUFFER, uvBufferDataSize, const_cast<GLfloat*>(uvBufferData), GL_STATIC_DRAW);

	}
	void Render(vtkSmartPointer<vtkMatrix4x4> mvp, MyShader* shader, GLuint textureId)
	{
		double _tempMat[16];
		vtkMatrix4x4::DeepCopy(_tempMat, mvp);
		float _mvpMat[16];
		for (int i = 0; i < 16; i++)
			_mvpMat[i] = _tempMat[i];
		glUseProgram(shader->GetProgramId());
		glUniformMatrix4fv(shader->GetUniformByName("mvp"), 1, GL_FALSE, _mvpMat);
		//passagem dos vértices
		glEnableVertexAttribArray(shader->GetAttributeByName("vertexPosition_modelspace"));
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
		glVertexAttribPointer(
			shader->GetAttributeByName("vertexPosition_modelspace"), //O id do atributo no shader
			3,                  // É 3 pq cada tupla tem 3 elementos
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);
		//passagem dos UVs
		glEnableVertexAttribArray(shader->GetAttributeByName("vertexUV"));
		glBindBuffer(GL_ARRAY_BUFFER, uvBufferId);
		glVertexAttribPointer(
			shader->GetAttributeByName("vertexUV"), //O id do atributo no shader
			2,                  // É 3 pq cada tupla tem 3 elementos
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);
		//A textura
		//glEnable(GL_TEXTURE);
		//glEnable(GL_TEXTURE0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glUniform1i(shader->GetUniformByName("textureSampler"), 0);// é 0 pq to usando GL_TEXTURE0
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, numeroDeElementos); // 3 indices starting at 0 -> 1 triangle
		glDisableVertexAttribArray(0);
	}
};
