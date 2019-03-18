#include "Editor.h"
#include"../Assistances.h"
#include"../Locator.h"
#include"../../misc/tinyXML2/tinyxml2.h"

Editor::Editor()
	:m_pWorld(NULL)
{
}

Editor::~Editor()
{
	for (auto&a : m_editorObjects) {
		delete a.first;
		for (auto&b : *a.second)
			delete b;
		delete a.second;
	}
}

void Editor::Init(World*pWorld,const char * filename)
{
	m_pWorld = pWorld;
	for(auto&platform:*m_pWorld->GetPlatform()){
		std::vector<EditorObject*>*vertexObjects = new std::vector<EditorObject*>();
		for (auto&a : platform->GetPolygons())
			for (auto&b:a->m_vertices)
				vertexObjects->push_back(new EditorObject(&b,NULL,NULL));

		m_editorObjects.push_back(
			std::pair<EditorObject*, std::vector<EditorObject*>*>(
				new EditorObject(&platform->GetPosition(), &platform->GetTexture()->GetDimension(), platform->GetTexture())
				, vertexObjects
				));
	}
}





void Editor::Update(float deltaTime)
{
	static bool save_flag = false;
	if (Locator::GetInputEvent()->IsKeyPressed(KEY_CTRL) &&
		Locator::GetInputEvent()->IsKeyPressed(KEY_S)) {
		if (!save_flag) {
			WriteWorldStructureToFile("Resources/GameData/game_world_data.xml",m_pWorld);
			save_flag = true;
		}
	}
	else save_flag = false;

	for(auto&platform:m_editorObjects){
		//bool temp = false;
		for (auto&a : *platform.second)
			if (a->Update(deltaTime, -*platform.first->m_pPos)) 
				return;

		//if (!temp)
			platform.first->Update(deltaTime);
	}

}

