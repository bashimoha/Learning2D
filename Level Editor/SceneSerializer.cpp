#include "SceneSerializer.h"
static void SerializeEntity(YAML::Emitter& out, std::shared_ptr<Entity> entity);
namespace YAML {

	template<>
	struct convert<vec2>
	{
		static Node encode(const vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};
	//for animation
	template<>
	struct convert<Animation>
	{
		static Node encode(const Animation& rhs)
		{
			Node node;
			node.SetStyle(EmitterStyle::Flow);
			node["name"] = rhs.GetName();
			node["size"] = rhs.GetSize();
			node["frame count"] = rhs.GetFrameCount();
			return node;
		}

		static bool decode(const Node& node, Animation& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;
			rhs.SetName(node[0].as<std::string>());
			rhs.mSize = (node[1].as<vec2>());
			rhs.SetFrame(node[2].as<int>());
		}
	};



	
	YAML::Emitter& operator<<(YAML::Emitter& out, const vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const Animation& a)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << a.GetName() << a.GetSize() << a.GetFrameCount() << YAML::EndSeq;
		return out;
	}
}
SceneSerializer::SceneSerializer()
{
}

void SceneSerializer::Serialize(Scene* scene, const std::string filepath)
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Scene" << YAML::Value << "Untitled";
	out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
	for (const auto e: scene->mEnities.getEntities())
	{
		SerializeEntity(out, e);
	};
	out << YAML::EndSeq;
	out << YAML::EndMap;

	std::ofstream fout(filepath);
	fout << out.c_str();
}


static void SerializeEntity(YAML::Emitter& out, std::shared_ptr<Entity> entity)
{
	out << YAML::BeginMap; 
	out << YAML::Key << "Entity " << YAML::Value << entity->ID();
	out << YAML::Key << "Tag" << YAML::Value << entity->Tag();

	if (entity->hasComponent<CTransform>())
	{
		out << YAML::Key << "Transform Component";
		out << YAML::BeginMap;
		auto tc = entity->getComponent<CTransform>();
		out << YAML::Key << "Position" << YAML::Value<< tc.position;
		out << YAML::Key << "velocity" << YAML::Value << tc.velocity;
		out << YAML::Key << "Scale" << YAML::Value << tc.position;
		out << YAML::Key << "Rotation" << YAML::Value  << tc.angle;
		out << YAML::EndMap;
	}
	if (entity->hasComponent<CGravity>())
	{
		auto gc = entity->getComponent<CGravity>();
		out << YAML::Key << "Gravity Component";
		out << YAML::BeginMap;
		out << YAML::Key << "Gravity" << YAML::Value << gc.gravity;
		out << YAML::Key << "Velocity" << YAML::Value << gc.gravity_velocity;
		out << YAML::EndMap;
	}
	if (entity->hasComponent<CHealth>())
	{
		auto hc = entity->getComponent<CHealth>();
		out << YAML::Key << "Health Component";
		out << YAML::BeginMap;
		out << YAML::Key << "Health" << YAML::Value << hc.health;
		out << YAML::EndMap;
	}
	if (entity->hasComponent<CInput>())
	{
		out << YAML::Key << "Input Component";
		auto ic = entity->getComponent<CInput>();
		out << YAML::BeginMap;
		out << YAML::Key << "Left" << YAML::Value << ic.left;
		out << YAML::Key << "Right" << YAML::Value << ic.right;
		out << YAML::Key << "Up" << YAML::Value << ic.up;
		out << YAML::Key << "Down" << YAML::Value << ic.down;
		out << YAML::EndMap;
	}
	if (entity->hasComponent<CBoundingBox>())
	{
		out << YAML::Key << "Bounding Box Component";
		auto bbc = entity->getComponent<CBoundingBox>();
		out << YAML::BeginMap;
		out << YAML::Key << "Bounding Box" << YAML::Value << bbc.size;
		out << YAML::EndMap;
	}
	if (entity->hasComponent<CState>())
	{
		out << YAML::Key << "State Component";
		out << YAML::BeginMap;
		auto sc = entity->getComponent<CState>();
		out << YAML::Key << "State" << YAML::Value << sc.state;
		out << YAML::EndMap;
	}
	if (entity->hasComponent<CAnimation>())
	{
		out << YAML::Key << "Animation Component";
		out << YAML::BeginMap;
		auto ac = entity->getComponent<CAnimation>();
		out << YAML::Key << "Animation" << YAML::Value << ac.animation << YAML::Key<< "Repeat" << YAML::Value << ac.repeat;
		out << YAML::EndMap;
	}
	if (entity->hasComponent<CDraggable>())
	{
		out << YAML::Key << "Draggable Component";
		out << YAML::BeginMap;
		auto dc = entity->getComponent<CDraggable>();
		out << YAML::Key << "Draggable" << YAML::Value << dc.dragging;
		out << YAML::EndMap;
	}
	out << YAML::EndMap;
	std::cout << out.c_str() << std::endl;
}


