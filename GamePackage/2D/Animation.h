#pragma once
#include"glm\glm.hpp"
#include<vector>
class Animation {
protected:
	std::vector<int>m_rowIndices;
	std::vector<int>m_colIndices;
	int m_spriteNum;
	int m_currentSprite;
	float m_secondPerFrame;
public:
	//initializer
	void SetSpriteNum(int num) { this->m_spriteNum = num; }
	void SetSecondPerFrame(float spf) { this->m_secondPerFrame = spf; }
	void AddColIndex(int col) { this->m_colIndices.push_back(col); }
	void AddRowIndex(int row){ this->m_rowIndices.push_back(row);}

	//public using
	int GetSpriteNum() { return this->m_spriteNum; }
	void SetSprite(int index) { this->m_currentSprite = index%m_spriteNum; }
	glm::vec2 GetIndex() { return glm::vec2(
		(float)this->m_colIndices[m_currentSprite], 
		(float)this->m_rowIndices[m_currentSprite]); }
	float GetSPF() { return m_secondPerFrame; }
};