void Editor::Draw()
{
	for(auto&platform:m_editorObjects){
		std::vector<EditorObject*>&m_vertexObjects = *platform.second;

		for (int i = 0; i < m_vertexObjects.size(); i++) {
			auto&a = m_vertexObjects[i];
			glm::vec2 pos = *platform.first->m_pPos;
			a->Draw(-pos);

			if (i > 0) {
				auto&a_prev = m_vertexObjects[i - 1];
				Locator::GetPrimitiveRenderer()->DrawLine(
					pos + *a_prev->m_pPos, pos + *a->m_pPos,
					glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
			}
			else {
				auto&a_prev = m_vertexObjects[m_vertexObjects.size()-1];
				Locator::GetPrimitiveRenderer()->DrawLine(
					pos +*a_prev->m_pPos, pos + *a->m_pPos,
					glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
			}
		}
		//for (auto&a : m_editorObjects)
		platform.first->Draw();
	}
}









bool EditorObject::Update(float deltaTime,glm::vec2 origin)
{
	m_clickEvent = false;
	m_hover = false;
	if (Locator::GetInputEvent()->IsMousePressed()) {
		if (!m_hold) {
			//fist click
			m_clickEvent = true;
		}
		m_hold = true;
	}
	else {
		//release
		m_hold = false;
		m_clickOnBox = false;
	}
	glm::vec4 box(-origin + *m_pPos, 10.0f, 10.0f);
	if (m_pSize != NULL) {
		box.z = m_pSize->x;
		box.w = m_pSize->y;
	}
	glm::vec4 point(Locator::GetInputEvent()->GetMousePosInWorld(), 1, 1);
	if (check_overlap(point, box))
	{
		m_hover = true;
		if (m_clickEvent) {
			oldpos = *m_pPos;
			oldmouse = point;
			m_clickOnBox = true;
		}
	}

	if (m_clickOnBox) {
		*m_pPos = oldpos + Locator::GetInputEvent()->GetMousePosInWorld() - oldmouse;
		return true;
	}

	return false;
}

void EditorObject::Draw(glm::vec2 origin)
{
	if (m_pPos == NULL) return;






	if (m_pTexture != NULL)
		Locator::GetTextureRenderer()->Draw(glm::vec4(
			-origin+*m_pPos,m_pTexture->GetWidth(),m_pTexture->GetHeight()
			),glm::vec4(0,0,1,1),m_pTexture->GetId(),0,
			glm::vec4(1.0f));
	glm::vec4 rect = glm::vec4(
		-origin+*m_pPos,
		10.0f, 10.0f);



	if (m_pSize != NULL) {
		rect.z = m_pSize->x;
		rect.w = m_pSize->y;

	}
	else {
		if (m_hold) {
			rect.z = 20.0f;
			rect.w = 20.0f;
			rect.x -= 10.0f;
			rect.y -= 10.0f;
			Locator::GetPrimitiveRenderer()->FillBox(rect, 0, glm::vec2(0.0f),
				glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
			return;
		}
	}


	if (m_hover)
		Locator::GetPrimitiveRenderer()->DrawBox(rect, 0, glm::vec2(0.0f),
			glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));

	else
		Locator::GetPrimitiveRenderer()->DrawBox(rect, 0, glm::vec2(0.0f),
			glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

}



#include"rapidjson/document.h"
Platform*Editor::LoadObjectFromJson(const char * filename)
{
	//this file support only one platform
	Platform*newPlatform = NULL;
	//this json reader is to load the data from the physics body editor
	char*json_data;
	loadFile(json_data, filename);
	rapidjson::Document document;
	document.Parse(json_data);

	assert(document.HasMember("rigidBodies"));
	rapidjson::Value&rigidBodies = document["rigidBodies"];
	assert(rigidBodies.IsArray());
	for (auto&rigidBody : rigidBodies.GetArray()) {

		//get texture Id
		assert(rigidBody.HasMember("imagePath"));
		rapidjson::Value&imagePath = rigidBody["imagePath"];
		std::string s(imagePath.GetString());
		s.erase(0, 12);
		newPlatform = new Platform(Locator::GetAssets()->GetTexture(s));
		float w = newPlatform->GetTexture()->GetWidth();
		float h = newPlatform->GetTexture()->GetHeight();



		assert(rigidBody.HasMember("origin"));
		rapidjson::Value&origin = rigidBody["origin"];
		assert(origin.HasMember("x"));
		assert(origin.HasMember("y"));
		glm::vec2 pos;
		pos.x = -origin["x"].GetFloat()*w;
		pos.y = -origin["y"].GetFloat()*h;
		newPlatform->SetPosition(pos);
		//get verices
		assert(rigidBody.HasMember("polygons"));
		rapidjson::Value&polygons = rigidBody["polygons"];

		assert(polygons.IsArray());
		for (auto&polygon : polygons.GetArray()) {
			assert(polygon.IsArray());
			PlatformPhysicsBody*newPolygon = new PlatformPhysicsBody();
			for (auto&vertex : polygon.GetArray()) {
				assert(vertex.HasMember("x"));
				assert(vertex.HasMember("y"));
				newPolygon->m_vertices.push_back(glm::vec2(
					vertex["x"].GetFloat()*w,
					vertex["y"].GetFloat()*h));
			}
			newPlatform->AddPlatformPhysicsBody(newPolygon);
		}


	}
	free(json_data);
	return newPlatform;
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

				std::string texture_name = std::string("platforms/")+ pBody->Attribute("name")+".png";
				Texture*texture = Locator::GetAssets()->GetTexture(texture_name);
				Platform *newPlatform = new Platform(texture);

				tinyxml2::XMLElement*pAnchorPoint = pBody->FirstChildElement("anchorpoint");
				if (pAnchorPoint) {
					std::vector<std::string> origin = split(std::string(pAnchorPoint->GetText()),',');
					glm::vec2 pos;
					tinyxml2::XMLUtil::ToFloat(origin[0].c_str(), &pos.x);
					tinyxml2::XMLUtil::ToFloat(origin[1].c_str(), &pos.y);
					pos.x = -pos.x*texture->GetWidth();
					pos.y = -pos.y*texture->GetHeight();
					newPlatform->SetPosition(pos);
				}
				tinyxml2::XMLElement*pFixtures = pBody->FirstChildElement("fixtures");
				if (pFixtures) {
					tinyxml2::XMLElement*pFixture = pFixtures->FirstChildElement("fixture");
					while (pFixture) {
						tinyxml2::XMLElement*pPolygons = pFixture->FirstChildElement("polygons");
						if (pPolygons) {
							tinyxml2::XMLElement*pPolygon = pPolygons->FirstChildElement("polygon");
							while (pPolygon) {

								PlatformPhysicsBody*vertices = new PlatformPhysicsBody();
								SDL_Log("%s", pPolygon->GetText());

								std::vector<std::string> polygon = split(std::string(pPolygon->GetText()), ',');
								for (int i = 0; i < polygon.size() / 2; i++) {
									glm::vec2 vertex;
									tinyxml2::XMLUtil::ToFloat(polygon[2*i].c_str(), &vertex.x);
									tinyxml2::XMLUtil::ToFloat(polygon[2 * i+1].c_str(), &vertex.y);
									SDL_Log("%f %f", vertex.x, vertex.y);
									vertices->m_vertices.push_back(vertex);
								}
								newPlatform->AddPlatformPhysicsBody(vertices);
								pPolygon = pPolygon->NextSiblingElement("polygon");
							}//end of while
						}

						pFixture = pFixture->NextSiblingElement("fixture");
					}//end of while
				}
				newPlatform->Init();
				pWorld->AddPlatform(newPlatform);
				pBody = pBody->NextSiblingElement("body");
			}//end of while
		}
	}
}
void Editor::WriteWorldStructureToFile(const char * filename, World*pWorld)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLNode*pRoot = doc.NewElement("world");
	tinyxml2::XMLDeclaration*pDec = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF - 8\"");
	doc.InsertFirstChild(pDec);
	doc.InsertEndChild(pRoot);

	for (int i = 0; i < pWorld->GetPlatform()->size(); i++) {
		Platform&platform = *pWorld->GetPlatform()->at(i);
		tinyxml2::XMLElement*pPlatform = doc.NewElement("platform");
		pPlatform->SetAttribute("id", i);
		pPlatform->SetAttribute("x", platform.GetPosition().x);
		pPlatform->SetAttribute("y", platform.GetPosition().y);
		pPlatform->SetAttribute("texture", platform.GetTexture()->GetName().c_str());

		for (int j = 0; j < platform.GetPolygons().size(); j++) {
			tinyxml2::XMLElement*pPolygon = doc.NewElement("polygon");
			pPolygon->SetAttribute("id", j);
			for (int k = 0; k < platform.GetPolygons().at(j)->m_vertices.size(); k++) {
				glm::vec2&vertex = platform.GetPolygons().at(j)->m_vertices.at(k);
				tinyxml2::XMLElement*pVertex = doc.NewElement("vertex");
				pVertex->SetAttribute("id", k);
				pVertex->SetAttribute("x", vertex.x);
				pVertex->SetAttribute("y", vertex.y);
				pPolygon->InsertEndChild(pVertex); 

			}
			pPlatform->InsertEndChild(pPolygon);
		}
		pRoot->InsertEndChild(pPlatform);
	}



	doc.SaveFile(filename);
	SDL_Log("Saved %s", filename);
}