bool SceneSerializer::Deserialize(Scene* scene, std::string filepath)
{
	//print out the current working directory
	
	std::cout << "Current working directory is: " << std::filesystem::current_path() << std::endl;
	YAML::Node node;

	try {
		node = YAML::LoadFile(filepath);
	}
	catch (YAML::ParserException e)
	{
		std::cout << "Failed to load " << filepath << " " << e.what();
		return false;
	}
	// get the scene name
	std::string sceneName = node["Scene"].as<std::string>();
	//scene->setName(sceneName);
	// get the entities
	auto entities = node["Entities"];
	if( entities)
	{
		for (auto entity : entities)
		{
			auto tag = entity["Tag"].as<std::string>();
			auto e = scene->mEnities.addEntity(tag);
			auto tc = entity["Transform Component"];
			if (tc)
			{
				auto position = tc["Position"].as<vec2>();
				auto velocity = tc["velocity"].as<vec2>();
				auto scale = tc["Scale"].as<vec2>();
				auto rotation = tc["Rotation"].as<float>();
				e->addComponent<CTransform>( position, velocity, rotation, scale );
			}
			auto gc = entity["Gravity Component"];
			if (gc)
			{
				auto gravity = gc["gravity"].as<float>();
				auto velocity = gc["velocity"].as<float>();
				e->addComponent<CGravity>(gravity, velocity);
			}
			auto hc = entity["Health Component"];
			if (hc)
			{
				auto health = hc["health"].as<float>();
				e->addComponent<CHealth>(health);
			}
			
			auto ic = entity["Input Component"];
			if (ic)
			{
				auto left = ic["Left"].as<bool>();
				auto right = ic["Right"].as<bool>();
				auto up = ic["Up"].as<bool>();
				auto down = ic["Down"].as<bool>();
				auto& input = e->addComponent<CInput>();
				input.left = left; input.right = right; input.up = up; input.down = down;
			
			}
		
			auto bbc = entity["Bounding Box Component"];
			if (bbc)
			{
				auto size = bbc["Bounding Box"].as<vec2>();
				e->addComponent<CBoundingBox>(size);
			}
			
			auto sc = entity["State Component"];
			if (sc)
			{
				auto state = sc["State"].as<std::string>();
				auto& s = e->addComponent<CState>();
				s.state = state;
				
			}
			auto ac = entity["Animation Component"];
			if (ac)
			{
				auto animation = ac["Animation"].as<Animation>();
				auto repeat = ac["Repeat"].as<bool>();
				auto& a = e->addComponent<CAnimation>();
				a.animation = animation;
				a.repeat = repeat;
				
			}
			auto dc = entity["Draggable Component"];
			if (dc)
			{
				auto dragging = dc["Draggable"].as<bool>();
				auto& d = e->addComponent<CDraggable>();
				d.dragging = dragging;
			}
		}
	}
	
	

	
	return true;
}
