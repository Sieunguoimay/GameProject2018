#include "Editor.h"
#include"../Assistances.h"
#include"../Locator.h"
#include"../../misc/tinyXML2/tinyxml2.h"
#include"rapidjson/document.h"
#include"../GameDataLoader.h"

Editor::Editor(World * pWorld, ObjectPool * pObjectPool)
	:m_pWorld(pWorld), m_putter(pObjectPool), m_pObjectPool(pObjectPool)
	,m_picker(pWorld)
{
}

Editor::~Editor()
{
}


void Editor::Init()
{
	//gameWorld->AddPlatform(Editor::LoadObjectFromJson("Resources/RawData/demo.json"));
	//Editor::LoadObjectFromPEXml("Resources/RawData/platform3.xml",this);
	//Editor::LoadObjectFromPEXml("Resources/RawData/platform2.xml", this);
}

void Editor::Update(float deltaTime)
{
	m_putter.Update(deltaTime);
	m_picker.DisableAttach();
}

void Editor::ExecuteCommand(char command)
{
	static bool save_flag = false;
	if (command == 0) {
		if (!save_flag) {
			GameLoader::WriteWorldStructureToFile("Resources/GameData/game_world_data.xml", m_pWorld, m_pObjectPool);
			save_flag = true;
		}
	}
	else save_flag = false;

	//Picker command
	if (m_picker.GetAABBEntity() != NULL) {
		if (command == 1) m_picker.SetZoomFlag(1);
		if (command == 2) m_picker.SetZoomFlag(-1);

		if (command == 5) {
			m_picker.m_pPickedEntity->Done();
			m_picker.m_pPickedEntity = NULL;
		}
	}
	if (command == 4) m_picker.EnableAttach();

	if (command == 3) m_putter.Trigger(GWOID_PLAYER);


}















