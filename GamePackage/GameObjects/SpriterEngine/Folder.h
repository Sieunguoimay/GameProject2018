#pragma once
#include<vector>
#include<string>
#include"../../2D/Texture.h"
#include<SDL2\SDL.h>
namespace SpriterEngine {
	class File {
	public:
		std::string name;
		float pivotX = 0;
		float pivotY = 0;

		//(engine specific type) fileReference;
		// a reference to the image store in this file
		Texture*pTexture = NULL;
		int textureId;
	};
	class Folder {
	public:
		std::string name;
		std::vector<SpriterEngine::File> files;
		~Folder() {
			SDL_Log("Deleted Folder\n");
		}
		void Log() {
			SDL_Log("folder %s\n", name.c_str());
			for (auto&file : files)
				SDL_Log("\tfile %s %f %f\n", file.name.c_str(), file.pivotX, file.pivotY);
		}
	};
}