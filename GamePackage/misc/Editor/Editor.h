#pragma once
#include"../../GameObjects/Entity.h"

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

#include"../../GameObjects/World/World.h"
struct EditorObject {
	glm::vec2*m_pPos;
	glm::ivec2*m_pSize;
	Texture*m_pTexture;

	bool m_hover;
	bool m_hold;
	bool m_clickEvent;
	bool m_clickOnBox;
	glm::vec2 oldpos;
	glm::vec2 oldmouse;

	EditorObject(glm::vec2*pPos, glm::ivec2*pSize,Texture*pTexture)
		:m_pPos(pPos),m_pSize(pSize),m_pTexture(pTexture)
	{}
	void ChangePos(float x, float y) {
		if (m_pPos != NULL) {
			m_pPos->x = x;
			m_pPos->y = y;
		}
	}
	bool Update(float deltaTime, glm::vec2 origin = glm::vec2(0,0));
	void Draw(glm::vec2 origin = glm::vec2(0.0f));
};
class Editor :public Entity{
	World*m_pWorld;
	//m_vertexObjects;//keeps the position of vertices within a object, origin is 0,0
	std::vector<std::pair<EditorObject*, std::vector<EditorObject*>*>>m_editorObjects;//keeps the position in the real world
public:
	Editor(World*pWorld);
	~Editor();
	//this function is for purpose of loading the data from Physics body Editor
	//and that data once loaded will be stored into the game-world-data-file
	//so once that game-world-data-file is created. this function is no use.
	static Platform*LoadObjectFromJson(const char*filename);
	static void LoadObjectFromPEXml(const char*filename, World*pWorld);
	static void WriteWorldStructureToFile(const char*filename, World*pWorld);
	static void LoadGameWorldFromXml(const char*filename, World*pWorld);

	void Init(const char*filename);



	void Update(float deltaTime)override;
	void Draw() override;
};
