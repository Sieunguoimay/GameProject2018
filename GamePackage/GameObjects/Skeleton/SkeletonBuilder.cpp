#include "SkeletonBuilder.h"
#include"../../misc/Locator.h"
#include"../../misc/Assistances.h"
void SkeletonBuilder::BuildFromFile(std::vector<Bone*>&skeleton,const std::string & file)
{
	std::vector<Bone*>levels;

	char*data = NULL;
	loadFile(data, file);
	if (data == NULL)
		assert("Problem with skeleton loading");
	std::string s_data(data);
	free(data);
	std::stringstream ss(s_data);
	std::string type;
	while (ss >> type) {
		std::string texture;
		int level, length, numBones;
		glm::vec2 dimension;
		glm::vec2 local;
		float angle;
		if (type == "#Bones") {
			ss >> numBones;
			for (int i = 0; i < numBones; i++) {
				ss >> level >> length >> dimension.x >> dimension.y >> local.x >> local.y >> angle >> texture;
				Texture *t = NULL;
				if (texture != "null") t = Locator::GetAssets()->GetTexture(texture);;

				Bone*newBone = new Bone(length, glm::vec4(-(dimension.x- length)/2, -dimension.y / 2, dimension.x, dimension.y), t);
				newBone->SetRotation(angle);
				newBone->SetOrigin(local);
				skeleton.push_back(newBone);

				if (level == levels.size()) {
					levels.push_back(newBone);
					if (level == 0) continue;
				}
				else {
					levels[level] = newBone;
					SDL_Log("%d", level);
				}
				levels[level - 1]->AddChild(newBone, levels[level - 1]->GetEndTerminal());


			}
		}
	}
}
