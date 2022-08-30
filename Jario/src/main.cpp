#include "Engine.h"
#include "Menu.h"
#include "LevelEditor.h"

int main()
{
	Engine engine("assets/config.txt");
	engine.ChangeScene("Fart", std::make_shared<Editor>(&engine));
	engine.Run();
}