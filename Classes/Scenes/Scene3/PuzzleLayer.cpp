#include "PuzzleLayer.h"

using namespace cocos2d;


CCScene * PuzzleLayer::scene()
{
	CCScene *scene = CCScene::node();
	PuzzleLayer *layer = PuzzleLayer::node();
	scene->addChild(layer);

	return scene;
}

bool PuzzleLayer::init()
{
	bool pRet = false;
	
	if(CCLayer::init())
	{
		this->setUpWorld();
		debugDraw = NULL;
		//this->setUpDebugDraw();
		this->scheduleUpdate();
		this->createGround();
		//set mouseJoint = NULL for NULL checking in touch methods
		mouseJoint = NULL;
		this->createSensor();
		this->instructions();
		hasWon = false;

		this->setIsTouchEnabled(true);
		this->setIsKeypadEnabled(true);

		// Random drop
		srand(time(NULL));
		//this->schedule(schedule_selector(PuzzleLayer::createBoxAtRandomLocation), 2.0f);

		//this->setIsAccelerometerEnabled(true);

		/*
		CCPoint location1, location2, location3;
		CCSize smallSize, medSize, largeSize;

		location1 = ccp(100, 200);
		location2 = ccp(250, 200);
		location3 = ccp(400, 200);
		smallSize = CCSizeMake(25, 25);
		medSize = CCSizeMake(50, 50);
		largeSize = CCSizeMake(100, 100);

		this->createBoxAtLocation(location1, medSize, 1.0, 0.0, 1.0);
		this->createBoxAtLocation(location1, medSize, 1.0, 0.5, 1.0);
		this->createBoxAtLocation(location1, medSize, 1.0, 1.0, 1.0); */

		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("scene3atlas-hd.plist");
		sceneSpriteBatchNode = CCSpriteBatchNode::batchNodeWithFile("scene3atlas-hd.png");

		this->addChild(sceneSpriteBatchNode, 0);
		this->createObjectAtLocation(kMeteorType, ccp(SCREEN_WIDTH/4.8,SCREEN_HEIGHT/1.07));
		this->createObjectAtLocation(kSkullType, ccp(SCREEN_WIDTH/4.8,SCREEN_HEIGHT/1.28));
		this->createObjectAtLocation(kRockType, ccp(SCREEN_WIDTH/2.4,SCREEN_HEIGHT/6.4));
		this->createObjectAtLocation(kIceType, ccp(SCREEN_WIDTH/2.4,SCREEN_HEIGHT/1.6));
		this->createObjectAtLocation(kLongBlockType, ccp(SCREEN_WIDTH/2.4,SCREEN_HEIGHT/2.1333));
		this->createObjectAtLocation(kFrozenVikingType, ccp(SCREEN_WIDTH/12,SCREEN_HEIGHT/1.6));
		this->createObjectAtLocation(kRockType, ccp(SCREEN_WIDTH/3.2,SCREEN_HEIGHT/1.6));

		CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGB565);

		CCSprite *background = CCSprite::spriteWithFile("puzzle_level_bkgrnd-hd.png");
		background->setAnchorPoint(ccp(0, 0));
		background->setScaleX(SCREEN_SIZE_PX.width/960);
		background->setScaleY(SCREEN_SIZE_PX.height/640);
		CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_Default);
		this->addChild(background, -1);

		for (int i = 0; i < 10; i++)
		{
			this->createObjectAtLocation(kSkullType, ccp(SCREEN_WIDTH/4.8,SCREEN_HEIGHT/1.28));
		}

		for (int i = 0; i < 2; i++)
		{
			this->createObjectAtLocation(kMeteorType, ccp(SCREEN_WIDTH/4.8,SCREEN_HEIGHT/1.28));
			this->createObjectAtLocation(kRockType, ccp(SCREEN_WIDTH/4.8,SCREEN_HEIGHT/1.28));
			this->createObjectAtLocation(kIceType, ccp(SCREEN_WIDTH/4.8,SCREEN_HEIGHT/1.28));
		}

		GameManager::sharedGameManager()->playBackgroundTrack(BACKGROUND_TRACK_PUZZLE);

		pRet = true;
	}

	return pRet;
}

void PuzzleLayer::setUpWorld()
{
	b2Vec2 gravity = b2Vec2(0.0f, -10.0f);
	bool doSleep = true;
	world = new b2World(gravity);
	world->SetAllowSleeping(doSleep);
}

PuzzleLayer::~PuzzleLayer()
{
	CC_SAFE_DELETE(world);
	CC_SAFE_DELETE(debugDraw);
}

void PuzzleLayer::createBoxAtLocation(CCPoint location, CCSize size, float32 friction, float32 restitution, float32 density)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);
	bodyDef.allowSleep = false;
	b2Body *body = world->CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox(size.width/2/PTM_RATIO, size.height/2/PTM_RATIO);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;

	body->CreateFixture(&fixtureDef);
}

