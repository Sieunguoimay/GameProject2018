#pragma once
#include<vector>
#include<SDL2\SDL.h>
struct Ref {
	int parent = -1;
	int timeline;
	int key;
	int z_index = 0;
};
struct MainlineKey {
	int time = 0;
	std::vector<Ref> boneRefs;
	std::vector<Ref> objectRefs;


	void Log() {
		SDL_Log("\t\tmain_line key: %d\n", time);
		for (auto&ref : boneRefs)
			SDL_Log("\t\t\tbone_ref %d %d %d\n", ref.parent, ref.timeline, ref.key);
		for (auto&ref : objectRefs)
			SDL_Log("\t\t\tobject_ref %d %d %d\n", ref.parent, ref.timeline, ref.key);
	}
};
