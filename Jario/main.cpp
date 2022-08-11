#include "Engine.h"
#include "Menu.h"

int main()
{
	Engine engine("assets/config.txt");
	engine.ChangeScene("Fart", std::make_shared<Menu>(&engine));
	engine.Run();
}