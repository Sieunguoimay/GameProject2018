#include "ScmlObject.h"
#include"../../misc/tinyXML2/tinyxml2.h"
#include"../../misc/Assistances.h"
#include"../../misc/Locator.h"


ScmlObject::~ScmlObject()
{
	for (auto&entity : entities)
		delete entity;
	for (auto&folder : folders)
		delete folder;
}

void ScmlObject::LoadFile(const char * filepath, AssetsManager*assets)
{
	std::string relative_path = "";

	std::string _filepath = std::string(filepath);
	auto found = _filepath.find_last_of('/');
	if (found != std::string::npos) {
		relative_path = _filepath.substr(0, found+1);
	}

	tinyxml2::XMLDocument doc;
	doc.LoadFile(filepath);
	//<spriter_data> ----> scmlObject
	tinyxml2::XMLElement*pRoot = doc.FirstChildElement("spriter_data");
	if (pRoot) {
		//<folder>
		tinyxml2::XMLElement*pFolder = pRoot->FirstChildElement("folder");
		while (pFolder) {

			Folder*folder = new Folder();
			folder->name = Utils::_atos(pFolder->Attribute("name"));
			tinyxml2::XMLElement*pFile = pFolder->FirstChildElement("file");
			//<file>
			while (pFile) {
				File file;
				const char* name = pFile->Attribute("name");
				if (name) {
					if(folder->name!="")
						file.name = relative_path + folder->name + "/" + Utils::_atos(name);
					else
						file.name = relative_path+Utils::_atos(name);

					auto found1 = file.name.find_first_of('/');
					if(found1!=std::string::npos)
						file.name.erase(0, found1+1);

					found1 = file.name.find_first_of('/');
					if (found1 != std::string::npos)
						file.name.erase(0, found1 + 1);

					//SDL_Log("---------------------------%s", file.name.c_str());
					if (assets) {
						file.pTexture = assets->GetTexture(file.name);
						file.textureId = file.pTexture->GetId();
					}
				}
				file.pivotX = Utils::_atof(pFile->Attribute("pivot_x"));
				file.pivotY = Utils::_atof(pFile->Attribute("pivot_y"));

				folder->files.push_back(file);
				pFile = pFile->NextSiblingElement("file");
			}
			this->folders.push_back(folder);
			pFolder = pFolder->NextSiblingElement("folder");
		}
		//<entity>
		std::map<int, int>boneTimelineToRefIdMap;
		tinyxml2::XMLElement*pEntity = pRoot->FirstChildElement("entity");
		while (pEntity) {
			ScmlEntity*entity = new ScmlEntity();
			entity->name = Utils::_atos(pEntity->Attribute("name"));

			bool bodySegnemtsFlag = true;

			//<animation>
			tinyxml2::XMLElement*pAnimation = pEntity->FirstChildElement("animation");
			while (pAnimation) {
				Animation *animation = new Animation(this, entity);
				entity->animations.push_back(animation);
				int controlKeyTemp = 0;

				animation->name = Utils::_atos(pAnimation->Attribute("name"));
				animation->length = Utils::_atoi(pAnimation->Attribute("length"));
				const char*looping = pAnimation->Attribute("looping");
				if (looping) animation->loopType = Utils::_atoi(looping);

				//<mainline>
				tinyxml2::XMLElement*pMainline = pAnimation->FirstChildElement("mainline");
				if (pMainline) {
					//<key>
					tinyxml2::XMLElement*pKey = pMainline->FirstChildElement("key");
					while (pKey) {
						MainlineKey*mainlineKey = new MainlineKey();
						mainlineKey->time = Utils::_atoi(pKey->Attribute("time"));
						//<bone_ref>
						tinyxml2::XMLElement*pBoneRef = pKey->FirstChildElement("bone_ref");
						while (pBoneRef) {
							Ref ref;
							
							const char*parent = pBoneRef->Attribute("parent");
							if (parent) ref.parent = Utils::_atoi(parent);

							ref.timeline = Utils::_atoi(pBoneRef->Attribute("timeline"));
							ref.key = Utils::_atoi(pBoneRef->Attribute("key"));
							ref.z_index = Utils::_atoi(pBoneRef->Attribute("z_index"));
							
							boneTimelineToRefIdMap.insert(std::pair<int, int>(ref.timeline, mainlineKey->boneRefs.size()));
							mainlineKey->boneRefs.push_back(ref);
							pBoneRef = pBoneRef->NextSiblingElement("bone_ref");
						}

						//<object_ref>
						tinyxml2::XMLElement*pObjectRef = pKey->FirstChildElement("object_ref");
						while (pObjectRef) {
							Ref ref;
							const char*parent = pObjectRef->Attribute("parent");
							if (parent) ref.parent = Utils::_atoi(parent);

							ref.timeline = Utils::_atoi(pObjectRef->Attribute("timeline"));
							ref.key = Utils::_atoi(pObjectRef->Attribute("key"));

							mainlineKey->objectRefs.push_back(ref);
							pObjectRef = pObjectRef->NextSiblingElement("object_ref");
						}
						animation->mainlineKeys.push_back(mainlineKey);
						pKey = pKey->NextSiblingElement("key");
					}
				}

				//<timeline>
				tinyxml2::XMLElement*pTimeline = pAnimation->FirstChildElement("timeline");
				int boneTimelineCount = 0;
				int spriteTimelineCount = 0;
				while (pTimeline) {

					bool controlKeyInitFlag2 = true;

					Timeline*timeline = new Timeline;
					timeline->name = Utils::_atos(pTimeline->Attribute("name"));

					timeline->objectType = Timeline::SPRITE;
					if (pTimeline->Attribute("object_type", "bone"))
						timeline->objectType = Timeline::BONE;


					if (timeline->objectType == Timeline::BONE) {

						if(bodySegnemtsFlag)
							entity->bodySegmentMap.insert(std::pair<std::string, int>(timeline->name, boneTimelineToRefIdMap[animation->timelines.size()]));
						//<key>
						tinyxml2::XMLElement*pKey = pTimeline->FirstChildElement("key");
						while (pKey) {
							BoneTimelineKey*m_boneTimelineKey = new BoneTimelineKey();
							const char*time = pKey->Attribute("time");
							if (time)m_boneTimelineKey->time = Utils::_atoi(time);

							const char*spin = pKey->Attribute("spin");
							if (spin)m_boneTimelineKey->spin = Utils::_atoi(spin);

							if (pKey->Attribute("curve_type", "instant"))
								m_boneTimelineKey->curveType = TimelineKey::INSTANT;
							if (pKey->Attribute("curve_type", "linear"))
								m_boneTimelineKey->curveType = TimelineKey::LINEAR;
							if (pKey->Attribute("curve_type", "quadratic")) {
								m_boneTimelineKey->curveType = TimelineKey::QUADRATIC;
								m_boneTimelineKey->c1 = Utils::_atof(pKey->Attribute("c1"));
							}
							if (pKey->Attribute("curve_type", "cubic")) {
								m_boneTimelineKey->curveType = TimelineKey::CUBIC;
								m_boneTimelineKey->c1 = Utils::_atof(pKey->Attribute("c1"));
								m_boneTimelineKey->c2 = Utils::_atof(pKey->Attribute("c2"));
							}
							if (pKey->Attribute("curve_type", "quartic")) {
								m_boneTimelineKey->curveType = TimelineKey::QUARTIC;
								m_boneTimelineKey->c1 = Utils::_atof(pKey->Attribute("c1"));
								m_boneTimelineKey->c2 = Utils::_atof(pKey->Attribute("c2"));
								m_boneTimelineKey->c3 = Utils::_atof(pKey->Attribute("c3"));
							}
							if (pKey->Attribute("curve_type", "quintic")) {
								m_boneTimelineKey->curveType = TimelineKey::QUINTIC;
								m_boneTimelineKey->c1 = Utils::_atof(pKey->Attribute("c1"));
								m_boneTimelineKey->c2 = Utils::_atof(pKey->Attribute("c2"));
								m_boneTimelineKey->c3 = Utils::_atof(pKey->Attribute("c3"));
								m_boneTimelineKey->c4 = Utils::_atof(pKey->Attribute("c4"));
							}

							//const char*paintDebugBones = pKey->Attribute("paintDebugBones");
							//if (paintDebugBones)m_boneTimelineKey->paintDebugBones = Utils::_atoi(paintDebugBones);

							//<bone>
							tinyxml2::XMLElement*pBone = pKey->FirstChildElement("bone");
							if (pBone) {
								const char*x = pBone->Attribute("x");
								if (x) m_boneTimelineKey->info.x = Utils::_atof(x);

								const char*y = pBone->Attribute("y");
								if (y) m_boneTimelineKey->info.y = Utils::_atof(y);

								const char*angle = pBone->Attribute("angle");
								if (angle) m_boneTimelineKey->info.angle = Utils::_atof(angle);

								const char*scaleX = pBone->Attribute("scale_x");
								if (scaleX) m_boneTimelineKey->info.scaleX = Utils::_atof(scaleX);

								const char*scaleY = pBone->Attribute("scale_y");
								if (scaleY)m_boneTimelineKey->info.scaleY = Utils::_atof(scaleY);

								const char*a = pBone->Attribute("a");
								if (a) m_boneTimelineKey->info.a = Utils::_atof(a);

							}

							timeline->keys.push_back(m_boneTimelineKey);
							pKey = pKey->NextSiblingElement("key");
						}
						boneTimelineCount++;
					}


					if (timeline->objectType == Timeline::SPRITE) {

						//<key>
						tinyxml2::XMLElement*pKey = pTimeline->FirstChildElement("key");
						while (pKey) {

							SpriteTimelineKey*m_spriteTimelineKey = new SpriteTimelineKey();

							const char*time = pKey->Attribute("time");
							if (time) m_spriteTimelineKey->time = Utils::_atoi(time);

							const char*spin = pKey->Attribute("spin");
							if (spin)m_spriteTimelineKey->spin = Utils::_atoi(spin);

							if(pKey->Attribute("curve_type","instant"))
								m_spriteTimelineKey->curveType = TimelineKey::INSTANT;
							if (pKey->Attribute("curve_type", "linear"))
								m_spriteTimelineKey->curveType = TimelineKey::LINEAR;
							if (pKey->Attribute("curve_type", "quadratic")) {
								m_spriteTimelineKey->curveType = TimelineKey::QUADRATIC;
								m_spriteTimelineKey->c1 = Utils::_atof(pKey->Attribute("c1"));
							}
							if (pKey->Attribute("curve_type", "cubic")) {
								m_spriteTimelineKey->curveType = TimelineKey::CUBIC;
								m_spriteTimelineKey->c1 = Utils::_atof(pKey->Attribute("c1"));
								m_spriteTimelineKey->c2 = Utils::_atof(pKey->Attribute("c2"));
							}
							if (pKey->Attribute("curve_type", "quartic")) {
								m_spriteTimelineKey->curveType = TimelineKey::QUARTIC;
								m_spriteTimelineKey->c1 = Utils::_atof(pKey->Attribute("c1"));
								m_spriteTimelineKey->c2 = Utils::_atof(pKey->Attribute("c2"));
								m_spriteTimelineKey->c3 = Utils::_atof(pKey->Attribute("c3"));
							}
							if (pKey->Attribute("curve_type", "quintic")) {
								m_spriteTimelineKey->curveType = TimelineKey::QUINTIC;
								m_spriteTimelineKey->c1 = Utils::_atof(pKey->Attribute("c1"));
								m_spriteTimelineKey->c2 = Utils::_atof(pKey->Attribute("c2"));
								m_spriteTimelineKey->c3 = Utils::_atof(pKey->Attribute("c3"));
								m_spriteTimelineKey->c4 = Utils::_atof(pKey->Attribute("c4"));
							}


							//<object>
							tinyxml2::XMLElement*pObject = pKey->FirstChildElement("object");
							if (pObject) {

								const char*x = pObject->Attribute("x");
								if (x) m_spriteTimelineKey->info.x = Utils::_atof(x);

								const char*y = pObject->Attribute("y");
								if (y) m_spriteTimelineKey->info.y = Utils::_atof(y);

								const char*angle = pObject->Attribute("angle");
								if (angle) m_spriteTimelineKey->info.angle = Utils::_atof(angle);

								const char*scaleX = pObject->Attribute("scale_x");
								if (scaleX) m_spriteTimelineKey->info.scaleX = Utils::_atof(scaleX);

								const char*scaleY = pObject->Attribute("scale_y");
								if (scaleY) m_spriteTimelineKey->info.scaleY = Utils::_atof(scaleY);

								const char*a = pObject->Attribute("a");
								if (a)m_spriteTimelineKey->info.a = Utils::_atof(a);

								
								//keep the pointer to file is faster								
								//const char*folder = pObject->Attribute("folder");
								////if (folder)m_spriteTimelineKey->folder = Utils::_atoi(folder);

								//const char*file = pObject->Attribute("file");
								////if (file) m_spriteTimelineKey->file = Utils::_atoi(file);
								//
								m_spriteTimelineKey->folderId = Utils::_atoi(pObject->Attribute("folder"));
								m_spriteTimelineKey->fileId = Utils::_atoi(pObject->Attribute("file"));
								//m_spriteTimelineKey->SetFile(&this->folders[Utils::_atoi(folder)]->files[Utils::_atoi(file)]);
								//so that we dont have to pass on the folder vector all the way down to the spriteTimelineKey->Paint()



								const char*pivot_x = pObject->Attribute("pivot_x");
								if (pivot_x) m_spriteTimelineKey->pivot_x = Utils::_atof(pivot_x);

								const char *pivot_y = pObject->Attribute("pivot_y");
								if (pivot_y) m_spriteTimelineKey->pivot_y = Utils::_atof(pivot_y);

								m_spriteTimelineKey->useDefaultPivot = !(pivot_x&&pivot_y);

							}
							timeline->keys.push_back(m_spriteTimelineKey);
							pKey = pKey->NextSiblingElement("key");
						}
						spriteTimelineCount++;
					}

					animation->timelines.push_back(timeline);
					//animation->timelineMap.insert(std::pair<std::string, int>(timeline->name, animation->timelines.size()-1));
					pTimeline = pTimeline->NextSiblingElement("timeline");
				}//end of while(pTimeline)

				bodySegnemtsFlag = false;
				entity->animationMap.insert(std::pair<std::string, int>(animation->name, entity->animations.size()-1));
				pAnimation = pAnimation->NextSiblingElement("animation");
			}//end of while(pAnimation)

			entity->animationSwitchingTime.resize(entity->animations.size(), std::vector<int>(entity->animations.size(), -1));
			pEntity = pEntity->NextSiblingElement("entity");
			this->entities.push_back(entity);
		}
	}
	
	//for (auto&a : animationMaps)
	//	for(auto&b:*a)
			//SDL_Log("entities size %d", entities.size());
	//this->Log();
}

