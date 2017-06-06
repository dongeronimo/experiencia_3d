#include <iostream>
#include <spdlog.h>

using namespace std;
using namespace spdlog;

int main(int argc, char** argv)
{
	//Multithreaded console logger(with color support)
	auto console = spdlog::stdout_color_mt("console");
	console->info("começando");
	return EXIT_SUCCESS;
}