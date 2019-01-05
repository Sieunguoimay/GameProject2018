#pragma once
#include<vector>
#include<map>
#include<string>
#include"Sprite.h"
#include<glm\glm.hpp>
class SpriteSheetAnimation {
	std::vector<int>m_indices;
	int m_fps;
	int m_index;
public:
	SpriteSheetAnimation(const std::vector<int>&indices, int fps);
	~SpriteSheetAnimation();

	int GetNumIndices() { return m_indices.size(); }
	int GetFPS() { return m_fps; }
	void NextFrame() { m_index = (m_index + 1) % m_indices.size(); }
	int GetIndex() { return m_indices[m_index]; }
	std::vector<int>&GetIndices() { return m_indices; }
	void Reset() { m_index = 0; }
};

//NOTE: if object of a class is kept in a container then it need not to have init()
//while the objects are member of other class should.


class AnimationCollection
{
	std::map<std::string, SpriteSheetAnimation*> m_animations;
	SpriteSheetAnimation *m_pCurrentAnimation;
	Sprite*m_sprite;
	float m_time;
public:
	AnimationCollection();
	~AnimationCollection();
	void Init(Texture*texture,glm::ivec2 dimension);
	void AddAnimation(std::string name, std::vector<int>&indices, int fps);
	void AddAnimation(std::string name, SpriteSheetAnimation*newAnimation);
	AnimationCollection*Clone();

	void SwitchAnimation(std::string name);
	void Update(float deltaTime);
	glm::vec4 GetCurrentSpriteRect();
	GLuint GetTextureID();
	inline Sprite*GetSprite() { return m_sprite; }
};