void Editor::LoadObjectFromPEXml(const char * filename, World*pWorld)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(filename);


	tinyxml2::XMLElement*pBodyDef = doc.FirstChildElement("bodydef");
	if (pBodyDef) {

		tinyxml2::XMLElement*pBodies = pBodyDef->FirstChildElement("bodies");
		if (pBodies) {
			tinyxml2::XMLElement*pBody = pBodies->FirstChildElement("body");
			while (pBody) {

				std::string texture_name = std::string("platforms/") + pBody->Attribute("name") + ".png";
				Texture*texture = Locator::GetAssets()->GetTexture(texture_name);


				tinyxml2::XMLElement*pAnchorPoint = pBody->FirstChildElement("anchorpoint");
				glm::vec2 pos;
				if (pAnchorPoint) {
					std::vector<std::string> origin = Utils::split(std::string(pAnchorPoint->GetText()), ',');
					tinyxml2::XMLUtil::ToFloat(origin[0].c_str(), &pos.x);
					tinyxml2::XMLUtil::ToFloat(origin[1].c_str(), &pos.y);
					pos.x = -pos.x*texture->GetWidth();
					pos.y = -pos.y*texture->GetHeight();
				}

				Platform *newPlatform = new Platform(texture, glm::vec4(pos.x, pos.y, pos.x, pos.y));

				tinyxml2::XMLElement*pFixtures = pBody->FirstChildElement("fixtures");
				if (pFixtures) {
					tinyxml2::XMLElement*pFixture = pFixtures->FirstChildElement("fixture");
					while (pFixture) {
						tinyxml2::XMLElement*pPolygons = pFixture->FirstChildElement("polygons");
						if (pPolygons) {
							tinyxml2::XMLElement*pPolygon = pPolygons->FirstChildElement("polygon");
							while (pPolygon) {

								newPlatform->m_polygons.push_back(std::vector<b2Vec2>());

								std::vector<std::string> polygon = Utils::split(std::string(pPolygon->GetText()), ',');
								for (int i = 0; i < polygon.size() / 2; i++) {
									b2Vec2 vertex;
									tinyxml2::XMLUtil::ToFloat(polygon[2 * i].c_str(), &vertex.x);
									tinyxml2::XMLUtil::ToFloat(polygon[2 * i + 1].c_str(), &vertex.y);

									newPlatform->m_polygons.back().push_back((1.0f / M2P)*vertex);
								}
								pPolygon = pPolygon->NextSiblingElement("polygon");
							}//end of while
						}

						pFixture = pFixture->NextSiblingElement("fixture");
					}//end of while
				}
				pWorld->AddEntity(newPlatform);
				pBody = pBody->NextSiblingElement("body");
			}//end of while
		}
	}
}
//void Editor::WriteWorldStructureToFile(const char * filename, World*pWorld, ObjectPool*pObjectPool)
//{
//	tinyxml2::XMLDocument doc;
//	tinyxml2::XMLNode*pRoot = doc.NewElement("world");
//	tinyxml2::XMLDeclaration*pDec = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
//	doc.InsertFirstChild(pDec);
//	doc.InsertEndChild(pRoot);
//	int count = 0;
//	for (int i = 0; i < pWorld->GetEntities()->size(); i++) {
//		Platform&platform = *pWorld->GetEntities()->at(i);
//		tinyxml2::XMLElement*pPlatform = doc.NewElement("platform");
//		pPlatform->SetAttribute("id", i);
//		pPlatform->SetAttribute("aa_x", platform.GetAABB().x);
//		pPlatform->SetAttribute("aa_y", platform.GetAABB().y);
//		pPlatform->SetAttribute("bb_x", platform.GetAABB().z);
//		pPlatform->SetAttribute("bb_y", platform.GetAABB().w);
//		pPlatform->SetAttribute("texture", platform.GetSkin()->GetTexture()->GetName().c_str());
//
//		pPlatform->SetAttribute("num", platform.m_polygons.size());
//
//		for (int j = 0, n = platform.m_polygons.size(); j < n; j++) {
//
//			tinyxml2::XMLElement*pPolygon = doc.NewElement("polygon");
//			pPolygon->SetAttribute("id", j);
//			pPolygon->SetAttribute("num", platform.m_polygons[j].size());
//
//			for (int k = 0, m = platform.m_polygons[j].size(); k < m; k++) {
//
//				b2Vec2&v = platform.m_polygons[j][k];
//				VertexObject*vo = pWorld->GetVertexObject(count++);
//
//				b2Vec2 vertex(v.x*platform.m_scaleBuffer.x, v.y*platform.m_scaleBuffer.y);
//
//
//				tinyxml2::XMLElement*pVertex = doc.NewElement("vertex");
//				pVertex->SetAttribute("id", k);
//				pVertex->SetAttribute("x", vertex.x);
//				pVertex->SetAttribute("y", vertex.y);
//
//				//vertexObject will be fetched here... and save all the data in to the
//				//child of vertex. :))) call it <object> 
//				if (vo != NULL) {
//					auto&l = vo->GetAttachedEntitites();
//					for (Node<AABBEntity*>*it = l.first(); it != l.tail; it = it->next){
//						auto&b = it->data;
//						const char* object_name = pObjectPool->GetObjectName(b->GetId());
//						const glm::vec4& aabb = b->GetAABB();
//						std::string s =
//							std::to_string(aabb.x) + " " + std::to_string(aabb.y) + " " +
//							std::to_string(aabb.z) + " " + std::to_string(aabb.w);
//
//						tinyxml2::XMLElement*pObject = doc.NewElement("object");
//						pObject->SetAttribute("name", object_name);
//						pObject->SetAttribute("extra", s.c_str());
//
//						pVertex->InsertEndChild(pObject);
//					}
//				}
//
//				pPolygon->InsertEndChild(pVertex);
//			}
//			pPlatform->InsertEndChild(pPolygon);
//		}
//
//		//for (int j = 0; j < platform.m_polygons.size(); j++) {
//		//	tinyxml2::XMLElement*pPolygon = doc.NewElement("polygon");
//		//	pPolygon->SetAttribute("id", j);
//
//		//	for (int k = 0; k < platform.m_polygons.at(j).size(); k++) {
//		//		b2Vec2&vertex = platform.m_polygons[j][k];
//		//		tinyxml2::XMLElement*pVertex = doc.NewElement("vertex");
//		//		pVertex->SetAttribute("id", k);
//		//		pVertex->SetAttribute("x", vertex.x);
//		//		pVertex->SetAttribute("y", vertex.y);
//		//		pPolygon->InsertEndChild(pVertex); 
//
//		//	}
//		//	pPlatform->InsertEndChild(pPolygon);
//		//}
//
//		pRoot->InsertEndChild(pPlatform);
//	}
//
//
//
//	doc.SaveFile(filename);
//	SDL_Log("Saved %s.", filename);
//}