void PuzzleLayer::createBoxAtRandomLocation(ccTime dt)
{
	float xLocation = rand() % (int) SCREEN_WIDTH;
	float yLocation = rand() % (int) SCREEN_HEIGHT;

	if (yLocation < SCREEN_HEIGHT/2)
		yLocation = yLocation + SCREEN_HEIGHT/2;

	CCSize size = CCSizeMake(50, 50);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = b2Vec2(xLocation/PTM_RATIO, yLocation/PTM_RATIO);
	b2Body *body = world->CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox(size.width/2/PTM_RATIO, size.height/2/PTM_RATIO);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0;
	body->CreateFixture(&fixtureDef);
}

void PuzzleLayer::createBodyAtLocation(CCPoint location, Box2DSprite *sprite, float32 friction, float32 restitution, float32 density, bool isBox)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);
	bodyDef.allowSleep = false;
	b2Body *body = world->CreateBody(&bodyDef);
	body->SetUserData(sprite);
	sprite->setBody(body);

	b2FixtureDef fixtureDef;

	if (isBox)
	{
		b2PolygonShape shape;
		shape.SetAsBox(sprite->boundingBox().size.width/2/PTM_RATIO, sprite->boundingBox().size.height/2/PTM_RATIO);
		fixtureDef.shape = &shape;

		fixtureDef.density = density;
		fixtureDef.friction = friction;
		fixtureDef.restitution = restitution;

		body->CreateFixture(&fixtureDef);
	}

	else
	{
		b2CircleShape shape;
		shape.m_radius = sprite->boundingBox().size.width/2/PTM_RATIO;
		fixtureDef.shape = &shape;

		fixtureDef.density = density;
		fixtureDef.friction = friction;
		fixtureDef.restitution = restitution;

		body->CreateFixture(&fixtureDef);
	}
}

void PuzzleLayer::setUpDebugDraw()
{
	debugDraw = new GLESDebugDraw(PTM_RATIO * CCDirector::sharedDirector()->getContentScaleFactor());
	world->SetDebugDraw(debugDraw);
	debugDraw->SetFlags(b2Draw::e_shapeBit);
}

void PuzzleLayer::draw()
{
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	world->DrawDebugData();
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void PuzzleLayer::registerWithTouchDispatcher()
{
	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
}

void PuzzleLayer::update( cocos2d::ccTime dt )
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

	if (!hasWon)
	{
		b2ContactEdge *edge = frozenVikingBody->GetContactList();
		while (edge)
		{
			b2Contact *contact = edge->contact;
			b2Fixture *fixtureA = contact->GetFixtureA();
			b2Fixture *fixtureB = contact->GetFixtureB();
			b2Body *bodyA = fixtureA->GetBody();
			b2Body *bodyB = fixtureB->GetBody();

			if (bodyA == sensorBody	|| bodyB == sensorBody)
			{
				hasWon = true;
				this->win();
				break;
			}

			edge = edge->next;
		}
	}
}

bool PuzzleLayer::ccTouchBegan(CCTouch *touch, CCEvent *event)
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

		Box2DSprite *sprite = (Box2DSprite *) body->GetUserData();
		if (sprite == NULL)
			return false;
		if (!sprite->mouseJointBegan())
			return false;

		b2MouseJointDef mouseJointDef;
		mouseJointDef.bodyA = groundBody;
		mouseJointDef.bodyB = body;
		mouseJointDef.target = locationWorld;
		mouseJointDef.maxForce = 100 * body->GetMass();
		mouseJointDef.collideConnected = true;

		mouseJoint = (b2MouseJoint *) world->CreateJoint(&mouseJointDef);
		body->SetAwake(true);
		return true;
	}

	else 
		//this->createBoxAtLocation(touchLocation, CCSizeMake(50, 50), 0.0, 0.0, 0.0);

	return true;
}

void PuzzleLayer::ccTouchMoved( cocos2d::CCTouch *touch, cocos2d::CCEvent *event )
{
	CCPoint touchLocation = touch->locationInView(touch->view());
	touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
	touchLocation = this->convertToNodeSpace(touchLocation);
	b2Vec2 locationWorld = b2Vec2(touchLocation.x/PTM_RATIO, touchLocation.y/PTM_RATIO);

	if (mouseJoint)
		mouseJoint->SetTarget(locationWorld);
}

void PuzzleLayer::ccTouchEnded( cocos2d::CCTouch *touch, cocos2d::CCEvent *event )
{
	if (mouseJoint)
	{
		world->DestroyJoint(mouseJoint);
		mouseJoint = NULL;
	}
}

void PuzzleLayer::createGround()
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

