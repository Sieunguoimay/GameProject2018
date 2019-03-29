#pragma once
#include"../Assistances.h"
#include"PutAABBObject.h"
#include"VertexObject.h"
#include"Picker.h"
//editor works with the game-world-data file
//and that file is also loaded for the game
//directly. 

//the only difference between reading that file
//from editor and from game is at how they process
//the data.

//that mean the reading method used for that file
//should be the same. and should be laid some where
//either in this Editor class or outside at the game base.
//or perhaps even in another different utility class 
class Editor:public Entity{
	//m_vertexObjects;//keeps the position of vertices within a object, origin is 0,0
	//keeps the position in the real world
	std::vector<VertexObject*> m_verexObjects;
	void addVertexObject(int i, int j, Platform*platform);
	VertexObject*getVertexObject(int i);

	Picker m_picker;
	PutAABBObject m_putter;

	//pointers
	World*m_pWorld = NULL;
	ObjectPool*m_pObjectPool;
public:
	Editor(World*pWorld,ObjectPool*pObjectPool);
	~Editor();

	void Init()override;
	void Update(float deltaTime) override;
	void ExecuteCommand(char command);
	inline World*GetWorld() { return m_pWorld; }
	inline Picker&GetPicker() { return m_picker; }




	//this function is for purpose of loading the data from Physics body Editor
	//and that data once loaded will be stored into the game-world-data-file
	//so once that game-world-data-file is created. this function is no use.
	static Platform*LoadObjectFromJson(const char*filename);
	static void LoadObjectFromPEXml(const char*filename, World*pWorld);
	static void WriteWorldStructureToFile(const char*filename, World*pWorld, ObjectPool*pObjectPool, Editor*editor);
	static void LoadGameWorldFromXml(const char*filename, World*pWorld, ObjectPool*pObjectPool, Editor*editor);

	static void SortEntitiesBySize(List<AABBEntity*>&pEntities);
	static bool Compare(AABBEntity*a, AABBEntity*b);

};

