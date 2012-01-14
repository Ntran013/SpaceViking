#include "Digger.h"

using namespace cocos2d;


void Digger::createBodyWithWorld(b2World *world, CCPoint location)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);
	b2Body *cartBody = world->CreateBody(&bodyDef);
	cartBody->SetUserData(this);
	this->setBody(cartBody);
	b2PolygonShape shape;
	int num = 4;
	b2Vec2 verts[] = {
		b2Vec2(87.0f / PTM_RATIO * SCREEN_HEIGHT_PX/640, -32.0f / PTM_RATIO * SCREEN_HEIGHT_PX/640),
		b2Vec2(81.0f / PTM_RATIO * SCREEN_HEIGHT_PX/640, 110.0f / PTM_RATIO * SCREEN_HEIGHT_PX/640),
		b2Vec2(-87.0f / PTM_RATIO * SCREEN_HEIGHT_PX/640, 112.0f / PTM_RATIO * SCREEN_HEIGHT_PX/640),
		b2Vec2(-84.0f / PTM_RATIO * SCREEN_HEIGHT_PX/640, -33.0f / PTM_RATIO * SCREEN_HEIGHT_PX/640)
	};
	shape.Set(verts, num);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 0.5;
	fixtureDef.friction = 0.5;
	fixtureDef.restitution = 0.5;
	cartBody->CreateFixture(&fixtureDef);
	cartBody->SetAngularDamping(1000);
}

void Digger::createWheelsWithWorld(b2World *world)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = body->GetWorldPoint(b2Vec2(-50.0/PTM_RATIO * SCREEN_HEIGHT_PX/640, -80.0/PTM_RATIO * SCREEN_HEIGHT_PX/640));
	wheelLBody = world->CreateBody(&bodyDef);
	bodyDef.position = body->GetWorldPoint(b2Vec2(50.0/PTM_RATIO * SCREEN_HEIGHT_PX/640, -80.0/PTM_RATIO * SCREEN_HEIGHT_PX/640));
	wheelRBody = world->CreateBody(&bodyDef);
	b2CircleShape circleShape;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.friction = 0.2;
	fixtureDef.restitution = 0.5;
	fixtureDef.density = 5.0;
	circleShape.m_radius = 25.0/PTM_RATIO * SCREEN_HEIGHT_PX/640;
	wheelLBody->CreateFixture(&fixtureDef);
	circleShape.m_radius = 25.0/PTM_RATIO * SCREEN_HEIGHT_PX/640;
	wheelRBody->CreateFixture(&fixtureDef);
	b2RevoluteJointDef revJointDef;
	revJointDef.Initialize(body, wheelLBody,
		wheelLBody->GetWorldCenter());
	revJointDef.enableMotor = true;
	revJointDef.motorSpeed = 0;
	revJointDef.maxMotorTorque = 1000;
	wheelLJoint = (b2RevoluteJoint *) world->CreateJoint(&revJointDef);
	revJointDef.Initialize(body, wheelRBody,
		wheelRBody->GetWorldCenter());
	wheelRJoint = (b2RevoluteJoint *) world->CreateJoint(&revJointDef);
	wheelLSprite = new Box2DSprite();
	wheelLSprite->initWithSpriteFrameName("Wheel.png");
	wheelLSprite->autorelease();
	wheelLSprite->setBody(wheelLBody);
	wheelLBody->SetUserData(wheelLSprite);

	wheelRSprite = new Box2DSprite();
	wheelRSprite->initWithSpriteFrameName("Wheel.png");
	wheelRSprite->autorelease();
	wheelRSprite->setBody(wheelRBody);
	wheelRBody->SetUserData(wheelRSprite);
}

