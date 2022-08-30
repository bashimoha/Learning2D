#pragma once
#include "2D.h"
#include "yaml-cpp\yaml.h"
#include "filesystem"

class SceneSerializer
{
	public:
	SceneSerializer(Scene* );
	SceneSerializer()=default;
	void Serialize(const std::string path);
	bool Deserialize(std::string path) ;
	//void SerializeEntity( YAML::Emitter &out, std::shared_ptr<Entity> entity );
private: 
	Scene* mScene = nullptr;
};

