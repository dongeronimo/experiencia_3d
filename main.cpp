#include <iostream>
#include <vtkCamera.h>
#include <string>
#include <spdlog.h>
#include <ImageLoader.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRendererCollection.h>
#include "MyVolumeActor.h"
using namespace std;
using namespace spdlog;
shared_ptr<imageLoader::LoadedImage> LoadData(string dir);
vtkSmartPointer<vtkRenderWindow> CreateScreen();


int main(int argc, char** argv)
{
	string imageDir(argv[1]);
	std::shared_ptr<spdlog::logger> mylogger = spdlog::stdout_color_mt("console");
	shared_ptr<imageLoader::LoadedImage> imagem = nullptr;
	imagem = LoadData(imageDir);
	vtkSmartPointer<vtkRenderWindow> renderWindow = CreateScreen();
	
	vtkSmartPointer<MyVolumeActor> myActor;
	myActor = vtkSmartPointer<MyVolumeActor>::New();
	myActor->SetImagem(imagem->GetImage());
	
	renderWindow->GetRenderers()->GetFirstRenderer()->AddActor(myActor);
	renderWindow->GetRenderers()->GetFirstRenderer()->ResetCamera();
	renderWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->ParallelProjectionOn();
	renderWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->SetParallelScale(2);
	renderWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->Zoom(0.5);
	renderWindow->GetInteractor()->Start();

	return EXIT_SUCCESS;
}

shared_ptr<imageLoader::LoadedImage> LoadData(string dir)
{
	imageLoader::NameListGenerator nlg;
	vector<string> seriesIdentifiers;
	vector<StringList> listOfFiles = nlg.CreateList(dir, seriesIdentifiers);
	unsigned int qual;
	if(seriesIdentifiers.size() == 1)
	{
		qual = 0;
	}
	else
	{
		cout << "Imagens no diretório:" << endl;
		for(auto i=0; i<seriesIdentifiers.size(); i++)
		{
			cout<<i<<") " << seriesIdentifiers[i] << "; com " << listOfFiles[i].size() << " imagens" << endl;
		}
		cin >> qual;
	}
	imageLoader::ImageLoader loader;
	return loader.Load(listOfFiles[qual], "exame", "serie");
}

vtkSmartPointer<vtkRenderWindow> CreateScreen()
{
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->SetBackground(0.1, 0.2, 0.4);
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->SetSize(640, 480);
	renderWindow->AddRenderer(renderer);
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);
	return renderWindow;
}