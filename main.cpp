#include <iostream>
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

vtkSmartPointer<vtkRenderWindow> CreateScreen();


int main(int argc, char** argv)
{
	std::shared_ptr<spdlog::logger> mylogger = spdlog::stdout_color_mt("console");
	shared_ptr<imageLoader::LoadedImage> imagem = nullptr;
	shared_ptr<imageLoader::LoadedImage> LoadData(string dir);
	vtkSmartPointer<MyVolumeActor> myActor;

	string imageDir(argv[1]);
	imagem = LoadData(imageDir);
	vtkSmartPointer<vtkRenderWindow> renderWindow = CreateScreen();
	myActor = vtkSmartPointer<MyVolumeActor>::New();
	myActor->SetImagem(imagem->GetImage());
	renderWindow->GetRenderers()->GetFirstRenderer()->AddActor(myActor);
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
	renderWindow->SetSize(200, 200);
	renderWindow->AddRenderer(renderer);
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);
	return renderWindow;
}