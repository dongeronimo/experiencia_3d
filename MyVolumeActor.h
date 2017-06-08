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