void Digger::createDrillWithWorld( b2World *world )
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = body->GetPosition();
	drillLBody = world->CreateBody(&bodyDef);
	drillRBody = world->CreateBody(&bodyDef);
	b2PolygonShape shape;
	int num = 3;
	b2Vec2 verts[] = {
		b2Vec2(-65.0f / PTM_RATIO * SCREEN_HEIGHT_PX/640, 31.0f / PTM_RATIO * SCREEN_HEIGHT_PX/640),
		b2Vec2(-189.0f / PTM_RATIO * SCREEN_HEIGHT_PX/640, -2.0f / PTM_RATIO * SCREEN_HEIGHT_PX/640),
		b2Vec2(-85.0f / PTM_RATIO * SCREEN_HEIGHT_PX/640, -72.0f / PTM_RATIO * SCREEN_HEIGHT_PX/640)
	};
	shape.Set(verts, num);
	b2FixtureDef fixtureDef;
	fixtureDef.density = 0.25;

	fixtureDef.shape = &shape;
	fixtureDef.isSensor = true;
	drillLFixture = drillLBody->CreateFixture(&fixtureDef);
	int num2 = 3;
	b2Vec2 verts2[] = {
		b2Vec2(85.0f / PTM_RATIO * SCREEN_HEIGHT_PX/640, -72.0f / PTM_RATIO * SCREEN_HEIGHT_PX/640),
		b2Vec2(189.0f / PTM_RATIO * SCREEN_HEIGHT_PX/640, -2.0f / PTM_RATIO * SCREEN_HEIGHT_PX/640),
		b2Vec2(65.0f / PTM_RATIO * SCREEN_HEIGHT_PX/640, 31.0f / PTM_RATIO * SCREEN_HEIGHT_PX/640),
	};
	shape.Set(verts2, num2);
	drillRFixture = drillRBody->CreateFixture(&fixtureDef);
	b2WeldJointDef weldJointDef;
	weldJointDef.Initialize(body, drillLBody, body->GetWorldCenter());
	world->CreateJoint(&weldJointDef);
	weldJointDef.Initialize(body, drillRBody, body->GetWorldCenter());
	world->CreateJoint(&weldJointDef);
}

bool Digger::initWithWorld(b2World *theWorld, CCPoint location)
{
	bool pRet = false;

	if (Box2DSprite::init())
	{
		this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("digger_anim5.png"));
		gameObjectType = kDiggerType;
		characterHealth = 100.0f;
		this->createBodyWithWorld(theWorld, location);
		this->createWheelsWithWorld(theWorld);
		this->createDrillWithWorld(theWorld);

		this->initAnimations();
		pRet = true;
	}

	return pRet;
}

