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

using namespace std;

class MyVolumeActor :public vtkProp
{
private:
	MyVolumeActor(const MyVolumeActor&); // Not implemented.
	void operator = (const MyVolumeActor&); // Not implemented.
protected:
	MyVolumeActor();
	~MyVolumeActor();
public:
	static MyVolumeActor *New();
	void PrintSelf(ostream& os, vtkIndent indent);
	void ShallowCopy(MyVolumeActor* t);

	int RenderOpaqueGeometry(vtkViewport *v);
	int RenderTranslucentPolygonalGeometry(vtkViewport *v);
	int RenderVolumetricGeometry(vtkViewport *v);
	int RenderOverlay(vtkViewport *v);
	bool RenderFilteredOpaqueGeometry(vtkViewport *v, vtkInformation *requiredKeys);
	bool RenderFilteredTranslucentPolygonalGeometry(vtkViewport *v, vtkInformation *requiredKeys);
	bool RenderFilteredVolumetricGeometry(vtkViewport *v, vtkInformation *requiredKeys);
	bool RenderFilteredOverlay(vtkViewport *v, vtkInformation *requiredKeys);
}