#pragma once
#include<vector>
#include<string>
#include<sstream>
#include"Box2D\Box2D.h"
#include"SDL2\SDL_rwops.h"
#include"SDL2\SDL.h"
#include"glm\glm.hpp"
#define GAME_WIDTH 1920
#define GAME_HEIGHT 1080
#define M2P 100.0f

extern void logError(const std::string&error);
extern float getAngle(float x, float y);
extern const glm::vec2&rotateVec2(const glm::vec2&, float);

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
static std::string _to_string(float f) {
	std::stringstream ss;
	ss << f;
	return ss.str();
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
extern void loadFile(char*&buffer, const std::string& filename);

extern void loadFile(std::vector<unsigned char>& buffer, const std::string& filename); //designed for loading files from hard disk in an std::vector
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

extern bool check_overlap(glm::vec4 a, glm::vec4 b);
extern float _atof(const char*a);
extern float _atoi(const char*a);
extern std::string _atos(const char*a);


