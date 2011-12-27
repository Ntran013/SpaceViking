#include "Scene4ActionLayer.h"

using namespace cocos2d;

void Scene4ActionLayer::setUpWorld()
{
	b2Vec2 gravity = b2Vec2(0.0f, -10.0f);
	bool doSleep = true;
	world = new b2World(gravity);
	world->SetAllowSleeping(doSleep);
}

void Scene4ActionLayer::setUpDebugDraw()
{
	debugDraw = new GLESDebugDraw(PTM_RATIO * CCDirector::sharedDirector()->getContentScaleFactor());
	world->SetDebugDraw(debugDraw);
	debugDraw->SetFlags(b2Draw::e_shapeBit);
}

void Scene4ActionLayer::createGround()
{
	float32 margin = 10.0f;
	b2Vec2 lowerLeft = b2Vec2(margin/PTM_RATIO, margin/PTM_RATIO);
	b2Vec2 lowerRight = b2Vec2((SCREEN_WIDTH - margin)/PTM_RATIO, margin/PTM_RATIO);
	b2Vec2 upperRight = b2Vec2((SCREEN_WIDTH - margin)/PTM_RATIO, (SCREEN_HEIGHT - margin)/PTM_RATIO);
	b2Vec2 upperLeft = b2Vec2(margin/PTM_RATIO, (SCREEN_HEIGHT - margin)/PTM_RATIO);

	b2BodyDef groundBodyDef;
	groundBodyDef.type = b2_staticBody;
	groundBodyDef.position.Set(0, 0);
	groundBody = world->CreateBody(&groundBodyDef);

	b2EdgeShape groundShape;
	b2FixtureDef groundFixtureDef;
	groundFixtureDef.shape = &groundShape;
	groundFixtureDef.density = 0.0;

	groundShape.Set(lowerLeft, lowerRight);
	groundBody->CreateFixture(&groundFixtureDef);
	groundShape.Set(lowerRight, upperRight);
	groundBody->CreateFixture(&groundFixtureDef);
	groundShape.Set(upperRight, upperLeft);
	groundBody->CreateFixture(&groundFixtureDef);
	groundShape.Set(upperLeft, lowerLeft);
	groundBody->CreateFixture(&groundFixtureDef);
}

void Scene4ActionLayer::createCartAtLocation(CCPoint location)
{
	cart = new Cart();
	cart->initWithWorld(world, location);
	cart->autorelease();
	sceneSpriteBatchNode->addChild(cart, 1, kVikingSpriteTagValue);
	sceneSpriteBatchNode->addChild(cart->getWheelL());
	sceneSpriteBatchNode->addChild(cart->getWheelR());
}

void Scene4ActionLayer::registerWithTouchDispatcher()
{
	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
}

bool Scene4ActionLayer::initWithScene4UILayer(Scene4UILayer *scene4UILayer)
{
	bool pRet = false;

	if (CCLayer::init())
	{
		uiLayer = scene4UILayer;

		this->setUpWorld();
		debugDraw = NULL;
		this->setUpDebugDraw();
		GameManager::sharedGameManager()->playBackgroundTrack(BACKGROUND_TRACK_MINECART);
		this->scheduleUpdate();
		this->createGround();
		this->setIsTouchEnabled(true);
		mouseJoint = NULL;

		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("scene4atlas-hd.plist");
		sceneSpriteBatchNode = CCSpriteBatchNode::batchNodeWithFile("scene4atlas-hd.png");
		this->addChild(sceneSpriteBatchNode, -1);

		this->createCartAtLocation(ccp(SCREEN_WIDTH/4, SCREEN_WIDTH*0.3));
		uiLayer->displayText("Go!", NULL, NULL);

		this->setIsKeypadEnabled(true);
		this->setIsAccelerometerEnabled(true);

		pRet = true;
	}

	return pRet;
}

void Scene4ActionLayer::update(ccTime dt)
{
	int32 velocityIterations = 3;
	int32 positionIterations = 2;

	world->Step(dt, velocityIterations, positionIterations);

	for (b2Body *b = world->GetBodyList(); b != NULL; b = b->GetNext())
	{
		if (b->GetUserData() != NULL)
		{
			Box2DSprite *sprite = (Box2DSprite *) b->GetUserData();
			sprite->setPosition(ccp(b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO));
			sprite->setRotation(CC_RADIANS_TO_DEGREES(b->GetAngle() *  -1));
		}
	}

	CCArray *listOfGameObjects = sceneSpriteBatchNode->getChildren();
	CCObject *object = NULL;
	CCARRAY_FOREACH(listOfGameObjects, object)
	{
		GameCharacter *tempChar = (GameCharacter *) object; 
		tempChar->updateStateWithDeltaTime(dt, listOfGameObjects);
	}
}

void Scene4ActionLayer::draw()
{
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if (world)
		world->DrawDebugData();

	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

bool Scene4ActionLayer::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
	CCPoint touchLocation = touch->locationInView(touch->view());
	touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
	touchLocation = this->convertToNodeSpace(touchLocation);
	b2Vec2 locationWorld = b2Vec2(touchLocation.x/PTM_RATIO, touchLocation.y/PTM_RATIO);

	b2AABB aabb;
	b2Vec2 delta = b2Vec2(1.0/PTM_RATIO, 1.0/PTM_RATIO);
	aabb.lowerBound = locationWorld - delta;
	aabb.upperBound = locationWorld + delta;
	SimpleQueryCallback callback(locationWorld);
	world->QueryAABB(&callback, aabb);

	if (callback.fixtureFound)
	{
		b2Body *body = callback.fixtureFound->GetBody();
		b2MouseJointDef mouseJointDef;
		mouseJointDef.bodyA = groundBody;
		mouseJointDef.bodyB = body;
		mouseJointDef.target = locationWorld;
		mouseJointDef.maxForce = 50 * body->GetMass();
		mouseJointDef.collideConnected = true;
		mouseJoint = (b2MouseJoint *) world->CreateJoint(&mouseJointDef);
		body->SetAwake(true);
		return true;
	}
	return true;
}

void Scene4ActionLayer::ccTouchMoved(CCTouch *touch, CCEvent *event)
{
	CCPoint touchLocation = touch->locationInView(touch->view());
	touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
	touchLocation = this->convertToNodeSpace(touchLocation);
	b2Vec2 locationWorld = b2Vec2(touchLocation.x/PTM_RATIO, touchLocation.y/PTM_RATIO);

	if (mouseJoint)
		mouseJoint->SetTarget(locationWorld);
}

void Scene4ActionLayer::ccTouchEnded(CCTouch *touch, CCEvent *event)
{
	if (mouseJoint)
	{
		world->DestroyJoint(mouseJoint);
		mouseJoint = NULL;
	}
}

void Scene4ActionLayer::keyBackClicked()
{
	GameManager::sharedGameManager()->setHasPlayerDied(false);
	GameManager::sharedGameManager()->runSceneWithID(kMainMenuScene);

}

Scene4ActionLayer::~Scene4ActionLayer()
{
	CC_SAFE_DELETE(world);
	CC_SAFE_DELETE(debugDraw);
}

void Scene4ActionLayer::accelerometer(CCAccelerometer *accelerometer, CCAcceleration *acceleration)
{
	float32 maxRevsPerSecond = 7.0;
	float32 accelerationFraction = acceleration->y*6;
	if (accelerationFraction < -1)
		accelerationFraction < -1;
	else if (accelerationFraction > 1)
		accelerationFraction = 1;

	float32 motorSpeed = (M_PI*2) * maxRevsPerSecond * accelerationFraction;
	cart->setMotorSpeed(motorSpeed);
}

