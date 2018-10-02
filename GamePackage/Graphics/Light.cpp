#include"Light.h"
#include"../misc/Assistances.h"
#include"SceneManager.h"
Light::Light() {

}
Light::~Light() {

}
bool Light::Init(int id,int type,glm::vec3 pos, glm::vec3 color, float constant, float linear, float quadratic) {
	this->m_type = type;
	this->m_id = id;
	this->m_pos = pos;
	this->m_color = color;

	this->m_constant = constant;
	this->m_linear = linear;
	this->m_quadratic = quadratic;

	return true;
}

void Light::SendDataToShaders(Shaders*shader, int id)
{
	glUniform1i(shader->u_lights[id][0], m_type);
	glUniform3f(shader->u_lights[id][1], m_color.r, m_color.g, m_color.b);
	glUniform3f(shader->u_lights[id][2], m_pos.x, m_pos.y, m_pos.z);

	glUniform1f(shader->u_lights[id][3], m_constant);
	glUniform1f(shader->u_lights[id][4], m_linear);
	glUniform1f(shader->u_lights[id][5], m_quadratic);

	if (id == 0) {
		glUniform1f(shader->u_shininess, 32.0f);
		glUniform1i(shader->u_lightNum, s_sceneManager.GetAllLight()->size());
	}
}
