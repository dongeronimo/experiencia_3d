#include <iostream>
#include <string>
#include <spdlog.h>
#include <ImageLoader.h>

using namespace std;
using namespace spdlog;
std::shared_ptr<spdlog::logger> mylogger = spdlog::stdout_color_mt("console");
shared_ptr<imageLoader::LoadedImage> imagem = nullptr;
shared_ptr<imageLoader::LoadedImage> LoadData(string dir);

int main(int argc, char** argv)
{
	string imageDir(argv[1]);
	imagem = LoadData(imageDir);
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