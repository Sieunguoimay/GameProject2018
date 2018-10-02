#pragma once
#include<vector>
#include<string>
#include<sstream>
#include"Box2D\Box2D.h"
#include"SDL2\SDL_rwops.h"
#include"SDL2\SDL.h"
#define GAME_WIDTH 1920
#define GAME_HEIGHT 1080
static std::vector<std::string> split(const std::string& s, char seperator)
{
	std::vector<std::string> output;

	std::string::size_type prev_pos = 0, pos = 0;

	while ((pos = s.find(seperator, pos)) != std::string::npos)
	{
		std::string substring(s.substr(prev_pos, pos - prev_pos));

		output.push_back(substring);

		prev_pos = ++pos;
	}

	output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word

	return output;
}
static float _stof(std::string str) {
	return strtof(str.c_str(), NULL);
}
static int _stoi(std::string str) {
	return atoi(str.c_str());
}
static std::string _to_string(int i){
	std::stringstream ss;
	ss<<i;
	return ss.str();
}
static float _sign(float a) {
	if (a >= 0) return 1.0f;
	return -1.0f;
}
static void loadFile(char*&buffer, const std::string& filename) {
	SDL_RWops* rwops = SDL_RWFromFile(filename.c_str(), "rb");
	if (rwops == NULL)
	{
		SDL_Log("Could not open file \"%s\".\n", filename.c_str());
		return ;
	}
	SDL_RWseek(rwops, 0, SEEK_END);
	size_t data_max_size = (size_t)SDL_RWtell(rwops);
	SDL_RWseek(rwops, 0, SEEK_SET);

	buffer = (char*)malloc(data_max_size + 1);
	long len = 0;
	while ((len = SDL_RWread(rwops, buffer, sizeof(char), data_max_size)) > 0)
	{
		// Do stuff with your 'len' bytes of data
	}
	buffer[data_max_size] = 0;
	SDL_RWclose(rwops);

}

static void loadFile(std::vector<unsigned char>& buffer, const std::string& filename) //designed for loading files from hard disk in an std::vector
{
/*	std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

	//get filesize
	std::streamsize size = 0;
	if (file.seekg(0, std::ios::end).good()) size = file.tellg();
	if (file.seekg(0, std::ios::beg).good()) size -= file.tellg();

	//read contents of the file into the vector
	if (size > 0)
	{
		buffer.resize((size_t)size);
		file.read((char*)(&buffer[0]), size);
	}
	else buffer.clear();
*/

	SDL_RWops* rwops = SDL_RWFromFile(filename.c_str(), "rb");
	if (rwops == NULL)
	{
		SDL_Log("Could not open file \"%s\".\n", filename.c_str());
		return;
	}
	SDL_RWseek(rwops, 0, SEEK_END);
	size_t data_size = (size_t)SDL_RWtell(rwops);
	SDL_RWseek(rwops, 0, SEEK_SET);

	buffer.resize(data_size);
	long len = 0;
	while ((len = SDL_RWread(rwops, (char*)(&buffer[0]), sizeof(char), data_size)) > 0)
	{
		// Do stuff with your 'len' bytes of data
		SDL_Log("size is %ld\n", len);
	}
	SDL_RWclose(rwops);
}

static float RandomClamped(float begin = -1.0f, float end = 1.0f) {
	return (end-begin)*((float)(rand() % (RAND_MAX))/(float)RAND_MAX)+begin; //0.0-1.0
}

static b2Vec2 PointToWorldSpace(const b2Vec2 &point,
	const b2Vec2 &AgentHeading,
	const b2Vec2 &AgentSide,
	const b2Vec2 &AgentPosition)
{
	b2Vec2 head = point.x*AgentHeading;
	b2Vec2 side = point.y*AgentSide;
	b2Vec2 transPoint = AgentPosition+head+side;
	return transPoint;
}

