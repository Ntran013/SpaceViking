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
	b2Vec2 verts[] = {b2Vec2((77.5f / PTM_RATIO) * SCREEN_SIZE_PX.height/640, (37.0f / PTM_RATIO) * SCREEN_SIZE_PX.height/640),
					b2Vec2((-78.5f / PTM_RATIO) * SCREEN_SIZE_PX.height/640, (38.0f / PTM_RATIO) * SCREEN_SIZE_PX.height/640),
					b2Vec2((-60.5f / PTM_RATIO) * SCREEN_SIZE_PX.height/640, (-37.0f / PTM_RATIO) * SCREEN_SIZE_PX.height/640),
					b2Vec2((56.5f / PTM_RATIO) * SCREEN_SIZE_PX.height/640, (-38.0f / PTM_RATIO) * SCREEN_SIZE_PX.height/640)};
	shape.Set(verts, num);
	fixtureDef.shape = &shape;

	fixtureDef.density = 320/SCREEN_HEIGHT_PX;
	fixtureDef.friction = 0.5;
	fixtureDef.restitution = 0.5;

	fixtureDef.filter.categoryBits = 0x2;
	fixtureDef.filter.maskBits = 0xFFFF;
	fixtureDef.filter.groupIndex = -1;
	
	body->CreateFixture(&fixtureDef); //body->SetAngularDamping(100000);

	b2PolygonShape sensorShape;
	sensorShape.SetAsBox(this->boundingBox().size.width/2/PTM_RATIO,this->boundingBox().size.height/4/PTM_RATIO, b2Vec2(0, -this->boundingBox().size.height/PTM_RATIO), 0);
	fixtureDef.shape = &sensorShape;
	fixtureDef.density = 0.0;
	fixtureDef.isSensor = true;
	body->CreateFixture(&fixtureDef);
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
		this->createOle();

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

	fixtureDef.filter.categoryBits = 0x2;
	fixtureDef.filter.maskBits = 0xFFFF;
	fixtureDef.filter.groupIndex = -1;

	return wheelBody;
}

void Cart::createWheels()
{
	wheelL = new Box2DSprite();
	wheelL->initWithSpriteFrameName("Wheel.png");
	wheelL->autorelease();
	wheelL->setGameObjectType(kCartType);
	wheelLBody = this->createWheelWithSprite(wheelL, b2Vec2(-63.0/PTM_RATIO * SCREEN_SIZE_PX.height/640, -48.0/PTM_RATIO * SCREEN_SIZE_PX.height/640));

	wheelR = new Box2DSprite();
	wheelR->initWithSpriteFrameName("Wheel.png");
	wheelR->autorelease();
	wheelR->setGameObjectType(kCartType);
	wheelRBody = this->createWheelWithSprite(wheelR, b2Vec2(63.0/PTM_RATIO * SCREEN_SIZE_PX.height/640, -48.0/PTM_RATIO * SCREEN_SIZE_PX.height/640));

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

void Cart::updateStateWithDeltaTime( cocos2d::ccTime deltaTime, cocos2d::CCArray *listOfGameObjects )
{
	float32 minAngle = CC_DEGREES_TO_RADIANS(-20);
	float32 maxAngle = CC_DEGREES_TO_RADIANS(20); 
	float32 desiredAngle = this->getBody()->GetAngle();
	if (this->getBody()->GetAngle() > maxAngle)
		desiredAngle = maxAngle;
	else if (this->getBody()->GetAngle() < minAngle)
		desiredAngle = minAngle;

	float32 diff = desiredAngle - this->getBody()->GetAngle();
	if (diff != 0)
	{
		body->SetAngularVelocity(0);
		float32 diff = desiredAngle - this->getBody()->GetAngle();
		float angimp = this->getBody()->GetInertia() * diff;
		this->getBody()->ApplyAngularImpulse(angimp * 2);
	}

	if (characterState == kStateDead)
		return;

	if (characterState == kStateTakingDamage && this->numberOfRunningActions() > 0)
	{
		return;
	}

	if (this->numberOfRunningActions() == 0)
	{
		if (characterHealth <= 0)
			this->changeState(kStateDead);
		else 
			this->changeState(kStateIdle);
	}

	if (isBodyCollidingWithObjectType(wheelLBody, kSpikesType))
		this->changeState(kStateTakingDamage);
	else if (isBodyCollidingWithObjectType(wheelRBody, kSpikesType))
		this->changeState(kStateTakingDamage);
}

void Cart::playJumpEffect()
{
	int soundToPlay = rand() % 4;
	if (soundToPlay == 0) 
	{
		PLAYSOUNDEFFECT(VIKING_JUMPING_1);
	} 
	else if (soundToPlay == 1) 
	{
		PLAYSOUNDEFFECT(VIKING_JUMPING_2);
	} 
	else if (soundToPlay == 2) 
	{
		PLAYSOUNDEFFECT(VIKING_JUMPING_3);
	} 
	else 
	{
		PLAYSOUNDEFFECT(VIKING_JUMPING_4);
	}
}

float32 Cart::fullMass()
{
	return body->GetMass() + wheelLBody->GetMass() + wheelRBody->GetMass();
}

void Cart::jump()
{
	this->playJumpEffect();
	b2Vec2 impulse = b2Vec2(this->fullMass()*1.0, this->fullMass()*5.0);
	b2Vec2 impulsePoint = body->GetWorldPoint(b2Vec2(5.0/PTM_RATIO * SCREEN_HEIGHT_PX/640, -15.0/PTM_RATIO * SCREEN_HEIGHT_PX/640));
	body->ApplyLinearImpulse(impulse, impulsePoint);

}

b2Body * Cart::createPartAtLocation(b2Vec2 location, Box2DSprite *sprite)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = location;
	bodyDef.angle = body->GetAngle();

	b2Body *retval = world->CreateBody(&bodyDef);
	retval->SetUserData(sprite);
	sprite->setBody(retval);

	b2PolygonShape shape;
	shape.SetAsBox(sprite->boundingBox().size.width/2/PTM_RATIO,
		sprite->boundingBox().size.height/2/PTM_RATIO);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 0.05;
	fixtureDef.filter.categoryBits = 0x2;
	fixtureDef.filter.maskBits = 0xFFFF;
	fixtureDef.filter.groupIndex = -1;

	retval->CreateFixture(&fixtureDef);
	return retval;
}

