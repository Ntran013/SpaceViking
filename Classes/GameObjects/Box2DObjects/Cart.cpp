#include "Cart.h"

using namespace cocos2d;


void Cart::createBodyAtLocation(CCPoint location)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);
	body = world->CreateBody(&bodyDef);
	body->SetUserData(this);

	b2FixtureDef fixtureDef;
	b2PolygonShape shape;
	//shape.SetAsBox(this->boundingBox().size.width/2/PTM_RATIO, this->boundingBox().size.height/2/PTM_RATIO);
	int num = 4;
	// set scaling
	b2Vec2 verts[] = {b2Vec2((77.5f / PTM_RATIO) * SCREEN_HEIGHT/640, (37.0f / PTM_RATIO) * SCREEN_HEIGHT/640),
					b2Vec2((-78.5f / PTM_RATIO) * SCREEN_HEIGHT/640, (38.0f / PTM_RATIO) * SCREEN_HEIGHT/640),
					b2Vec2((-60.5f / PTM_RATIO) * SCREEN_HEIGHT/640, (-37.0f / PTM_RATIO) * SCREEN_HEIGHT/640),
					b2Vec2((56.5f / PTM_RATIO) * SCREEN_HEIGHT/640, (-38.0f / PTM_RATIO) * SCREEN_HEIGHT/640)};
	shape.Set(verts, num);
	fixtureDef.shape = &shape;

	fixtureDef.density = 1.0;
	fixtureDef.friction = 0.5;
	fixtureDef.restitution = 0.5;
	
	body->CreateFixture(&fixtureDef); //body->SetAngularDamping(10000000000000000);
}

bool Cart::initWithWorld(b2World *theWorld, CCPoint location)
{
	bool pRet = false;

	if(Box2DSprite::init())
	{
		world = theWorld;
		this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("Cart.png"));
		gameObjectType = kCartType;
		characterHealth = 100.0f;
		this->createBodyAtLocation(location);

		this->createWheels();

		pRet = true;
	}

	return pRet;
}

b2Body * Cart::createWheelWithSprite(Box2DSprite *sprite, b2Vec2 offset)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = body->GetWorldPoint(offset);
	b2Body *wheelBody = world->CreateBody(&bodyDef);
	wheelBody->SetUserData(sprite);
	sprite->setBody(wheelBody);

	b2CircleShape circleShape;
	circleShape.m_radius = sprite->boundingBox().size.width/2/PTM_RATIO;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.friction = 1.0;
	fixtureDef.restitution = 0.2;
	fixtureDef.density = 10.0;
	wheelBody->CreateFixture(&fixtureDef);

	return wheelBody;
}

void Cart::createWheels()
{
	wheelL = new Box2DSprite();
	wheelL->initWithSpriteFrameName("Wheel.png");
	wheelL->autorelease();
	wheelL->setGameObjectType(kCartType);
	wheelLBody = this->createWheelWithSprite(wheelL, b2Vec2(-63.0/PTM_RATIO * SCREEN_HEIGHT/640, -48.0/PTM_RATIO * SCREEN_HEIGHT/640));

	wheelR = new Box2DSprite();
	wheelR->initWithSpriteFrameName("Wheel.png");
	wheelR->autorelease();
	wheelR->setGameObjectType(kCartType);
	wheelRBody = this->createWheelWithSprite(wheelR, b2Vec2(63.0/PTM_RATIO * SCREEN_HEIGHT/640, -48.0/PTM_RATIO * SCREEN_HEIGHT/640));

	b2RevoluteJointDef revJointDef;
	revJointDef.Initialize(body, wheelLBody, wheelLBody->GetWorldCenter());

	revJointDef.enableMotor = true;
	revJointDef.maxMotorTorque = 1000;
	revJointDef.motorSpeed = 0;

	wheelLJoint = (b2RevoluteJoint *) world->CreateJoint(&revJointDef);

	revJointDef.Initialize(body, wheelRBody, wheelRBody->GetWorldCenter());
	wheelRJoint = (b2RevoluteJoint *) world->CreateJoint(&revJointDef);
}

void Cart::setMotorSpeed( float32 motorSpeed )
{
	if (characterState != kStateTakingDamage)
	{
		wheelLJoint->SetMotorSpeed(motorSpeed);
		wheelRJoint->SetMotorSpeed(motorSpeed);
	}	

	else
	{
		wheelLJoint->SetMotorSpeed(0.2 * motorSpeed);
		wheelRJoint->SetMotorSpeed(0.2 * motorSpeed);
	}
}