static b2Vec2 PointToLocalSpace(const b2Vec2 &point,
	const b2Vec2 &AgentHeading,
	const b2Vec2 &AgentSide,
	const b2Vec2 &AgentPosition)
{
	b2Vec2 _point = point - AgentPosition;
	float x = b2Dot(AgentHeading, _point) / AgentHeading.Length();
	float y = b2Dot(AgentSide, _point)/AgentSide.Length();

	return b2Vec2(x,y);
}
static b2Vec2 VectorToWorldSpace(const b2Vec2 &vector,
	const b2Vec2 &AgentHeading,
	const b2Vec2 &AgentSide)
{
	b2Vec2 head = vector.x*AgentHeading;
	b2Vec2 side = vector.y*AgentSide;
	return b2Vec2(head.x + side.x, head.y + side.y);
}
static void Truncate(b2Vec2&vector, float max_length) {
	if (vector.LengthSquared() > max_length*max_length) {
		vector.Normalize();
		vector *= max_length;
	}
}
#include"glm\glm.hpp"
#include"SDL2\SDL_opengles2.h"
static void processVertex(std::vector<std::string>&vertexDataString, std::vector<GLushort>& indices, std::vector<glm::vec2>& textures, std::vector<glm::vec3>& normals, float * &uvArray, float * &normalArray)
{
	unsigned int vertexData[3];
	for (size_t i = 0; i < vertexDataString.size(); i++) {
		if (vertexDataString[i] != "")
			vertexData[i] = _stoi(vertexDataString[i]);
		else vertexData[i] = 1;
	}
	int currentVertexPointer = vertexData[0] - 1;
	indices.push_back(currentVertexPointer);

	glm::vec2 currentUv = (textures.size() == 0 ? glm::vec2(-1.0, -1.0) : textures[vertexData[1] - 1]);
	(uvArray[currentVertexPointer * 2]) = currentUv.x;
	(uvArray[currentVertexPointer * 2 + 1]) = currentUv.y;

	glm::vec3 currentNor = (normals.size() == 0 ? glm::vec3(-1.0, -1.0, -1.0) : normals[vertexData[2] - 1]);
	(normalArray[currentVertexPointer * 3]) = currentNor.x;
	(normalArray[currentVertexPointer * 3 + 1]) = currentNor.y;
	(normalArray[currentVertexPointer * 3 + 2]) = currentNor.z;
}
static bool LoadObj(std::string path, std::vector<GLfloat>&verticesArray, std::vector<GLushort>&indicesArray)
{
	int current = SDL_GetTicks();
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< GLushort > temp_indices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;
	float*uvArray = NULL, *normalArray = NULL;

	SDL_RWops*rwops = SDL_RWFromFile(path.c_str(), "r");
	if (rwops == NULL)
		return false;
	std::string line;
	char temp;
	while (SDL_RWread(rwops, &temp, sizeof(temp), 1)>0) {
		line += temp;
		if (temp == '\n') {
			//printf("I got the line : %s\n", line.c_str());
			std::vector<std::string>splittedLine = split(line, ' ');
			if (splittedLine[0] == "v") {
				temp_vertices.push_back(glm::vec3(
					_stof(splittedLine[1]),
					_stof(splittedLine[2]),
					_stof(splittedLine[3])
					));
				//printf("OK %f %f %f\n", temp_vertices[temp_vertices.size() - 1].x,
				//	temp_vertices[temp_vertices.size() - 1].y,
				//	temp_vertices[temp_vertices.size() - 1].z);
			}
			else if (splittedLine[0] == "vt") {
				temp_uvs.push_back(glm::vec2(
					_stof(splittedLine[1]),
					-_stof(splittedLine[2])
					));
				//printf("OK %f %f\n", temp_uvs[temp_uvs.size() - 1].x,
				//	temp_uvs[temp_uvs.size() - 1].y);
			}
			else if (splittedLine[0] == "vn") {
				temp_normals.push_back(glm::vec3(
					_stof(splittedLine[1]),
					_stof(splittedLine[2]),
					_stof(splittedLine[3])
					));
				//printf("OK %f %f %f\n", temp_normals[temp_normals.size() - 1].x,
				//	temp_normals[temp_normals.size() - 1].y,
				//	temp_normals[temp_normals.size() - 1].z);
			}
			else if (splittedLine[0] == "f") {
				if (uvArray == NULL) {
					uvArray = new GLfloat[temp_vertices.size() * 2];
					normalArray = new GLfloat[temp_vertices.size() * 3];
					if (temp_uvs.size() == 0)
						SDL_Log("No texture babe :(");
					if (temp_normals.size() == 0)
						SDL_Log("No normal babe :(");
				}

				std::vector<std::string> vertexDataString0 = split(splittedLine[1], '/');
				processVertex(vertexDataString0, temp_indices, temp_uvs, temp_normals, uvArray, normalArray);
				std::vector<std::string> vertexDataString1 = split(splittedLine[2], '/');
				processVertex(vertexDataString1, temp_indices, temp_uvs, temp_normals, uvArray, normalArray);
				std::vector<std::string> vertexDataString2 = split(splittedLine[3], '/');
				processVertex(vertexDataString2, temp_indices, temp_uvs, temp_normals, uvArray, normalArray);



				if (splittedLine.size() == 5) {

					processVertex(vertexDataString0, temp_indices, temp_uvs, temp_normals, uvArray, normalArray);
					processVertex(vertexDataString2, temp_indices, temp_uvs, temp_normals, uvArray, normalArray);

					std::vector<std::string> vertexDataString3 = split(splittedLine[4], '/');
					processVertex(vertexDataString3, temp_indices, temp_uvs, temp_normals, uvArray, normalArray);

				}
			}
			line = "";
		}
	}
	SDL_RWclose(rwops);
	int i = 0;
	for (glm::vec3&v : temp_vertices) {
		verticesArray.push_back(v.x);
		verticesArray.push_back(v.y);
		verticesArray.push_back(v.z);

		verticesArray.push_back(normalArray[i * 3]);
		verticesArray.push_back(normalArray[i * 3 + 1]);
		verticesArray.push_back(normalArray[i * 3 + 2]);

		verticesArray.push_back(uvArray[i * 2]);
		verticesArray.push_back(uvArray[i * 2 + 1]);
		i++;
	}

	for (size_t i = 0; i < temp_indices.size(); i++) {
		indicesArray.push_back(temp_indices[i]);
	}

	delete[] uvArray;
	delete[] normalArray;
	SDL_Log("Loading Time %f", (float)(SDL_GetTicks() - current) / 1000.0f);
	return true;
}

