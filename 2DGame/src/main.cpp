



#if 1

#include "2D.h"
#include "LevelEditor.h"
#include <box2d.h>
class Test : public Scene
{
public:
	Test(Engine* engine)
		:Scene(engine)
	{

		b2AABB worldAABB;
		auto size = mEngine->Window().getSize();
		worldAABB.lowerBound.Set(-size.x / PIXEL_SCALE_FACTOR, -size.y/PIXEL_SCALE_FACTOR);
		worldAABB.upperBound.Set(size.x / PIXEL_SCALE_FACTOR, size.y/PIXEL_SCALE_FACTOR);
		
		mWorld =  new b2World(b2Vec2(0.0f, -10.0f));
		mWorld->SetAllowSleeping(true);
		mWorld->SetContinuousPhysics(true);
		mView = sf::View( sf::Vector2f(0, 0), sf::Vector2f(10, -10));
	}
	
	void Update(sf::Clock) override
	{
		mEntities.update();
		mWorld->Step(1.0f / 60.0f, 8, 3);
	}
	void Render() override
	{
		mEngine->Window().setView(mView);
		for (auto e : mEntities.getEntities())
		{
			auto& rigidBody = e->getComponent<CRigidBody>();
			auto& transform = e->getComponent<CTransform>();
			transform.position.x = rigidBody.body->GetPosition().x;
			transform.position.y = rigidBody.body->GetPosition().y;
			//convert to degrees
			transform.angle = rigidBody.body->GetAngle() * 180 / b2_pi;
			//rotate the angle by 180 degrees
			transform.angle += 180;
			auto& a = e->getComponent<CAnimation>();
			a.animation.GetSprite().setPosition(transform.position.x, transform.position.y);
			//scale every pixel to 1 meter
			a.animation.GetSprite().setScale(1.f / 100.f, 1.f / 100.f);
			a.animation.GetSprite().setRotation(transform.angle);
			mEngine->Window().draw(a.animation.GetSprite());
		}
		mEngine->Window().setView(mEngine->Window().getDefaultView());
	}
	void  DoAction(const Action& action) override
	{
		mEngine->Window().setView(mView);
		if (action.Name() == "RightClick") 
		{
			///get mouse position and map it to the view
			sf::Vector2f mousePos = mEngine->Window().mapPixelToCoords(sf::Mouse::getPosition(mEngine->Window()));
			std::cout << mousePos.x << " " << mousePos.y << std::endl;
			CreateStaticEntity(mousePos.x, mousePos.y);
			
		}
		if( action.Name() == "LeftClick")
		{
			sf::Vector2f mousePos = mEngine->Window().mapPixelToCoords(sf::Mouse::getPosition(mEngine->Window()));
			std::cout << mousePos.x << " " << mousePos.y << std::endl;
			CreateDynamicEntity(mousePos.x, mousePos.y);
		}
	}
	void CreateDynamicEntity( float x=1, float y=1)
	{
		auto entt = mEntities.addEntity("test");
		entt->addComponent<CTransform>(vec2(x, y), vec2(1.f, 1.f), 1);
		auto& ac = entt->addComponent<CAnimation>(mEngine->GetAsset().getAnimation("rock3"), true);
		auto bodyDef = new b2BodyDef();
		bodyDef->type = b2_dynamicBody;
		bodyDef->position.Set(x, y);
		auto body = mWorld->CreateBody(bodyDef);
		auto shape = new b2PolygonShape();
		//set box relative to the sprite size
		auto sprite = ac.animation.GetSprite();
		float width = sprite.getTextureRect().width / 8.f;
		float height = sprite.getTextureRect().height / 8.f;
		shape->SetAsBox(width / PIXEL_SCALE_FACTOR, height / PIXEL_SCALE_FACTOR / 8);
		auto fixtureDef = new b2FixtureDef();
		fixtureDef->shape = shape;
		fixtureDef->density = 1.f;
		fixtureDef->friction = 0.3f;
		fixtureDef->restitution = 0.5f;
		body->CreateFixture(fixtureDef);
		entt->addComponent<CRigidBody>(body);
		
	}
	void CreateStaticEntity(float x, float y)
	{
		auto entt = mEntities.addEntity("test");
		entt->addComponent<CTransform>(vec2(0, 0), vec2(1.f, 1.f), 1);
		auto& ac = entt->addComponent<CAnimation>(mEngine->GetAsset().getAnimation("shop"), true);
		auto bodyDef = new b2BodyDef();
		bodyDef->type = b2_staticBody;
		bodyDef->position.Set(x, y);
		auto body = mWorld->CreateBody(bodyDef);
		auto shape = new b2PolygonShape();
		auto sprite = ac.animation.GetSprite();
		float width = sprite.getTextureRect().width / 2.f;
		float height = sprite.getTextureRect().height / 2.f;
		shape->SetAsBox(width / PIXEL_SCALE_FACTOR/4, height / PIXEL_SCALE_FACTOR/4);
		auto fixtureDef = new b2FixtureDef();
		fixtureDef->shape = shape;
		fixtureDef->density = 1.f;
		fixtureDef->friction = 0.3f;
		body->CreateFixture(fixtureDef);
		entt->addComponent<CRigidBody>(body);
	}
	sf::View mView;
	std::shared_ptr<Entity> e;
};







int main()
{
	//change directory to  2DGame
	//auto cwd = std::filesystem::current_path();
	//cwd.append("2DGame");
	//std::filesystem::current_path(cwd);

	Engine engine("assets/config.txt");
	engine.ChangeScene("Editor", std::make_shared<Test> (&engine));
	engine.Run();
	return 0;
}
#endif
