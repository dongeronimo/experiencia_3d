#include "MyVolumeActor.h"
#include <iostream>
#include <array>
#include <vtkRenderer.h>
#include <vtkCamera.h>
#include <vtkMatrix4x4.h>
#include <vtkObjectFactory.h>
using namespace std;
string vertexSrc = "#version 330 core\n"
"layout(location = 0) in vec2 vertexUV;\n"
"layout(location = 1) in vec3 vertexPosition_modelspace;\n"
"uniform mat4 mvp;\n"
"out vec2 UV;\n"
"void main()\n"
"{\n"
"  gl_Position = mvp * vec4(vertexPosition_modelspace, 1);\n"
"  UV = vertexUV;\n"
"}\n";
string fragSrc = "#version 330 core\n"
"in vec2 UV;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"  color = vec4(UV.x, UV.y, 0.0, 1);\n"
"}\n";
vtkStandardNewMacro(MyVolumeActor);
void MyVolumeActor::InitOpenGL()
{

	shader = make_shared<MyShader>(vertexSrc, fragSrc);

	const float min_vert = -0.2; //0
	const float max_vert = 0.2;
	
	std::array<float, 3> p1 = { min_vert, min_vert, 0 };  std::array<float, 2> uv1 = { 0, 0 };
	std::array<float, 3> p2 = { max_vert, min_vert, 0 };  std::array<float, 2> uv2 = { 1, 0 };
	std::array<float, 3> p3 = { min_vert, max_vert, 0 };  std::array<float, 2> uv3 = { 0, 1 };
	std::array<float, 3> p4 = { max_vert, max_vert, 0 };  std::array<float, 2> uv4 = { 1, 1 };
	
	std::vector<std::array<float, 3>> vb; std::vector<std::array<float, 2>> uv;
	std::array<float, 3> v1 = { p1[0], p1[1], p1[2] }; 
	std::array<float, 3> v2 = { p2[0], p2[1], p2[2] }; 
	std::array<float, 3> v3 = { p3[0], p3[1], p3[2] }; 
	std::array<float, 3> v4 = { p4[0], p4[1], p4[2] }; 
	vb.push_back(v1); uv.push_back(uv1);
	vb.push_back(v2); uv.push_back(uv2);
	vb.push_back(v3); uv.push_back(uv3);
	
	vb.push_back(v2); uv.push_back(uv2);
	vb.push_back(v4); uv.push_back(uv4);
	vb.push_back(v3); uv.push_back(uv3);
			
	myGeometry = make_shared<MyGeometry>(vb, uv);

	glInitialized = true;
}

MyVolumeActor::MyVolumeActor()
{
	imagem = nullptr;
	shader = nullptr;
	myGeometry = nullptr;
	glInitialized = false;
}

MyVolumeActor::~MyVolumeActor()
{
	
}

void MyVolumeActor::PrintSelf(ostream& os, vtkIndent indent)
{
	vtkProp::PrintSelf(os, indent);
	if (imagem)
		imagem->Print(os, 1);
	else
		os << "Sem imagem ITK";
}

void MyVolumeActor::ShallowCopy(MyVolumeActor* t)
{
	t->imagem = this->imagem;
	t->shader = this->shader;
	t->myGeometry = this->myGeometry;
	t->glInitialized = this->glInitialized;
	
}
//Esse é um dos métodos de renderização invocados
int MyVolumeActor::RenderOpaqueGeometry(vtkViewport* v)
{
	cout << __FUNCTION__ << endl;
	return 0;
}

int MyVolumeActor::RenderTranslucentPolygonalGeometry(vtkViewport* v)
{
	cout << __FUNCTION__ << endl;
	return 0;
}
//Esse é um dos métodos de renderização invocados
int MyVolumeActor::RenderVolumetricGeometry(vtkViewport* v)
{
	cout << __FUNCTION__ << endl;
	if (!glInitialized)
		InitOpenGL();
	vtkRenderer *ren = vtkRenderer::SafeDownCast(v);
	vtkCamera *cam = ren->GetActiveCamera();
	vtkSmartPointer<vtkMatrix4x4> mvp = cam->GetModelViewTransformMatrix();
	myGeometry->Render(mvp, shader.get());
	return 0;
}
//Esse é um dos métodos de renderização invocados
int MyVolumeActor::RenderOverlay(vtkViewport* v)
{
	cout << __FUNCTION__ << endl;
	return 0;
}

bool MyVolumeActor::RenderFilteredOpaqueGeometry(vtkViewport* v, vtkInformation* requiredKeys)
{
	cout << __FUNCTION__ << endl;
	return false;
}

bool MyVolumeActor::RenderFilteredTranslucentPolygonalGeometry(vtkViewport* v, vtkInformation* requiredKeys)
{	
	cout << __FUNCTION__ << endl;
	return false;
}

bool MyVolumeActor::RenderFilteredVolumetricGeometry(vtkViewport* v, vtkInformation* requiredKeys)
{
	cout << __FUNCTION__ << endl;
	return false;
}

bool MyVolumeActor::RenderFilteredOverlay(vtkViewport* v, vtkInformation* requiredKeys)
{
	cout << __FUNCTION__ << endl;
	return false;
}
