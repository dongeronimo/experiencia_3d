#define ITKV3_COMPATIBILITY//Necessário pq as classes foram escritas para uma versão antiga da biblioteca
#pragma once
#ifdef OPENGL_2
	#include <vtk_glew.h>
#endif
#include <vtkOpenGL.h>
#include <vtkProp.h>
#include <vtkSmartPointer.h>
#include <vtkImageImport.h>
#include <vtkMatrix4x4.h>

#include <itkImage.h>
#include <itkCastImageFilter.h>
#include "itkObliqueSectionImageFilter.h"
#include <itkTileImageFilter.h>
#include <itkImageFileWriter.h>
#include <itkMaximumProjectionImageFilter.h>
#include <itkIntensityWindowingImageFilter.h>

#include <array>
#include <vector>
#include <map>
#include <string>
#include <memory>

using namespace std;

typedef itk::Image<short, 3> Input3DImageType;
typedef itk::Image<float, 3> Internal3DImageType;
typedef itk::CastImageFilter<Input3DImageType, Internal3DImageType> InputToInternalCasterType;
typedef itk::Image<float, 2> ImageResliceResultType;
typedef itk::ObliqueSectionImageFilter< float, float > ObliqueFilterType;
typedef itk::MaximumProjectionImageFilter< itk::Image<float, 3>, itk::Image<float, 2> > MipFilterType;
typedef itk::IntensityWindowingImageFilter<itk::Image<float, 2>, itk::Image<unsigned char, 2>> WLFilterType;
typedef itk::ImageFileWriter< itk::Image< float, 3> > SlabWriterType;
typedef itk::ImageFileWriter< itk::Image<unsigned char, 2> > WriterType;
typedef itk::TileImageFilter<itk::Image<float, 2>, itk::Image<float, 3>> TileFilterType;
//TODO: Versão opengl 1.5 disso tudo
#ifdef OPENGL_2
//Só funciona se estiver com o backend de opengl2.
class MyShader
{
private:
	GLuint programId;
	GLuint vsId;
	GLuint fsId;
	map<string, GLuint> attributes;
	map<string, GLuint> uniforms;
string GetShaderInfoLog(GLuint object, PFNGLGETSHADERIVPROC glGet__iv, PFNGLGETSHADERINFOLOGPROC glGet__InfoLog)
{
	GLint log_length;
	char *log;
	glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
	log = static_cast<char*>(malloc(log_length));
	glGet__InfoLog(object, log_length, nullptr, log);
	return log;
}
GLuint MakeShader(GLenum type, std::string source)
{
	GLint length[] = { source.length() };
	const GLchar *srcLn = source.c_str();
	GLuint shader;
	GLint shader_ok;
	shader = glCreateShader(type);
	glShaderSource(shader, 1, &srcLn, length);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
	string _probl = GetShaderInfoLog(shader, glGetShaderiv, glGetShaderInfoLog);
	cout << _probl << endl;
	if (!shader_ok)
	{
		glDeleteShader(shader);
		throw std::runtime_error(_probl.c_str());
	}
	else
	{
		return shader;
	}
}
GLuint MakeProgram(GLuint vertex_shader, GLuint fragment_shader)
{
	GLint program_ok;
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
	if (!program_ok) {
		string _probl = GetShaderInfoLog(program, glGetProgramiv, glGetProgramInfoLog);
		glDeleteProgram(program);
		throw std::runtime_error(_probl.c_str());
	}
	return program;
}
void IntrospectProgram(GLuint programId, map<string, GLuint> &attributes, map<string, GLuint> &uniforms)
{
	GLint numberOfAttributes, largestAttributeName;
	glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTES, &numberOfAttributes);
	glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &largestAttributeName);
	//para cada atributo, pegar as propriedades e guardar.
	for (GLuint i = 0; i < numberOfAttributes; i++)
	{
		char* nameBuffer = new char[largestAttributeName];
		GLsizei length;
		GLint size;
		GLenum type;
		//Pega, entre outras coisas, o nome do atributo.
		glGetActiveAttrib(programId, i, largestAttributeName, &length, &size, &type, nameBuffer);
		GLint attribLocation = glGetAttribLocation(programId, nameBuffer);
		std::string _name(nameBuffer);
		attributes.insert(std::make_pair(_name, attribLocation));
	}
	//Agora pra uniforms
	GLint numberOfUniforms, largesUniformName;
	glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &numberOfUniforms);
	glGetProgramiv(programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &largesUniformName);
	//para cada atributo, pegar as propriedades e guardar.
	for (GLuint i = 0; i < numberOfUniforms; i++)
	{
		char* buffer = new char[largesUniformName];
		GLsizei length;
		GLint size;
		GLenum type;
		glGetActiveUniform(programId, i, largesUniformName, &length,
			&size, &type, buffer);
		GLint unifLoc = glGetUniformLocation(programId, buffer);
		std::string _name(buffer);
		uniforms.insert(std::make_pair(_name, unifLoc));
	}
}
public:
	MyShader(std::string vsSrc, std::string fsSrc)
	{
		vsId = MakeShader(GL_VERTEX_SHADER, vsSrc);
		fsId = MakeShader(GL_FRAGMENT_SHADER, fsSrc);
		programId = MakeProgram(vsId, fsId);
		IntrospectProgram(programId, attributes, uniforms);
	}
	GLuint GetUniformByName(std::string name)
	{
		return uniforms.at(name);
	}
	GLuint GetAttributeByName(std::string name)
	{
		return attributes.at(name);
	}
	~MyShader()
	{
		glDeleteProgram(programId);
		glDeleteShader(vsId);
		glDeleteShader(fsId);
	}
	GLuint GetProgramId(){ return programId; }
};
#endif
#ifdef OPENGL_2
//Só funciona se estiver com o backend de opengl2
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
#else
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
	vector<array<float, 3>> vertexes;
	vector<array<float, 2>> UVs;
