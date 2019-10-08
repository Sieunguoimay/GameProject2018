#include "..\..\..\GameEngine\GamePackage\misc\Assistances.h"
#include"Assistances.h"
#include"SDL2\SDL_rwops.h"
#include"../GameObjects/SpriterEngine/TimelineKey.h"
void Utils::logError(const std::string&error) {
	SDL_Log("Error: %s", error.c_str());
	exit(69);
}

std::vector<std::string> Utils::split(const std::string& s, char seperator)
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



float Utils::_atof(const char*a) {
	if (a) {
		return atof(a);
	}
	return 0;
}
float Utils::_atoi(const char*a) {
	if (a) {
		return atoi(a);
	}
	return 0;
}
std::string Utils::_atos(const char*a) {
	if (a) {
		return std::string(a);
	}
	return "";
}

b2AABB Utils::getAABB(b2Body * body)
{
	b2AABB aabb;
	for (auto it = body->GetFixtureList(); it != NULL; it = it->GetNext()) {
		b2AABB aabb2 = it->GetAABB(0);
		if (aabb.lowerBound.x < aabb2.lowerBound.x)
			aabb.lowerBound.x = aabb2.lowerBound.x;
		if (aabb.lowerBound.y < aabb2.lowerBound.y)
			aabb.lowerBound.y = aabb2.lowerBound.y;
		if (aabb.upperBound.x > aabb2.upperBound.x)
			aabb.upperBound.x = aabb2.upperBound.x;
		if (aabb.upperBound.y > aabb2.upperBound.y)
			aabb.upperBound.y = aabb2.upperBound.y;
	}
	return aabb;
}

glm::vec2 Utils::LocalPointInSpartialInfoSpace(const SpatialInfo & coord, const glm::vec2 & v)
{
	//relative position w.r.t coord space
	glm::vec2 p(v.x - coord.x, v.y - coord.y);

	//coordinates of p on coord reference frame
	glm::vec2 p2(
		p.x*cos(coord.angle) - p.y*sin(coord.angle),
		p.x*sin(coord.angle) + p.y*cos(coord.angle));

	//finally scale
	p2.x /= coord.scaleX;
	p2.y /= coord.scaleY;

	return p2;
}



void Utils::loadFile(char*&buffer, const std::string& filename) {
	SDL_RWops* rwops = SDL_RWFromFile(filename.c_str(), "rb");
	if (rwops == NULL)
	{
		SDL_Log("Could not open file \"%s\".\n", filename.c_str());
		return;
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
void Utils::loadFile(std::vector<unsigned char>& buffer, const std::string& filename) //designed for loading files from hard disk in an std::vector
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

bool Utils::check_AABB_overlap(const glm::vec4& a, const glm::vec4& b){
	if (a.x + a.z < b.x)return false;
	if (a.x > b.x + b.z)return false;
	if (a.y + a.w < b.y)return false;
	if (a.y > b.y + b.w)return false;
	return true;
}

bool Utils::check_AABB_against_point(const glm::vec4& AABB,const glm::vec2 & point)
{
	if (point.x < AABB.x) return false;
	if (point.x > AABB.z) return false;
	if (point.y < AABB.y) return false;
	if (point.y > AABB.w) return false;
	return true;
}
bool Utils::check_AABB_against_point(const b2AABB& AABB, const b2Vec2& point)
{
	if (point.x < AABB.lowerBound.x) return false;
	if (point.x > AABB.upperBound.x) return false;
	if (point.y < AABB.lowerBound.y) return false;
	if (point.y > AABB.upperBound.y) return false;
	return true;
}



b2Vec2 Utils::PointToWorldSpace(const b2Vec2 &point,
	const b2Vec2 &AgentHeading,
	const b2Vec2 &AgentSide,
	const b2Vec2 &AgentPosition)
{
	b2Vec2 head = point.x*AgentHeading;
	b2Vec2 side = point.y*AgentSide;
	b2Vec2 transPoint = AgentPosition + head + side;
	return transPoint;
}

b2Vec2 Utils::PointToLocalSpace(const b2Vec2 &point,
	const b2Vec2 &AgentHeading,
	const b2Vec2 &AgentSide,
	const b2Vec2 &AgentPosition)
{
	b2Vec2 _point = point - AgentPosition;
	float x = b2Dot(AgentHeading, _point) / AgentHeading.Length();
	float y = b2Dot(AgentSide, _point) / AgentSide.Length();

	return b2Vec2(x, y);
}
b2Vec2 Utils::VectorToWorldSpace(const b2Vec2 &vector,
	const b2Vec2 &AgentHeading,
	const b2Vec2 &AgentSide)
{
	b2Vec2 head = vector.x*AgentHeading;
	b2Vec2 side = vector.y*AgentSide;
	return b2Vec2(head.x + side.x, head.y + side.y);
}
void Utils::Truncate(b2Vec2&vector, float max_length) {
	if (vector.LengthSquared() > max_length*max_length) {
		vector.Normalize();
		vector *= max_length;
	}
}
