#include "MyVolumeActor.h"
#include <iostream>
#include <vtkObjectFactory.h>
using namespace std;
vtkStandardNewMacro(MyVolumeActor);
MyVolumeActor::MyVolumeActor()
{
}

MyVolumeActor::~MyVolumeActor()
{
}

void MyVolumeActor::PrintSelf(ostream& os, vtkIndent indent)
{
}

void MyVolumeActor::ShallowCopy(MyVolumeActor* t)
{
	
}
//Esse � um dos m�todos de renderiza��o invocados
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
//Esse � um dos m�todos de renderiza��o invocados
int MyVolumeActor::RenderVolumetricGeometry(vtkViewport* v)
{
	cout << __FUNCTION__ << endl;
	return 0;
}
//Esse � um dos m�todos de renderiza��o invocados
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
