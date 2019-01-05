#include "Transform.h"

Transform::Transform(const glm::mat3&matrix /*= glm::mat3(1.0f)*/)
	:m_matrix(matrix)
{
}

Transform Transform::Origin()
{
	return Transform(glm::mat3(1.0f));
}

Transform Transform::Combine(Transform & other)
{
	return Transform(m_matrix * other.m_matrix);
}
