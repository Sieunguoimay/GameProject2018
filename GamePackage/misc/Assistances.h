#pragma once
#ifndef ASSISTANCES_H
#define ASSISTANCES_H
#include<vector>
#include<string>
#include<sstream>
#include"Box2D\Box2D.h"
#include"SDL2\SDL.h"
#include"glm\glm.hpp"
#define GAME_WIDTH 1920
#define GAME_HEIGHT 1080
#define M2P 100.0f
#define DEBUG_MODE true



class Utils {
public:
	static void logError(const std::string&error);
	static std::vector<std::string> split(const std::string& s, char seperator);

	inline static glm::vec2 b2ToGlm(const b2Vec2&v) { return glm::vec2(v.x, v.y); }
	inline static b2Vec2 glmToB2(const glm::vec2&v) { return b2Vec2(v.x, v.y); }
	inline static float _stof(std::string str) { return strtof(str.c_str(), NULL); }
	inline static int _stoi(std::string str) { return atoi(str.c_str()); }
	template<class T> inline static std::string _to_string(T i) { return std::stringstream(i).str(); }
	inline static float _sign(float a) { return (a >= 0 ? 1.0f : -1.0f); }
	inline static float RandomClamped(float begin = -1.0f, float end = 1.0f) { return (end - begin)*((float)(rand() % (RAND_MAX)) / (float)RAND_MAX) + begin; }
	inline static float Random_0_1() { return ((float)(rand() % (RAND_MAX)) / (float)(RAND_MAX-1)); }

	static void loadFile(char*&buffer, const std::string& filename);
	static void loadFile(std::vector<unsigned char>& buffer, const std::string& filename); //designed for loading files from hard disk in an std::vector
	static b2Vec2 PointToWorldSpace(const b2Vec2 &point, const b2Vec2 &AgentHeading, const b2Vec2 &AgentSide, const b2Vec2 &AgentPosition);
	static b2Vec2 PointToLocalSpace(const b2Vec2 &point, const b2Vec2 &AgentHeading, const b2Vec2 &AgentSide, const b2Vec2 &AgentPosition);
	static b2Vec2 VectorToWorldSpace(const b2Vec2 &vector, const b2Vec2 &AgentHeading, const b2Vec2 &AgentSide);
	static void Truncate(b2Vec2&vector, float max_length);
	static bool check_AABB_overlap(const glm::vec4& a, const glm::vec4& b);
	static bool check_AABB_against_point(const glm::vec4& AABB, const glm::vec2 & point);
	static bool check_AABB_against_point(const b2AABB& AABB, const b2Vec2& point);
	static float _atof(const char*a);
	static float _atoi(const char*a);
	static std::string _atos(const char*a);
	inline static float RadToDeg(float radian) { return radian*180.0f / 3.141592654f; }

	static b2AABB getAABB(b2Body*body);
	template <class T>
	static void Swap(T&a, T&b);

	static glm::vec2 LocalPointInSpartialInfoSpace(const struct SpatialInfo&coord,const glm::vec2&v);
};


struct cmp_str { bool operator()(char const *a, char const *b) const { return std::strcmp(a, b) < 0; } };


#include"List.h"

template<class T> static Node<T>*partition(Node<T>*first, Node<T>*last, bool(*compare_data)(T, T)) {

	T&pivot_data = last->data;
	Node<T>*place_holder = first->prev;

	for (Node<T>*it = first; it!=last->next; it = it->next) {
		if (compare_data(it->data, pivot_data)) {
			place_holder = place_holder->next;
			if (it != place_holder) {
				//swap the data
				T temp = it->data;
				it->data = place_holder->data;
				place_holder->data = temp;
			}
		}
	}
	if(place_holder!=last){
		//swap the data
		T temp = last->data;
		last->data = place_holder->data;
		place_holder->data = temp;
	}

	return place_holder;
}
template<class T> static void quickSort(Node<T>*first, Node<T>*last, bool(*compare_data)(T, T)) {
	if (first == last)return;
	Node<T>*mid = partition(first, last, compare_data);

	if (mid == first)return;
	quickSort(first, mid->prev, compare_data);

	if (mid == last)return;
	quickSort(mid->next, last, compare_data);
}
template<class T> static void sortList(List<T>& list, bool(*compare_data)(T, T))
{
	if (list.empty())return;
	quickSort(list.first(), list.last(), compare_data);
}

template<class T>
inline void Utils::Swap(T & a, T & b)
{
	T temp = a;
	a = b;
	b = temp;
}



#endif ASSISTANCES_H

