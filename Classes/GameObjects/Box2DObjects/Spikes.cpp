#include "Spikes.h"

using namespace cocos2d;

void Spikes::createBodyAtLocation(CCPoint location)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = b2Vec2(location.x/PTM_RATIO,	location.y/PTM_RATIO);
	this->setBody(world->CreateBody(&bodyDef));
	body->SetUserData(this);
	b2PolygonShape shape;
	shape.SetAsBox(this->boundingBox().size.width/2/PTM_RATIO, this->boundingBox().size.height/2/PTM_RATIO,
		b2Vec2(0, +5.0/PTM_RATIO * SCREEN_SIZE_PX.height/640), 0);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1000.0;
	body->CreateFixture(&fixtureDef);
}

bool Spikes::initWithWorld( b2World *theWorld, cocos2d::CCPoint location )
{
	bool pRet = false;

	if (Box2DSprite::init())
	{
		world = theWorld;
		this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("spikes.png"));
		gameObjectType = kSpikesType;
		this->createBodyAtLocation(location);

		pRet = true;
	}

	return pRet;
}