//void Editor::LoadGameWorldFromXml(const char * filename, World * pGameWorld, ObjectPool*pObjectPool, Editor*pEditor)
//{
//}

void Editor::SortEntitiesBySize(List<AABBEntity*>&pEntities)
{
	long int t = SDL_GetTicks();
	sortList(pEntities, &Compare);
	SDL_Log("Sorting time of Entities %f", (float)(SDL_GetTicks() - t) / 1000.0f);
}

bool Editor::Compare(AABBEntity * a, AABBEntity * b)
{
	glm::vec4 aabbA = a->GetAABB();
	glm::vec4 aabbB = b->GetAABB();
	float halfCircumferenceA = (aabbA.z - aabbA.x) + (aabbA.w - aabbA.y);
	float halfCircumferenceB = (aabbB.z - aabbB.x) + (aabbB.w - aabbB.y);
	return halfCircumferenceA <= halfCircumferenceB;
}



//does not support any more
Platform*Editor::LoadObjectFromJson(const char * filename)
{
	//this file support only one platform
	Platform*newPlatform = NULL;
	//this json reader is to load the data from the physics body editor
	char*json_data;
	Utils::loadFile(json_data, filename);
	rapidjson::Document document;
	document.Parse(json_data);

	assert(document.HasMember("rigidBodies"));
	rapidjson::Value&rigidBodies = document["rigidBodies"];
	assert(rigidBodies.IsArray());
	for (auto&rigidBody : rigidBodies.GetArray()) {

		////get texture Id
		//assert(rigidBody.HasMember("imagePath"));
		//rapidjson::Value&imagePath = rigidBody["imagePath"];
		//std::string s(imagePath.GetString());
		//s.erase(0, 12);
		//newPlatform = new Platform(Locator::GetAssets()->GetTexture(s));
		//float w = newPlatform->GetTexture()->GetWidth();
		//float h = newPlatform->GetTexture()->GetHeight();



		//assert(rigidBody.HasMember("origin"));
		//rapidjson::Value&origin = rigidBody["origin"];
		//assert(origin.HasMember("x"));
		//assert(origin.HasMember("y"));
		//glm::vec2 pos;
		//pos.x = -origin["x"].GetFloat()*w;
		//pos.y = -origin["y"].GetFloat()*h;
		//newPlatform->SetPosition(pos);
		////get verices
		//assert(rigidBody.HasMember("polygons"));
		//rapidjson::Value&polygons = rigidBody["polygons"];

		//assert(polygons.IsArray());
		//for (auto&polygon : polygons.GetArray()) {
		//	assert(polygon.IsArray());
		//	PlatformPhysicsBody*newPolygon = new PlatformPhysicsBody();
		//	for (auto&vertex : polygon.GetArray()) {
		//		assert(vertex.HasMember("x"));
		//		assert(vertex.HasMember("y"));
		//		newPolygon->m_vertices.push_back(glm::vec2(
		//			vertex["x"].GetFloat()*w,
		//			vertex["y"].GetFloat()*h));
		//	}
		//	newPlatform->AddPlatformPhysicsBody(newPolygon);
		//}


	}
	free(json_data);
	return newPlatform;
}