#include"../../misc/Locator.h"
void Editor::LoadGameWorldFromXml(const char * filename, World * pGameWorld)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(filename);

	tinyxml2::XMLElement*pWorld = doc.FirstChildElement("world");
	if (pWorld) {
		tinyxml2::XMLElement*pPlatform = pWorld->FirstChildElement("platform");
		while (pPlatform) {
			Platform*newPlatform = new Platform(Locator::GetAssets()->GetTexture(pPlatform->Attribute("texture")));
			glm::vec2 pos;
			tinyxml2::XMLUtil::ToFloat(pPlatform->Attribute("x"),&pos.x);
			tinyxml2::XMLUtil::ToFloat(pPlatform->Attribute("y"), &pos.y);
			newPlatform->SetPosition(pos);

			tinyxml2::XMLElement*pPolygon = pPlatform->FirstChildElement("polygon");
			while (pPolygon) {

				PlatformPhysicsBody*newPolygon = new PlatformPhysicsBody();

				tinyxml2::XMLElement*pVertex = pPolygon->FirstChildElement("vertex");
				while (pVertex) {
	
					glm::vec2 vertex;
					tinyxml2::XMLUtil::ToFloat(pVertex->Attribute("x"), &vertex.x);
					tinyxml2::XMLUtil::ToFloat(pVertex->Attribute("y"), &vertex.y);
					newPolygon->m_vertices.push_back(vertex);

					pVertex = pVertex->NextSiblingElement("vertex");
				}
				pPolygon = pPolygon->NextSiblingElement("polygon");

				newPlatform->AddPlatformPhysicsBody(newPolygon);
			}
			pPlatform = pPlatform->NextSiblingElement("platform");

			newPlatform->Init();
			pGameWorld->AddPlatform(newPlatform);
		}
	}
}
