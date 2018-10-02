#pragma once
#include"Model3D.h"
#include"Camera.h"
#include"Light.h"
class SceneManager {
	SceneManager(){}
	SceneManager(const SceneManager&);
	SceneManager&operator=(const SceneManager&);
	static SceneManager*s_instance;
	std::vector<Model3D*> m_models;
	std::vector<Camera*>m_cameras;
	std::vector<Light*>m_lights;
	int m_currentCameraId;
	int m_loadedModelNum;
public:
	~SceneManager();
	static SceneManager&Intance();
	bool Init(std::string filename);
	void CleanUp();

	void AddModel(Model3D*newModel) { this->m_models.push_back(newModel); }//at the end
	Model3D*GetModel(int id) { if (id < m_models.size())return this->m_models[id]; return NULL; }
	std::vector<Model3D*>*GetAllModels() { return &(this->m_models); }
	Camera*GetCamera(int id) { if (id < m_cameras.size())return this->m_cameras[id]; return NULL; }
	std::vector<Camera*>*GetAllCamera() { return &(this->m_cameras); }
	Camera*GetCurrentCamera() { return this->m_cameras[m_currentCameraId]; }
	std::vector<Light*>*GetAllLight() { return &(this->m_lights); }
	Light*GetLight(int id) { if (id < m_lights.size())return this->m_lights[id]; return NULL; }
	int GetLoadedModelNum() { return this->m_loadedModelNum; }//only the loaded ones.
};

//to make life easier..
#define s_sceneManager SceneManager::Intance()