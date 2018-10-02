#pragma once
#include<string>
#include<map>
#include"SDL2\SDL.h"
#include"SDL2\SDL_opengles2.h"
#include"glm\glm.hpp"
#ifndef SHADERS_H
#define SHADERS_H
class Shaders 
{
	GLuint LoadShader(GLenum type, const char * filename);
	GLuint LoadProgram(GLuint vertexShader, GLuint fragmentShader);
	std::map<std::string, GLint>m_variableLocations;
public:
	GLuint program, vertexShader, fragmentShader;
	char fileVS[260];
	char fileFS[260];
	bool Init(std::string fileVertexShader, std::string fileFragmentShader);
	void UseProgram();
	Shaders() { SDL_Log("Created Shader Object"); }
	~Shaders();


	GLint a_posL;
	GLint a_norm;
	GLint a_texcoord;

	GLint u_model;
	GLint u_view;
	GLint u_proj;
	GLint u_camZ;
	GLint u_texSize;
	GLint u_spriteOffset;
	GLint u_alpha;
	GLint u_texture0;
	GLint u_shininess;
	GLint u_lightNum;
	GLint u_lights[4][6];

};
#endif//SHADERS_H