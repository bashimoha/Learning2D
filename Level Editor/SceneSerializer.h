#pragma once
#include "2D.h"
#include "yaml-cpp\yaml.h"
#include "filesystem"

class SceneSerializer
{
	public:
	SceneSerializer();
	void Serialize(Scene* scene, const std::string path);
	bool Deserialize(Scene* scene, std::string path);
	//void SerializeEntity( YAML::Emitter &out, std::shared_ptr<Entity> entity );
};

