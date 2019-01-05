#include"Assistances.h"
void logError(const std::string&error) {
	SDL_Log("Error: %s", error.c_str());
	exit(69);
}
float getAngle(float x, float y) {
	float length = std::sqrt(x*x + y*y);

	if (length < 0) return 0;

	float angle = acosf(x / length);
	if (y < 0) angle = 2.0f*3.141592654f - angle;
	return angle;
}

float _atof(const char*a) {
	if (a) {
		return atof(a);
	}
	return 0;
}
float _atoi(const char*a) {
	if (a) {
		return atoi(a);
	}
	return 0;
}
std::string _atos(const char*a) {
	if (a) {
		return std::string(a);
	}
	return "";
}

void loadFile(char*&buffer, const std::string& filename) {
	SDL_RWops* rwops = SDL_RWFromFile(filename.c_str(), "rb");
	if (rwops == NULL)
	{
		SDL_Log("Could not open file \"%s\".\n", filename.c_str());
		return;
	}
	SDL_RWseek(rwops, 0, SEEK_END);
	size_t data_max_size = (size_t)SDL_RWtell(rwops);
	SDL_RWseek(rwops, 0, SEEK_SET);

	buffer = (char*)malloc(data_max_size + 1);
	long len = 0;
	while ((len = SDL_RWread(rwops, buffer, sizeof(char), data_max_size)) > 0)
	{
		// Do stuff with your 'len' bytes of data
	}
	buffer[data_max_size] = 0;
	SDL_RWclose(rwops);
}
void loadFile(std::vector<unsigned char>& buffer, const std::string& filename) //designed for loading files from hard disk in an std::vector
{
	/*	std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

	//get filesize
	std::streamsize size = 0;
	if (file.seekg(0, std::ios::end).good()) size = file.tellg();
	if (file.seekg(0, std::ios::beg).good()) size -= file.tellg();

	//read contents of the file into the vector
	if (size > 0)
	{
	buffer.resize((size_t)size);
	file.read((char*)(&buffer[0]), size);
	}
	else buffer.clear();
	*/

	SDL_RWops* rwops = SDL_RWFromFile(filename.c_str(), "rb");
	if (rwops == NULL)
	{
		SDL_Log("Could not open file \"%s\".\n", filename.c_str());
		return;
	}
	SDL_RWseek(rwops, 0, SEEK_END);
	size_t data_size = (size_t)SDL_RWtell(rwops);
	SDL_RWseek(rwops, 0, SEEK_SET);

	buffer.resize(data_size);
	long len = 0;
	while ((len = SDL_RWread(rwops, (char*)(&buffer[0]), sizeof(char), data_size)) > 0)
	{
		// Do stuff with your 'len' bytes of data
		SDL_Log("size is %ld\n", len);
	}
	SDL_RWclose(rwops);
}
