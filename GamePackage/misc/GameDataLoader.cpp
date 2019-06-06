#include "GameDataLoader.h"
#include"../misc/tinyXML2/tinyxml2.h"
#include"../misc/Locator.h"
void GameLoader::LoadGameWorldFromXml(const char * filename, World * pGameWorld, ObjectPool * pObjectPool)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(filename);

	tinyxml2::XMLElement*pWorld = doc.FirstChildElement("world");
	if (pWorld) {
		tinyxml2::XMLElement*pPlatform = pWorld->FirstChildElement("platform");
		while (pPlatform) {

			glm::vec4 aabb(0, 0, 0, 0);
			const char*aax = pPlatform->Attribute("aa_x");
			if (aax != 0)tinyxml2::XMLUtil::ToFloat(aax, &aabb.x);
			const char*aay = pPlatform->Attribute("aa_y");
			if (aay != 0) tinyxml2::XMLUtil::ToFloat(aay, &aabb.y);
			const char*bbx = pPlatform->Attribute("bb_x");
			if (bbx != 0) tinyxml2::XMLUtil::ToFloat(bbx, &aabb.z);
			const char*bby = pPlatform->Attribute("bb_y");
			if (bby != 0) tinyxml2::XMLUtil::ToFloat(bby, &aabb.w);


			Platform*newPlatform = new Platform(
				Locator::GetAssets()->GetTexture(pPlatform->Attribute("texture")), aabb);

			tinyxml2::XMLElement*pPolygon = pPlatform->FirstChildElement("polygon");
			int count_i = 0;
			while (pPolygon) {
				tinyxml2::XMLElement*pVertex = pPolygon->FirstChildElement("vertex");
				newPlatform->m_polygons.push_back(std::vector<b2Vec2>());
				int count_j = 0;
				while (pVertex) {

					b2Vec2 vertex;
					tinyxml2::XMLUtil::ToFloat(pVertex->Attribute("x"), &vertex.x);
					tinyxml2::XMLUtil::ToFloat(pVertex->Attribute("y"), &vertex.y);
					newPlatform->m_polygons.back().push_back(/*(1.0f / M2P)**/vertex);

					//create a vertexObject for this vertex
					pGameWorld->AddVertexObject(count_i, count_j, newPlatform);

					//to fetch the attached object to this vertex
					tinyxml2::XMLElement*pAABBObject = pVertex->FirstChildElement("object");
					while (pAABBObject) {

						//get the info and pass it to the ObjectPool for creating
						ObjectId::ObjectId objectId = pObjectPool->GetObjectId(pAABBObject->Attribute("name"));
						std::string data(pAABBObject->Attribute("extra"));
						AABBEntity* attachedObject = pObjectPool->CreateNewObject(new InfoPacket(objectId, (void*)&data));

						//attach it again into the vertexObject of this vertex
						pGameWorld->Attach(attachedObject, pGameWorld->GetVertexObjects().back());

						pAABBObject = pAABBObject->NextSiblingElement("object");
					}

					count_j++;
					pVertex = pVertex->NextSiblingElement("vertex");
				}
				count_i++;
				pPolygon = pPolygon->NextSiblingElement("polygon");
			}
			((MultiEntity<Platform>*)pGameWorld)->AddEntity(newPlatform);
			pPlatform = pPlatform->NextSiblingElement("platform");
		}
	}
	SDL_Log("Game data loaded.");

}

void GameLoader::WriteWorldStructureToFile(const char * filename, World * pWorld, ObjectPool * pObjectPool)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLNode*pRoot = doc.NewElement("world");
	tinyxml2::XMLDeclaration*pDec = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
	doc.InsertFirstChild(pDec);
	doc.InsertEndChild(pRoot);
	int count = 0;
	for (int i = 0; i < pWorld->GetEntities()->size(); i++) {
		Platform&platform = *pWorld->GetEntities()->at(i);
		tinyxml2::XMLElement*pPlatform = doc.NewElement("platform");
		pPlatform->SetAttribute("id", i);
		pPlatform->SetAttribute("aa_x", platform.GetAABB().x);
		pPlatform->SetAttribute("aa_y", platform.GetAABB().y);
		pPlatform->SetAttribute("bb_x", platform.GetAABB().z);
		pPlatform->SetAttribute("bb_y", platform.GetAABB().w);
		pPlatform->SetAttribute("texture", platform.GetSkin()->GetTexture()->GetName().c_str());

		pPlatform->SetAttribute("num", platform.m_polygons.size());

		for (int j = 0, n = platform.m_polygons.size(); j < n; j++) {

			tinyxml2::XMLElement*pPolygon = doc.NewElement("polygon");
			pPolygon->SetAttribute("id", j);
			pPolygon->SetAttribute("num", platform.m_polygons[j].size());

			for (int k = 0, m = platform.m_polygons[j].size(); k < m; k++) {

				b2Vec2&v = platform.m_polygons[j][k];
				VertexObject*vo = pWorld->GetVertexObject(count++);

				b2Vec2 vertex(v.x*platform.m_scaleBuffer.x, v.y*platform.m_scaleBuffer.y);


				tinyxml2::XMLElement*pVertex = doc.NewElement("vertex");
				pVertex->SetAttribute("id", k);
				pVertex->SetAttribute("x", vertex.x);
				pVertex->SetAttribute("y", vertex.y);

				//vertexObject will be fetched here... and save all the data in to the
				//child of vertex. :))) call it <object> 
				if (vo != NULL) {
					auto&l = vo->GetAttachedEntitites();
					for (Node<AABBEntity*>*it = l.first(); it != l.tail; it = it->next) {
						auto&b = it->data;
						const char* object_name = pObjectPool->GetObjectName(b->GetId());
						const glm::vec4& aabb = b->GetAABB();
						std::string s =
							std::to_string(aabb.x) + " " + std::to_string(aabb.y) + " " +
							std::to_string(aabb.z) + " " + std::to_string(aabb.w);

						tinyxml2::XMLElement*pObject = doc.NewElement("object");
						pObject->SetAttribute("name", object_name);
						pObject->SetAttribute("extra", s.c_str());

						pVertex->InsertEndChild(pObject);
					}
				}

				pPolygon->InsertEndChild(pVertex);
			}
			pPlatform->InsertEndChild(pPolygon);
		}

		//for (int j = 0; j < platform.m_polygons.size(); j++) {
		//	tinyxml2::XMLElement*pPolygon = doc.NewElement("polygon");
		//	pPolygon->SetAttribute("id", j);

		//	for (int k = 0; k < platform.m_polygons.at(j).size(); k++) {
		//		b2Vec2&vertex = platform.m_polygons[j][k];
		//		tinyxml2::XMLElement*pVertex = doc.NewElement("vertex");
		//		pVertex->SetAttribute("id", k);
		//		pVertex->SetAttribute("x", vertex.x);
		//		pVertex->SetAttribute("y", vertex.y);
		//		pPolygon->InsertEndChild(pVertex); 

		//	}
		//	pPlatform->InsertEndChild(pPolygon);
		//}

		pRoot->InsertEndChild(pPlatform);
	}



	doc.SaveFile(filename);
	SDL_Log("Saved %s.", filename);
}
