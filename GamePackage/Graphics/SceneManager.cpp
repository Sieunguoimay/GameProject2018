#include"SceneManager.h"
#include<sstream>
#include"../misc/Assistances.h"
#include"ResourceManager.h"

SceneManager* SceneManager::s_instance = NULL;
SceneManager&SceneManager::Intance() {
	if (s_instance == NULL)
		s_instance = new SceneManager();
	return *s_instance;
}
SceneManager::~SceneManager() {
	
}

bool SceneManager::Init(std::string filename) {

	m_currentCameraId = 0;

	char*data = NULL;
	loadFile(data, filename);
	if (data==NULL) {
		SDL_Log("Failed to load scene %s", filename.c_str());
		return false;
	}
	std::string s_data(data);
	free(data);
	std::stringstream ss(s_data);
	std::string type;
	while (ss >> type) {
		std::string name, prefix;
		if (type == "#Cameras") {
			int n, id,current;
			glm::vec3 pos, front;
			ss >> n;
			for (int i = 0; i < n; i++) {
				ss >> prefix >> id
					>> prefix >> name
					>> prefix >> pos.x >> pos.y >> pos.z
					>> prefix >> front.x >> front.y >> front.z
					>> prefix >> current;
				Camera*newCamera = new Camera();
				newCamera->Init(pos,front);
				m_cameras.push_back(newCamera);
				if (current) m_currentCameraId = i;
				SDL_Log("Created camera %s", name.c_str());
			}
		}
		else if (type == "#Models") {
			int id, meshId, textureNum, textureId, shaderId;
			glm::vec3 pos, rot, scale;
			float angle;

			ss >> m_loadedModelNum;
			for (int i = 0; i < m_loadedModelNum; i++) {
				Model3D*newModel = new Model3D();

				ss >> prefix >> id
					>> prefix >> name
					>> prefix >> meshId
					>> prefix >> shaderId
					>> prefix >> textureNum;
				newModel->SetMesh(s_resourceManager.GetMesh(meshId));
				newModel->SetShader(s_resourceManager.GetShader(shaderId));
				for (int j = 0; j < textureNum; j++) {
					ss >> prefix >> textureId;
					newModel->AddTextrue(s_resourceManager.GetTexture(textureId));
				}
				ss >> prefix >> pos.x >> pos.y >> pos.z
					>> prefix >> rot.x >> rot.y >> rot.z
					>> prefix >> scale.x >> scale.y >> scale.z
					>> prefix >> angle;
				newModel->SetPos(pos);
				newModel->SetRot(rot);
				newModel->SetScale(scale);
				newModel->SetAngle(angle);

				m_models.push_back(newModel);
				SDL_Log("Created model %s", name.c_str());
			}
		}
		else if (type == "#Lights") {
			int n, id, _type, shaderNum,shaderId;
			glm::vec3 pos, color;
			std::string type;
			float constant = 1.0, linear = 0.09, quadratic = 0.032;
			ss >> n;
			for (int i = 0; i < n; i++) {
				Light*newLight = new Light();
				
				ss >> prefix >> id
					>> prefix >> name
					>> prefix >> type
					>> prefix >> shaderNum;
				for (int j = 0; j < shaderNum; j++) {
					ss >> prefix >> shaderId;
					newLight->AddShader(s_resourceManager.GetShader(shaderId));
				}
				ss >> prefix >> color.x >> color.y >> color.z
					>> prefix >> pos.x >> pos.y >> pos.z;
				if (type == "SUN") {
					_type = 0;
				}
				else if (type == "LIGHT_BULB") {
					ss	>> prefix >> constant
						>> prefix >> linear
						>> prefix >> quadratic;
					_type = 1;
				}

				if (newLight->Init(id,_type,pos, color,constant, linear, quadratic))
					m_lights.push_back(newLight);
				else delete newLight;
				SDL_Log("Created Light %s %f %f %f", name.c_str(),color.x,color.y,color.z);
			}
		}
	}
	SDL_Log("Scene Loaded. %d %d\n\n",m_cameras.size(), m_models.size());
	return true;
}

void SceneManager::CleanUp() {
	for (auto&a : m_models)
		delete a;
	for (auto&a : m_cameras)
		delete a;
	for (auto&a : m_lights)
		delete a;
	delete s_instance;
}
