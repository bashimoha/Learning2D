#include "2D.h"

#include "LevelEditor.h"


class DebugSfmlToBox2D : public Scene
{
	public:
		DebugSfmlToBox2D(Engine* e)
			:Scene(e)
		{
			//init physcis world
			mWorld = new b2World(b2Vec2(0.0f, 9.8f));
			//create a box2d body
			

		}	
		~DebugSfmlToBox2D() override
		{
			delete mWorld;
		}
	void Update(sf::Clock) override;
	void Render() override;
	void  DoAction(const Action&) override;



};

int main()
{
	//change directory to  2DGame
	//auto cwd = std::filesystem::current_path();
	//cwd.append("2DGame");
	//std::filesystem::current_path(cwd);

	Engine engine("assets/config.txt");
	engine.ChangeScene("Editor", std::make_shared<Editor> (&engine));
	engine.Run();
	return 0;
}