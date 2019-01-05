#pragma once
#include<glm\glm.hpp>
class Transform {

public:
	Transform(const glm::mat3& matrix = glm::mat3(1.0f));
	static Transform Origin();
	Transform Combine(Transform&other);

	//any local point wrt this level moving within it. the others want to see it
	// have to multiply that local point with the m_local matrix.
	glm::mat3 m_matrix;//relative coordinates of this level with the prev level.
};