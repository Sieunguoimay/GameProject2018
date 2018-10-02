#include"SpriteManager.h"
#include"../misc/Assistances.h"
#include"../Graphics/ResourceManager.h"
#include"AnimatedSprite.h"
SpriteManager::~SpriteManager() {

}
SpriteManager*SpriteManager::s_instance = NULL;
SpriteManager&SpriteManager::Instance() {
	if (s_instance == NULL) {
		s_instance = new SpriteManager();
	}
	return *s_instance;
}


bool SpriteManager::Init(const char*filename) {
	char*data = NULL;
	loadFile(data, filename);
	if (data == NULL) {
		SDL_Log("Failed to load scene %s", filename);
		return false;
	}
	std::string s_data(data);
	free(data);
	std::stringstream ss(s_data);
	std::string type;
	while (ss >> type) {
		std::string name, prefix;
		int n,id;
		if (type == "#Sprites") {
			int textureId, colNum, rowNum, shaderId, meshId,animationId;
			ss >> n;
			for (int i = 0; i < n; i++) {
				ss >> prefix >> id
					>> prefix >> textureId
					>> prefix >> colNum
					>> prefix >> rowNum
					>> prefix >> shaderId
					>> prefix >> meshId;
				Sprite*newSprite = new Sprite(s_resourceManager.GetMesh(meshId),s_resourceManager.GetTexture(textureId), s_resourceManager.GetShader(shaderId));
				newSprite->SetColNum(colNum);
				newSprite->SetRowNum(rowNum);
				/*
				Model3D*newModel = new Model3D();
				newModel->SetMesh(s_resourceManager.GetMesh(meshId));
				newModel->SetShader(s_resourceManager.GetShader(shaderId));
				newModel->AddTextrue(s_resourceManager.GetTexture(textureId));

				newSprite->SetModel(newModel);
				newSprite->SetColNum(colNum);
				newSprite->SetRowNum(rowNum);
				newSprite->Init();
				*/
				name = s_resourceManager.GetTexture(textureId)->GetName();
				m_sprites[name]=newSprite;
				m_animationSets[m_sprites[name]] = (new std::map<std::string, Animation*>());
				SDL_Log("Loaded new Sprite %d %s %d %d %d %d %d	%f %f",id, name.c_str(),textureId, colNum, rowNum, shaderId, meshId,newSprite->GetWidth(), newSprite->GetHeight());
			}
		}
		if (type == "#Animations") {
			std::string name, spriteName;
			int num, row, col;
			float spf;
			ss >> n;
			for (int i = 0; i < n; i++) {
				Animation*newAnimation = new Animation();
				ss >> prefix >> id
					>> prefix >> name
					>> prefix >> spriteName
					>> prefix >> spf
					>> prefix >> num;
				ss >> prefix;
				for (int i = 0; i < num; i++) {
					ss >> row;
					newAnimation->AddRowIndex(row);
				}
				ss >> prefix;
				for (int i = 0; i < num; i++) {
					ss >> col;
					newAnimation->AddColIndex(col);
				}
				newAnimation->SetSpriteNum(num);
				newAnimation->SetSecondPerFrame(spf);
				(*m_animationSets[m_sprites[spriteName]])[name] = newAnimation;
				SDL_Log("Loaded new Animation %s %f %d ", name.c_str(), spf, num);
			}
		}

	}
}


void SpriteManager::CleanUp() {
	for (std::map<std::string, Sprite*>::iterator it = m_sprites.begin(); it != m_sprites.end();it++)
		delete it->second;
	for (std::map<Sprite*, std::map<std::string, Animation*>*>::iterator it = m_animationSets.begin();
		it != m_animationSets.end(); it++) {
		for (std::map<std::string, Animation*>::iterator it2 = (it->second)->begin();
			it2 != (it->second)->end(); it2++)
			delete it2->second;
		delete it->second;
	}
	delete s_instance;
}
std::map<std::string, Animation*>*SpriteManager::GetAnimationSet(std::string name) {
	auto a = m_animationSets.find(m_sprites[name]);
	if (a == m_animationSets.end())
		return NULL;
	return a->second;
}
std::map<std::string, Animation*>*SpriteManager::GetAnimationSet(Sprite*sprite) {
	auto a = m_animationSets.find(sprite);
	if (a == m_animationSets.end())
		return NULL;
	return a->second;
}

void SpriteManager::DrawLine(float x1, float y1, float x2, float y2)
{
	glm::vec2 diff(x2-x1,y2-y1);
	float angle = b2Atan2(diff.x, -diff.y);
	float length = glm::length(diff);
	m_sprites["b4"]->Draw(0, 0, (x1 + x2)/2 , (y1+y2)/2, 1, length, glm::degrees(angle));
}