public:
	MyGeometry(std::vector< std::array<float, 3>> vertexes, std::vector<std::array<float, 2>> UVs)
	{
		this->vertexes = vertexes;
		this->UVs = UVs;
	}
	void Render(vtkSmartPointer<vtkMatrix4x4> mvp, GLuint textureId)
	{
		glEnable(GL_TEXTURE_2D);
		double _tempMat[16];
		vtkMatrix4x4::DeepCopy(_tempMat, mvp);
		float _mvpMat[16];
		for (int i = 0; i < 16; i++)
			_mvpMat[i] = _tempMat[i];
		
		glBindTexture(GL_TEXTURE_2D, textureId);
		
		glBegin(GL_TRIANGLES);
		for (auto i = 0; i < vertexes.size(); i++)
		{
			glTexCoord2fv(UVs[i].data());
			glVertex3fv(vertexes[i].data());
		}
		glEnd();
	}
};
#endif


class MyITKMPRActor :public vtkProp
{
private:
#ifdef OPENGL_2
	unique_ptr<MyShader> myShader;
#endif
	unique_ptr<MyGeometry> myGeometry;
	GLuint texture;
	bool isModified;
	array<double, 3> vetorNormalAoPlano;
	MyITKMPRActor(const MyITKMPRActor&); // Not implemented.
	void operator = (const MyITKMPRActor&); // Not implemented.
	Internal3DImageType::Pointer image;
	vector<ObliqueFilterType::Pointer> cacheOfObliqueFilters;
	vtkSmartPointer<vtkImageImport> importFromITKToVTK;
	itk::Image<unsigned char, 2>::Pointer finalResult;
	MipFilterType::Pointer mipFilter;
	WLFilterType::Pointer wlMIP;
	SlabWriterType::Pointer slabWriter;
	WriterType::Pointer writerMIP;
	TileFilterType::Pointer tileFilter;
protected:
	MyITKMPRActor();
	~MyITKMPRActor();

public:
	void UpdateReslice();
	//A primeira diferença desse meu actor é que ele usa como fonte de dados uma imagem da ITK, não um mapper
	void SetITKImage(Input3DImageType::Pointer i);
	void SetVector(array<double, 3> vec);

	static MyITKMPRActor *New();
	void PrintSelf(ostream& os, vtkIndent indent);
	void ShallowCopy(MyITKMPRActor* t);
	
	int RenderOpaqueGeometry(vtkViewport *v);
	int RenderTranslucentPolygonalGeometry(vtkViewport *v);
	int RenderVolumetricGeometry(vtkViewport *v);
	int RenderOverlay(vtkViewport *v);
	bool RenderFilteredOpaqueGeometry(vtkViewport *v, vtkInformation *requiredKeys);
	bool RenderFilteredTranslucentPolygonalGeometry(vtkViewport *v,vtkInformation *requiredKeys);
	bool RenderFilteredVolumetricGeometry(vtkViewport *v, vtkInformation *requiredKeys);
	bool RenderFilteredOverlay(vtkViewport *v, vtkInformation *requiredKeys);
};