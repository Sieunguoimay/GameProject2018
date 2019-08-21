#pragma once
#include<vector>
#include<string>
#include<SDL2\SDL.h>
class MapInstruction {
public:
	int folder;
	int file;
	int tarFolder = -1;
	int tarFile = -1;
};
class CharacterMap {
public:
	std::string name;
	std::vector<MapInstruction>maps;
	void Log() {
		SDL_Log("\tcharacter_map %s\n", name.c_str());
		for (auto&map : maps)
			SDL_Log("\t\tmap %d %d %d %d\n", map.folder, map.file, map.tarFolder, map.tarFile);
	}
};