void Digger::updateStateWithDeltaTime( cocos2d::ccTime deltaTime, cocos2d::CCArray *listOfGameObjects )
{
	if (characterState == kStateTakingDamage && this->numberOfRunningActions() > 0)
		return;

	if (characterState == kStateDrilling && this->numberOfRunningActions() == 0)
		this->changeState(kStateRotating);

	if (characterState == kStateTakingDamage && this->numberOfRunningActions() == 0)
	{
		wheelLJoint->SetMotorSpeed(0);
		wheelRJoint->SetMotorSpeed(0);
		this->changeState(kStateRotating);
	}

	if (characterState != kStateWalking && this->numberOfRunningActions() == 0)
		this->changeState(kStateWalking);

	if (characterState == kStateWalking)
	{
		Cart *cart = (Cart *) this->getParent()->getChildByTag(kVikingSpriteTagValue);
		b2Body *cartBody = cart->getBody();

		cc_timeval curTime; 
		CCTime::gettimeofdayCocos2d(&curTime, NULL);
		cc_timeval timeMoving;
		CCTime::timersubCocos2d(&timeMoving, &movingStartTime, &curTime);
		static double TIME_TO_MOVE = 2.0f;

		b2Body *drill = drillLBody;
		float direction = -1.0;
		if (this->isFlipX())
		{
			drill = drillRBody;
			direction = -1 * direction;
		}

		if (isBodyCollidingWithObjectType(drill, kCartType)) 
		{
			//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("drill.caf");
			cart->changeState(kStateTakingDamage);
			this->changeState(kStateDrilling);
			wheelLJoint->SetMotorSpeed(0);
			wheelRJoint->SetMotorSpeed(0);
			cartBody->ApplyLinearImpulse(
				b2Vec2(direction * cart->fullMass() * 13, -1.0 * cart->fullMass()),
				cartBody->GetWorldPoint(b2Vec2(0, -15.0/PTM_RATIO * SCREEN_HEIGHT_PX/640)));
		}

		else if (isBodyCollidingWithObjectType(cartBody, kDiggerType)) 
		{
			//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("collision.caf");
			this->changeState(kStateTakingDamage);
			cartBody->ApplyLinearImpulse(
				b2Vec2(-direction * cart->fullMass() * 8, -1.0 * cart->fullMass()),
				cartBody->GetWorldPoint(b2Vec2(0, -15.0/PTM_RATIO * SCREEN_HEIGHT_PX/640)));
			body->ApplyLinearImpulse(
				b2Vec2(direction * body->GetMass() * 8, 0),
				body->GetWorldPoint(b2Vec2(0, -5.0/PTM_RATIO * SCREEN_HEIGHT_PX/640)));
		}

		else if (timeMoving.tv_sec > TIME_TO_MOVE) 
		{
			wheelLJoint->SetMotorSpeed(0);
			wheelRJoint->SetMotorSpeed(0);
			this->changeState(kStateRotating);
		}
		
		else 
		{
			wheelLJoint->SetMotorSpeed(-1 * direction * M_PI * 3);
			wheelRJoint->SetMotorSpeed(-1 * direction * M_PI * 3);
		}
	}
}

void Digger::disableDrills()
{
	drillLFixture->SetSensor(true);
	drillRFixture->SetSensor(true);
}

void Digger::enableDrills()
{
	if (this->isFlipX()) {
		drillRFixture->SetSensor(false);
	} else {
		drillLFixture->SetSensor(false);
	}
}

void Digger::changeState(CharacterStates newState)
{
	if (characterState == newState)
		return;

	this->stopAllActions();
	CCFiniteTimeAction *action = NULL;
	this->setCharacterState(newState);

	switch (newState)
	{
	case kStateTakingDamage:
		{
			action = CCBlink::actionWithDuration(1.0, 3.0);
			break;
		}
	case kStateDrilling:
		{
			action = CCRepeat::actionWithAction(CCAnimate::actionWithAnimation(drillAnim, true), 3);
			break;
		}
	case kStateWalking:
		CCTime::gettimeofdayCocos2d(&movingStartTime, NULL);
		break;
	case kStateRotating:
		{
			CCCallFunc *disableDrills = CCCallFunc::actionWithTarget(this, callfunc_selector(Digger::disableDrills));
			CCAnimate *rotToCenter = CCAnimate::actionWithAnimation(rotateAnim, false);
			CCFlipX *flip = CCFlipX::actionWithFlipX(!this->isFlipX());
			CCAnimate *rotToSide = (CCAnimate *) rotToCenter->reverse();
			CCCallFunc *enableDrills = CCCallFunc::actionWithTarget(this, callfunc_selector(Digger::enableDrills));
			action = CCSequence::actions(disableDrills, rotToCenter, flip, rotToSide, enableDrills, NULL);
			break;
		}
	default:
		break;
	}

	if (action != NULL)
		this->runAction(action);
}

void Digger::initAnimations()
{
	rotateAnim = this->loadPlistForAnimationWithName("rotateAnim", "Plists/Digger.plist");
	rotateAnim->retain();
	drillAnim = this->loadPlistForAnimationWithName("drillAnim", "Plists/Digger.plist");
	drillAnim->retain();
}

Digger::~Digger()
{
	CC_SAFE_RELEASE_NULL(rotateAnim);
	CC_SAFE_RELEASE_NULL(drillAnim);
}