void Cart::createOle()
{
	legs = new Box2DSprite();
	legs->initWithSpriteFrameName("OleCartLegs.png");
	legs->autorelease();
	legs->setGameObjectType(kCartType);
	legsBody = this->createPartAtLocation(body->GetWorldPoint(b2Vec2(-10.0/PTM_RATIO * SCREEN_SIZE_PX.height/640, 6.0/PTM_RATIO * SCREEN_SIZE_PX.height/640)), legs);

	trunk = new Box2DSprite();
	trunk->initWithSpriteFrameName("OleCartBody.png");
	trunk->autorelease();
	trunk->setGameObjectType(kCartType);
	trunkBody = this->createPartAtLocation(legsBody->GetWorldPoint(b2Vec2(0.0f, 45.0/PTM_RATIO * SCREEN_SIZE_PX.height/640)), trunk);

	head = new Box2DSprite();
	head->initWithSpriteFrameName("OleCartHead.png");
	head->autorelease();
	head->setGameObjectType(kCartType);
	headBody = this->createPartAtLocation(trunkBody->GetWorldPoint(b2Vec2(18.0/PTM_RATIO * SCREEN_SIZE_PX.height/640, 24.0/PTM_RATIO * SCREEN_SIZE_PX.height/640)), head);

	helm = new Box2DSprite();
	helm->initWithSpriteFrameName("OleCartHelmet.png");
	helm->autorelease();
	helm->setGameObjectType(kCartType);
	helmBody = this->createPartAtLocation(headBody->GetWorldPoint(b2Vec2(15.0/PTM_RATIO * SCREEN_SIZE_PX.height/640, 25.0/PTM_RATIO * SCREEN_SIZE_PX.height/640)), helm);

	arm = new Box2DSprite();
	arm->initWithSpriteFrameName("OleCartArm.png");
	arm->autorelease();
	arm->setGameObjectType(kCartType);
	armBody = this->createPartAtLocation(trunkBody->GetWorldPoint(b2Vec2(5.0/PTM_RATIO * SCREEN_SIZE_PX.height/640, -15.0/PTM_RATIO * SCREEN_SIZE_PX.height/640)), arm);

	b2Transform axisTransform;
	axisTransform.Set(b2Vec2(0, 0), body->GetAngle());
	b2Vec2 axis = b2Mul(axisTransform.q, b2Vec2(0,1));
		
	b2PrismaticJointDef prisJointDef;
	prisJointDef.Initialize(body, legsBody,
		legsBody->GetWorldCenter(), axis);
	prisJointDef.enableLimit = true;
	prisJointDef.lowerTranslation = 0.0;
	prisJointDef.upperTranslation = 43.0/PTM_RATIO * SCREEN_SIZE_PX.height/640;
	world->CreateJoint(&prisJointDef);

	b2RevoluteJointDef revJointDef;
	revJointDef.Initialize(legsBody, trunkBody,
		legsBody->GetWorldPoint(b2Vec2(0, 20.0/PTM_RATIO * SCREEN_SIZE_PX.height/640)));
	revJointDef.lowerAngle = CC_DEGREES_TO_RADIANS(-15);
	revJointDef.upperAngle = CC_DEGREES_TO_RADIANS(15);
	revJointDef.enableLimit = true;
	revJointDef.enableMotor = true;
	revJointDef.motorSpeed = 0.5;
	revJointDef.maxMotorTorque = 50.0;
	world->CreateJoint(&revJointDef);
	revJointDef.enableMotor = false;

	revJointDef.Initialize(trunkBody, armBody,
		armBody->GetWorldPoint(b2Vec2(-9.0/PTM_RATIO * SCREEN_SIZE_PX.height/640, 29.0/PTM_RATIO * SCREEN_SIZE_PX.height/640)));
	revJointDef.lowerAngle = CC_DEGREES_TO_RADIANS(-30);
	revJointDef.upperAngle = CC_DEGREES_TO_RADIANS(60);
	revJointDef.enableLimit = true;
	world->CreateJoint(&revJointDef);

	revJointDef.Initialize(trunkBody, headBody,
		headBody->GetWorldPoint(b2Vec2(-12.0/PTM_RATIO * SCREEN_SIZE_PX.height/640, -9.0/PTM_RATIO * SCREEN_SIZE_PX.height/640)));
	revJointDef.lowerAngle = CC_DEGREES_TO_RADIANS(-5);
	revJointDef.upperAngle = CC_DEGREES_TO_RADIANS(5);
	revJointDef.enableLimit = true;
	world->CreateJoint(&revJointDef);

	prisJointDef.Initialize(headBody, helmBody,
		helmBody->GetWorldCenter(), axis);
	prisJointDef.enableLimit = true;
	prisJointDef.lowerTranslation = 0.0/PTM_RATIO * SCREEN_SIZE_PX.height/640;
	prisJointDef.upperTranslation = 5.0/PTM_RATIO * SCREEN_SIZE_PX.height/640;
	world->CreateJoint(&prisJointDef);
}