static bool LoadNfg(std::string path, std::vector<GLfloat>&vertices, std::vector<GLushort>&indices) {
	int current = SDL_GetTicks();
	SDL_RWops*rwops = SDL_RWFromFile(path.c_str(), "r");
	if (rwops == NULL)
	{
		SDL_Log("Could not open file \"%s\".\n", path.c_str());
		return false;
	}

	SDL_RWseek(rwops, 0, SEEK_END);
	size_t data_max_size = (size_t)SDL_RWtell(rwops);
	SDL_RWseek(rwops, 0, SEEK_SET);

	long len = 0;
	char*data = (char*)malloc(data_max_size + 1);
	while ((len = SDL_RWread(rwops, data, sizeof(char), data_max_size))>0) {
		//SDL_Log("-----%s-----\n", data);
	}
	data[data_max_size] = 0;
	SDL_RWclose(rwops);


	if (data != NULL) {
		int vertexNum;
		sscanf(data, "NrVertices: %d\r\n", &vertexNum);
		//printf("\n\n\n\n%d",vertexNum);
		std::string base = "NrVertices: %*d\n";

		float x, y, z, v, t;
		float nx, ny, nz, bnx, bny, bnz, tx, ty, tz;

		for (int i = 0; i < vertexNum; i++) {

			//base += "%*d. pos:[%f, %f, %f]; norm:[%f, %f, %f]; binorm:[%f, %f, %f]; tgt:[%f, %f, %f]; uv:[%f,%f];\n";
			int n = sscanf(data, (base + "%*d. pos:[%f, %f, %f]; norm:[%f, %f, %f]; binorm:[%f, %f, %f]; tgt:[%f, %f, %f]; uv:[%f,%f];\n").c_str(),
				&x, &y, &z, &nx, &ny, &nz, &bnx, &bny, &bnz, &tx, &ty, &tz, &v, &t);

			base += "%*d. pos:[%*f, %*f, %*f]; norm:[%*f, %*f, %*f]; binorm:[%*f, %*f, %*f]; tgt:[%*f, %*f, %*f]; uv:[%*f,%*f];\n";

			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
			vertices.push_back(nx);
			vertices.push_back(ny);
			vertices.push_back(nz);
			vertices.push_back(v);
			vertices.push_back(-t);
			//printf("\n\n%f %f %f", x, y,z);

			//return 0;
		}
		//return 0;
		int m_numIndices;
		sscanf(data, (base + "NrIndices: %d\n").c_str(), &m_numIndices);
		base += "NrIndices: %*d\n";
		printf("\n\nindices %d", m_numIndices);

		int a, b, c;
		for (int i = 0; i < m_numIndices / 3; i++) {
			sscanf(data, (base + "%*d. %d, %d, %d\n").c_str(), &a, &b, &c);
			base += "%*d. %*d, %*d, %*d\n";

			indices.push_back(a);
			indices.push_back(b);
			indices.push_back(c);
		}

		//fclose(file);
		printf("Loaded model %s\n", path.c_str());
		SDL_Log("Loading Time %f", (float)(SDL_GetTicks() - current) / 1000.0f);
		return true;
	}
	else
		printf("Error to Load model %s\n", path.c_str());
	return true;
}