void PuzzleLayer::keyBackClicked()
{
	GameManager::sharedGameManager()->setHasPlayerDied(false);
	GameManager::sharedGameManager()->runSceneWithID(kMainMenuScene);
}

void PuzzleLayer::accelerometer(CCAccelerometer *accelerometer, CCAcceleration * acceleration)
{
	b2Vec2 gravity(-acceleration->y * 15, acceleration->x *15);
	world->SetGravity(gravity);
}

void PuzzleLayer::createObjectAtLocation(GameObjectType type, cocos2d::CCPoint location)
{
	switch (type)
	{
	case kMeteorType:
		{
			Meteor *sprite = new Meteor();
			sprite->init();
			sprite->autorelease();
			this->createBodyAtLocation(location, sprite, 0.1, 0.3, 1.0, false);
			sceneSpriteBatchNode->addChild(sprite);
			break;
		}
	case kSkullType:
		{
			Skull *sprite = new Skull();
			sprite->init();
			sprite->autorelease();
			this->createBodyAtLocation(location, sprite, 0.5, 0.5, 0.25, false);
			sceneSpriteBatchNode->addChild(sprite);
			break;
		}
	case kLongBlockType:
		{
			LongBlock *sprite = new LongBlock();
			sprite->init();
			sprite->autorelease();
			this->createBodyAtLocation(location, sprite, 0.2, 0.0, 1.0, true);
			sceneSpriteBatchNode->addChild(sprite);
			break;
		}
	case kIceType:
		{
			IceBlock *sprite = new IceBlock();
			sprite->init();
			sprite->autorelease();
			this->createBodyAtLocation(location, sprite, 0.2, 0.2, 1.0, true);
			sceneSpriteBatchNode->addChild(sprite);
			break;
		}
	case kFrozenVikingType:
		{
			FrozenOle *sprite = new FrozenOle();
			sprite->init();
			sprite->autorelease();
			this->createBodyAtLocation(location, sprite, 0.1, 0.2, 1.0, true);
			sceneSpriteBatchNode->addChild(sprite);
			frozenVikingBody = sprite->getBody();
			break;
		}
	case kRockType:
		{
			Rock *sprite = new Rock();
			sprite->init();
			sprite->autorelease();
			this->createBodyAtLocation(location, sprite, 3.0, 0.0, 1.0, true);
			sceneSpriteBatchNode->addChild(sprite);
			break;
		}
	default:
		{
			CCLOG("Unknown Type");
			break;
		}
	}
}

void PuzzleLayer::createSensor()
{
	CCSize sensorSize = CCSizeMake(SCREEN_WIDTH/9.6, SCREEN_WIDTH/12.8);

	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position =b2Vec2((SCREEN_WIDTH - sensorSize.width/2)/PTM_RATIO, sensorSize.height/2/PTM_RATIO);
	sensorBody = world->CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox(sensorSize.width/PTM_RATIO, sensorSize.height/PTM_RATIO);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.isSensor = true;
	sensorBody->CreateFixture(&fixtureDef);
}

void PuzzleLayer::instructions()
{
	CCLabelTTF *label = CCLabelTTF::labelWithString("Melt the Viking!" ,"Helvetica", 48.0);
	label->setPosition(ccp(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
	label->setScale(0.25);
	this->addChild(label);

	CCScaleTo *scaleUp = CCScaleTo::actionWithDuration(1.0, 1.2);
	CCScaleTo *scaleBack = CCScaleTo::actionWithDuration(1.0, 1.0);
	CCDelayTime *delay = CCDelayTime::actionWithDuration(5.0);
	CCFadeOut *fade = CCFadeOut::actionWithDuration(2.0);
	CCFiniteTimeAction *sequence = CCSequence::actions(scaleUp, scaleBack, delay, fade, NULL);
	label->runAction(sequence);
}

void PuzzleLayer::winComplete(CCNode *pSender)
{
	GameManager::sharedGameManager()->setHasPlayerDied(false);
	GameManager::sharedGameManager()->runSceneWithID(kLevelCompleteScene);
}

void PuzzleLayer::win()
{
	CCLabelTTF *label = CCLabelTTF::labelWithString("You Win!" ,"Helvetica", 48.0);
	label->setPosition(ccp(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
	label->setScale(0.25);
	this->addChild(label);

	CCScaleTo *scaleUp = CCScaleTo::actionWithDuration(1.0, 1.2);
	CCScaleTo *scaleBack = CCScaleTo::actionWithDuration(1.0, 1.0);
	CCDelayTime *delay = CCDelayTime::actionWithDuration(5.0);
	CCCallFuncN *winComplete = CCCallFuncN::actionWithTarget(this, callfuncN_selector(PuzzleLayer::winComplete));
	CCFiniteTimeAction *sequence = CCSequence::actions(scaleUp, scaleBack, delay, winComplete, NULL);
	label->runAction(sequence);
}