void Cart::playHitEffect()
{
	int soundToPlay = rand()%4;
	if (soundToPlay == 0)
		PLAYSOUNDEFFECT(VIKING_HIT_1);
	else if (soundToPlay == 1)
		PLAYSOUNDEFFECT(VIKING_HIT_2);
	else if (soundToPlay == 2) 
		PLAYSOUNDEFFECT(VIKING_HIT_3);
	else
		PLAYSOUNDEFFECT(VIKING_HIT_4);
}

void Cart::changeState( CharacterStates newState )
{
	if (characterState == newState) 
		return;

	this->stopAllActions();
	this->setCharacterState(newState);

	switch (newState)
	{
	case kStateTakingDamage:
		{
			this->playHitEffect();
			characterHealth = characterHealth - 10;
			CCActionInterval *blink = CCBlink::actionWithDuration(1.0, 3.0);
			this->runAction(blink);
			blink = CCBlink::actionWithDuration(1.0, 3.0);
			wheelL->runAction(blink);
			blink = CCBlink::actionWithDuration(1.0, 3.0);
			wheelR->runAction(blink);
			blink = CCBlink::actionWithDuration(1.0, 3.0);
			legs->runAction(blink);
			blink = CCBlink::actionWithDuration(1.0, 3.0);
			trunk->runAction(blink);
			blink = CCBlink::actionWithDuration(1.0, 3.0);
			head->runAction(blink);
			blink = CCBlink::actionWithDuration(1.0, 3.0);
			helm->runAction(blink);
			blink = CCBlink::actionWithDuration(1.0, 3.0);
			arm->runAction(blink);
			break;
		}
	default:
		break;
	}
}

