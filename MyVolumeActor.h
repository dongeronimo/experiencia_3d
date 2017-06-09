#pragma once

#include <vtk_glew.h>
#include <vtkOpenGL.h>
#include <vtkProp.h>
#include <vtkSmartPointer.h>
#include <array>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <itkImage.h>
#include "geometry.h"
#include "shader.h"
#include "MyTexture.h"
using namespace std;

typedef itk::Image<short, 3> ImageType;
class MyVolumeActor :public vtkProp
{
private:
	MyVolumeActor(const MyVolumeActor&); // Not implemented.
	void operator = (const MyVolumeActor&); // Not implemented.
	ImageType::Pointer imagem;
	shared_ptr<MyGeometry> myGeometry;
	shared_ptr<MyShader> shader;
	bool glInitialized;
	void InitOpenGL();
protected:
	MyVolumeActor();
	~MyVolumeActor();
public:
	void SetImagem(ImageType::Pointer img) { this->imagem = img; }

	static MyVolumeActor *New();
	void PrintSelf(ostream& os, vtkIndent indent) override;
	void ShallowCopy(MyVolumeActor* t);

	int RenderOpaqueGeometry(vtkViewport *v) override;
	int RenderTranslucentPolygonalGeometry(vtkViewport *v) override;
	int RenderVolumetricGeometry(vtkViewport *v) override;
	int RenderOverlay(vtkViewport *v) override;
	bool RenderFilteredOpaqueGeometry(vtkViewport *v, vtkInformation *requiredKeys) override;
	bool RenderFilteredTranslucentPolygonalGeometry(vtkViewport *v, vtkInformation *requiredKeys) override;
	bool RenderFilteredVolumetricGeometry(vtkViewport *v, vtkInformation *requiredKeys) override;
	bool RenderFilteredOverlay(vtkViewport *v, vtkInformation *requiredKeys) override;
};