void ScmlObject::Log()
{
	SDL_Log("spriter_data\n\n");
	for (auto&folder : folders)
		folder->Log();
	for (auto&entity : entities)
		entity->Log();

}


//SpatialInfo ScmlObject::characterInfo()
//{
//	// Fill a SpatialInfo class with the 
//	// x,y,angle,etc of this character in game
//
//	// To avoid distortion the character keep 
//	// scaleX and scaleY values equal
//
//	// Make scaleX or scaleY negative to flip on that axis
//
//	// Examples (scaleX,scaleY)
//	// (1,1) Normal size
//	// (-2.5,2.5) 2.5x the normal size, and flipped on the x axis
//	return SpatialInfo();
//}

float ScmlObject::Draw(float newTime)
{
	if (entities.empty())return 0.0f;
	float temp = entities[currentEntity]->animations[currentAnimation]->UpdateAndDraw(newTime);
	return temp;
}

//void ScmlObject::ApplyCharacterMap(CharacterMap * charMap, bool reset)
//{
//	if (reset)
//	{
//		for(auto&folder:folders)
//			activeCharacterMap.push_back(folder);
//	}
//	for (auto&currentMap:charMap->maps)
//	{
//		if (currentMap.tarFolder>-1 && currentMap.tarFile>-1)
//		{
//			SpriterEngine::Folder*targetFolder = activeCharacterMap[currentMap.tarFolder];
//			SpriterEngine::File targetFile = targetFolder->files[currentMap.tarFile];
//			activeCharacterMap[currentMap.folder]->files[currentMap.file] = targetFile;
//		}
//	}
//}



//<character_map>
//tinyxml2::XMLElement*pCharacterMap = pEntity->FirstChildElement("character_map");
//while (pCharacterMap)
//{
//	CharacterMap*characterMap = new CharacterMap();
//	characterMap->name = pCharacterMap->Attribute("name");

//	//<map>
//	tinyxml2::XMLElement*pMap = pCharacterMap->FirstChildElement("map");
//	while (pMap) {
//		MapInstruction map;
//		map.folder = Utils::_atoi(pMap->Attribute("folder"));
//		map.file = Utils::_atoi(pMap->Attribute("file"));

//		const char*targetFolder = pMap->Attribute("target_folder");
//		if (targetFolder) map.tarFolder = Utils::_atoi(targetFolder);

//		const char*targetFile = pMap->Attribute("target_file");
//		if (targetFile) map.tarFile = Utils::_atoi(targetFile);

//		characterMap->maps.push_back(map);
//		pMap = pMap->NextSiblingElement("map");
//	}


//	entity->characterMaps.push_back(characterMap);
//	pCharacterMap = pCharacterMap->NextSiblingElement("character_map");
//}
