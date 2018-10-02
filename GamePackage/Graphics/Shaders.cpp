#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include "Shaders.h"
#include"SDL2\SDL.h"
#include"SDL2\SDL_rwops.h"
#include"../misc/Assistances.h"
bool Shaders::Init(std::string fileVertexShader, std::string fileFragmentShader)
{
	vertexShader = LoadShader(GL_VERTEX_SHADER,fileVertexShader.c_str());

	if (vertexShader == 0) {
		return false;
	}

	fragmentShader = LoadShader( GL_FRAGMENT_SHADER, fileFragmentShader.c_str());

	if ( fragmentShader == 0 )
	{
		glDeleteShader( vertexShader );
		return false;
	}

	program = LoadProgram(vertexShader, fragmentShader);
	if (program == 0)
		return false;

	fileFragmentShader.erase(fileFragmentShader.find('.'), fileFragmentShader.size()-1);
	fileFragmentShader.erase(0,fileFragmentShader.find_last_of('/')+1);
	SDL_Log("Init Shader %s successfully", fileFragmentShader.c_str());

	//finding location of uniforms / attributes
	a_posL = glGetAttribLocation(program, "a_posL");
	a_norm = glGetAttribLocation(program, "a_norm");
	a_texcoord = glGetAttribLocation(program, "a_texcoord");

	u_model = glGetUniformLocation(program, "u_model");
	u_view = glGetUniformLocation(program, "u_view");
	u_proj = glGetUniformLocation(program, "u_proj");
	u_camZ = glGetUniformLocation(program, "u_camZ");
	u_texSize = glGetUniformLocation(program, "u_texSize");
	u_spriteOffset = glGetUniformLocation(program, "u_spriteOffset");
	u_alpha = glGetUniformLocation(program, "u_alpha");
	u_texture0 = glGetUniformLocation(program, "u_texture0");
	u_shininess = glGetUniformLocation(program, "u_shininess");
	u_lightNum = glGetUniformLocation(program, "u_lightNum");
	for (int i = 0; i < 4; i++) {
		u_lights[i][0] = glGetUniformLocation(program, (std::string("u_lights[") + std::to_string(i) + std::string("].type")).c_str());
		u_lights[i][1] = glGetUniformLocation(program, (std::string("u_lights[") + std::to_string(i) + std::string("].diffuse")).c_str());
		u_lights[i][2] = glGetUniformLocation(program, (std::string("u_lights[") + std::to_string(i) + std::string("].pos")).c_str());
		u_lights[i][3] = glGetUniformLocation(program, (std::string("u_lights[") + std::to_string(i) + std::string("].constant")).c_str());
		u_lights[i][4] = glGetUniformLocation(program, (std::string("u_lights[") + std::to_string(i) + std::string("].linear")).c_str());
		u_lights[i][5] = glGetUniformLocation(program, (std::string("u_lights[") + std::to_string(i) + std::string("].quadratic")).c_str());
	}




	GLint maxUniformLen, numUniforms;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);
	glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformLen);
    GLchar *uniformName  = (GLchar*)malloc(sizeof(GLchar)*maxUniformLen);

	for (int i = 0; i < numUniforms; i++) {
		GLint size;
		GLenum type;
		GLint location;
		glGetActiveUniform(program, i, maxUniformLen, NULL, &size, &type, uniformName);
		location = glGetUniformLocation(program, uniformName);
		SDL_Log("%s %d", uniformName,location);
	}
    free(uniformName);


	GLint maxAttribLen, numAttribs;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numAttribs);
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttribLen);
	GLchar *attribName = (GLchar*)malloc(sizeof(GLchar)*maxAttribLen);
	for (int i = 0; i < numAttribs; i++) {
		GLint size;
		GLenum type;
		GLint location;
		glGetActiveAttrib(program, i, maxAttribLen, NULL, &size, &type, attribName);
		location = glGetAttribLocation(program, attribName);
		SDL_Log("%s %d", attribName, location);
	}
	//delete[] attribName;
	free(attribName);
	return true;
}

void Shaders::UseProgram()
{
	glUseProgram(this->program);
}

Shaders::~Shaders()
{

	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
GLuint Shaders::LoadShader(GLenum type,const char * filename)
{
	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader(type);

	if (shader == 0)
		return 0;
	char*v_data;
	loadFile(v_data, filename);

	//glShaderSource(shader, 1, (const char **)&data, NULL);
	glShaderSource(shader, 1, (const char **)(&v_data), NULL);
	free(v_data);

	//FILE * pf = fopen(filename, "rb");
	//if(pf==NULL)
	//	return 0;
	//fseek(pf, 0, SEEK_END);
	//long size = ftell(pf);
	//fseek(pf, 0, SEEK_SET);

	//char * shaderSrc = new char[size + 1];
	//fread(shaderSrc, sizeof(char), size, pf);
	//shaderSrc[size] = 0;
	//fclose(pf);

	//glShaderSource(shader, 1, (const char **)&shaderSrc, NULL);
	//delete[] shaderSrc;

	// Compile the shader
	glCompileShader(shader);

	// Check the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled)
	{
		GLint infoLen = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			char* infoLog = new char[infoLen];


			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
			SDL_Log("Error compiling shader:\n%s\n", infoLog);

			delete[] infoLog;
		}

		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

GLuint Shaders::LoadProgram(GLuint vertexShader, GLuint fragmentShader)
{
	GLuint programObject;
	GLint linked;

	// Create the program object
	programObject = glCreateProgram();

	if (programObject == 0)
		return 0;

	glAttachShader(programObject, vertexShader);
	glAttachShader(programObject, fragmentShader);

	// Link the program
	glLinkProgram(programObject);

	// Check the link status
	glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

	if (!linked)
	{
		GLint infoLen = 0;

		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			char* infoLog = new char[sizeof(char) * infoLen];


			glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
			SDL_Log("Error linking program:\n%s\n", infoLog);

			delete infoLog;
		}

		glDeleteProgram(programObject);
		return 0;
	}

	return programObject;
}