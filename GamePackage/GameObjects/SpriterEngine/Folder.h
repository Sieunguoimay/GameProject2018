#pragma once

#include<vector>
#include<string>
#include"../../2D/Texture.h"
namespace SpriterEngine {

	struct File {
		std::string name;
		float pivotX = 0;
		float pivotY = 0;

		//(engine specific type) fileReference;
		// a reference to the image store in this file
		Texture*pTexture = NULL;
		int textureId;
		
		~File() {pTexture = NULL;}
	};

	struct Folder {
		std::string name;
		std::vector<SpriterEngine::File> files;
		
		~Folder() {printf("Deleted Folder\n");}
		void Log() {
			printf("folder %s\n", name.c_str());
			for (auto&file : files)
				printf("\tfile %s %f %f\n", file.name.c_str(), file.pivotX, file.pivotY);
		}
	};
}