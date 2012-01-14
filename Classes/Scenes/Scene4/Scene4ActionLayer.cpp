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
	b2BodyDef groundBodyDef;
	groundBodyDef.type = b2_staticBody;
	groundBodyDef.position.Set(0, 0);
	groundBody = world->CreateBody(&groundBodyDef);
	groundMaxX = 0;
}

void Scene4ActionLayer::createGroundEdgesWithVerts(b2Vec2 *verts, int num, const char *spriteFrameName)
{
	CCSprite *ground = CCSprite::spriteWithSpriteFrameName(spriteFrameName);
	ground->setScaleY(SCREEN_SIZE_PX.height/640);
	ground->setScaleX(SCREEN_SIZE_PX.height/640);
	ground->setPosition(ccp(groundMaxX + ground->boundingBox().size.width/2, ground->boundingBox().size.height/2));
	groundSpriteBatchNode->addChild(ground);

	b2EdgeShape groundShape;
	b2FixtureDef groundFixtureDef;
	groundFixtureDef.shape = &groundShape;
	groundFixtureDef.density = 0.0;

	for (int i = 0; i < num - 1; i++)
	{
		b2Vec2 offset = b2Vec2(groundMaxX/PTM_RATIO + ground->boundingBox().size.width/2/PTM_RATIO, ground->boundingBox().size.height/2/PTM_RATIO);
		b2Vec2 left = verts[i] + offset;
		b2Vec2 right = verts[i + 1] + offset;
		groundShape.Set(left, right);
		groundBody->CreateFixture(&groundFixtureDef);
	}

	groundMaxX += ground->boundingBox().size.width;
}

void Scene4ActionLayer::createCartAtLocation(CCPoint location)
{
	cart = new Cart();
	cart->initWithWorld(world, location);
	cart->autorelease();
	sceneSpriteBatchNode->addChild(cart, 1, kVikingSpriteTagValue);
	sceneSpriteBatchNode->addChild(cart->getWheelL());
	sceneSpriteBatchNode->addChild(cart->getWheelR());
	sceneSpriteBatchNode->addChild(cart->getLegs());
	sceneSpriteBatchNode->addChild(cart->getTrunk(), -1);
	sceneSpriteBatchNode->addChild(cart->getHead());
	sceneSpriteBatchNode->addChild(cart->getHelm());
	sceneSpriteBatchNode->addChild(cart->getArm(), 2);
}

void Scene4ActionLayer::registerWithTouchDispatcher()
{
	CCTouchDispatcher::sharedDispatcher()->addStandardDelegate(this, 0);
}

bool Scene4ActionLayer::initWithScene4UILayer(Scene4UILayer *scene4UILayer)
{
	bool pRet = false;

	if (CCLayer::init())
	{
		uiLayer = scene4UILayer;

		this->setUpWorld();
		debugDraw = NULL;
		//this->setUpDebugDraw();
		GameManager::sharedGameManager()->playBackgroundTrack(BACKGROUND_TRACK_MINECART);
		this->scheduleUpdate();
		this->createGround();
		this->setIsTouchEnabled(true);
		mouseJoint = NULL;
		lastBridgeStartJoint = NULL; 
		lastBridgeEndJoint = NULL; 
		isGameOver = false;

		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("scene4atlas-hd.plist");
		sceneSpriteBatchNode = CCSpriteBatchNode::batchNodeWithFile("scene4atlas-hd.png");
		this->addChild(sceneSpriteBatchNode, -1);

		this->createCartAtLocation(ccp(SCREEN_WIDTH/4, SCREEN_WIDTH*0.3));
		//uiLayer->displayText("Go!", NULL, NULL);

		char label[100] = {0};
		
		Cart* ct = (Cart*) sceneSpriteBatchNode->getChildByTag(kVikingSpriteTagValue);
		float fullM = ct->fullMass();
		sprintf(label, "%.2f", fullM);
		uiLayer->displayText(label, NULL, NULL);

		leftButton = uiLayer->getLeftButton();
		rightButton = uiLayer->getRightButton();

		this->setIsKeypadEnabled(true);
		this->setIsAccelerometerEnabled(true);

		CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGB5A1);
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("groundAtlas-hd.plist");
		groundSpriteBatchNode = CCSpriteBatchNode::batchNodeWithFile("groundAtlas-hd.png");

		CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_Default);
		this->addChild(groundSpriteBatchNode, -2);

		this->createLevel();
		srand(time(NULL));

		pRet = true;
	}

	return pRet;
}

void Scene4ActionLayer::update(ccTime dt)
{
	/*int32 velocityIterations = 3;
	int32 positionIterations = 2;

	world->Step(dt, velocityIterations, positionIterations);*/

	static double UPDATE_INTERVAL = 1.0f/60.0f;
	static double MAX_CYCLES_PER_FRAME = 5;
	static double timeAccumulator = 0;

	timeAccumulator += dt;
	if (timeAccumulator > (MAX_CYCLES_PER_FRAME * UPDATE_INTERVAL)) 
		timeAccumulator = UPDATE_INTERVAL;

	int32 velocityIterations = 3;
	int32 positionIterations = 2;
	while (timeAccumulator >= UPDATE_INTERVAL) 
	{
		timeAccumulator -= UPDATE_INTERVAL;
		world->Step(UPDATE_INTERVAL,
			velocityIterations, positionIterations);
	}

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

	if (leftButton->getIsActive())
	{
		cart->setMotorSpeed((M_PI*2) * 7.0 * 1);
		b2Vec2 impulse = b2Vec2(-1 * 1 * cart->fullMass() * 3.5, 0);
		cart->getBody()->ApplyLinearImpulse(impulse, cart->getBody()->GetWorldCenter());
	}
	else if (rightButton->getIsActive())
	{
		cart->setMotorSpeed((M_PI*2) * 7.0 * -1);
		b2Vec2 impulse = b2Vec2(-1 * -1 * cart->fullMass() * 3.5, 0);
		cart->getBody()->ApplyLinearImpulse(impulse, cart->getBody()->GetWorldCenter());
	}

	this->followCart();

	if (!isGameOver) 
	{
		if (isBodyCollidingWithObjectType(offscreenSensorBody, kCartType)) 
		{
			isGameOver = true;
			uiLayer->displayText("You Lose", this, callfuncN_selector(Scene4ActionLayer::gameOver));
		} 
		else if (isBodyCollidingWithObjectType(offscreenSensorBody,	kDiggerType)) 
		{
			isGameOver = true;
			uiLayer->displayText("You Win!", this, callfuncN_selector(Scene4ActionLayer::gameOver));
		}
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

void Scene4ActionLayer::ccTouchesBegan(CCSet *touches, CCEvent *event)
{
	CCTouch *touch = (CCTouch *) touches->anyObject();
	if (touch)
	{
		if (isBodyCollidingWithObjectType(cart->getBody(), kGroundType) || isBodyCollidingWithObjectType(groundBody, kCartType)) 
		{
			cart->jump();
		}
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

void Scene4ActionLayer::didAccelerate(CCAcceleration *acceleration)
{
	float32 maxRevsPerSecond = 7.0;
	float32 accelerationFraction = acceleration->y*6;
	if (accelerationFraction < -1)
		accelerationFraction = -1;
	else if (accelerationFraction > 1)
		accelerationFraction = 1;

	float32 motorSpeed = (M_PI*2) * maxRevsPerSecond * accelerationFraction;
	cart->setMotorSpeed(motorSpeed);

	if (abs(cart->getBody()->GetLinearVelocity().x) < 5.0) 
	{
	b2Vec2 impulse = b2Vec2(-1 * acceleration->y * cart->fullMass() * 2, 0);
	cart->getBody()->ApplyLinearImpulse(impulse, cart->getBody()->GetWorldCenter());
	}
}

void Scene4ActionLayer::createGround1()
{
	int num = 23;
	b2Vec2 verts[] = {
		b2Vec2(-1022.5f / PTM_RATIO * SCREEN_SIZE_PX.height/640, -20.2f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(-966.6f / PTM_RATIO * SCREEN_SIZE_PX.height/640, -18.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(-893.8f / PTM_RATIO * SCREEN_SIZE_PX.height/640, -10.3f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(-888.8f / PTM_RATIO * SCREEN_SIZE_PX.height/640, 1.1f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(-804.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640, 10.3f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(-799.7f / PTM_RATIO * SCREEN_SIZE_PX.height/640, 5.3f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(-795.5f / PTM_RATIO * SCREEN_SIZE_PX.height/640, 8.1f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(-755.2f / PTM_RATIO * SCREEN_SIZE_PX.height/640, -1.8f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(-755.2f / PTM_RATIO * SCREEN_SIZE_PX.height/640, -9.5f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(-632.2f / PTM_RATIO * SCREEN_SIZE_PX.height/640, 5.3f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(-603.9f / PTM_RATIO * SCREEN_SIZE_PX.height/640, 17.3f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(-536.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640, 18.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(-518.3f / PTM_RATIO * SCREEN_SIZE_PX.height/640, 28.6f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(-282.1f / PTM_RATIO * SCREEN_SIZE_PX.height/640, 13.1f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(-258.1f / PTM_RATIO * SCREEN_SIZE_PX.height/640, 27.2f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(-135.1f / PTM_RATIO * SCREEN_SIZE_PX.height/640, 18.7f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(9.2f / PTM_RATIO * SCREEN_SIZE_PX.height/640, -19.4f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(483.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640, -18.7f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(578.4f / PTM_RATIO * SCREEN_SIZE_PX.height/640, 11.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(733.3f / PTM_RATIO * SCREEN_SIZE_PX.height/640, -7.4f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(827.3f / PTM_RATIO * SCREEN_SIZE_PX.height/640, -1.1f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(1006.9f / PTM_RATIO * SCREEN_SIZE_PX.height/640, -20.2f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(1023.2f / PTM_RATIO * SCREEN_SIZE_PX.height/640, -20.2f / PTM_RATIO * SCREEN_SIZE_PX.height/640)
	};
	this->createGroundEdgesWithVerts(verts, num, "ground1.png");
}

void Scene4ActionLayer::createGround2()
{
	int num = 24;
	b2Vec2 verts[] = {
		b2Vec2(-1022.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640, -20.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(-963.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640, -23.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(-902.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640, -4.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(-762.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640, -7.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(-674.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640, 26.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(-435.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640, 22.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(-258.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640, -1.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(-242.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640, 19.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(-170.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640, 43.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(-58.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640, 45.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(98.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640, -20.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(472.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640, -20.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(471.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640, -7.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(503.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640, 4.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(614.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640, 66.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(679.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640, 59.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(681.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640, 46.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(735.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640, 31.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(822.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640, 24.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(827.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640, 12.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(934.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640, 14.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(975.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640, 1.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(982.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640, -19.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(1023.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640, -20.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640)
	};
	this->createGroundEdgesWithVerts(verts, num, "ground2.png");
}

void Scene4ActionLayer::createGround3()
{
	int num = 2;
	b2Vec2 verts[] = {
		b2Vec2(-1021.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640, -22.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640),
		b2Vec2(1021.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640, -20.0f / PTM_RATIO * SCREEN_SIZE_PX.height/640)
	};
	this->createGroundEdgesWithVerts(verts, num, "ground3.png");
}

void Scene4ActionLayer::createBackground()
{
	CCParallaxNode *parallax = CCParallaxNode::node();
	CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGB565);
	CCSprite *background;
	background = CCSprite::spriteWithFile("scene_4_background-hd.png");
	background->setScaleY(SCREEN_SIZE_PX.height/640);
	background->setScaleX(SCREEN_SIZE_PX.width/960);
	background->setAnchorPoint(ccp(0,0));
	CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_Default);
	parallax->addChild(background, -10, ccp(0.05f, 0.05f), ccp(0,0));
	this->addChild(parallax, -10);
}

void Scene4ActionLayer::createBridge()
{
	Box2DSprite *lastObject;
	b2Body *lastBody = groundBody;
	for(int i = 0; i < 15; i++) 
	{
		Box2DSprite *plank = new Box2DSprite();
		plank->initWithSpriteFrameName("plank.png");
		plank->autorelease();
		plank->setGameObjectType(kGroundType);

		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position =b2Vec2(groundMaxX/PTM_RATIO + plank->boundingBox().size.width/2/PTM_RATIO, 80.0/PTM_RATIO * SCREEN_SIZE_PX.height/640 - (plank->boundingBox().size.height/2/PTM_RATIO));
		b2Body *plankBody = world->CreateBody(&bodyDef);
		plankBody->SetUserData(plank);
		plank->setBody(plankBody);
		groundSpriteBatchNode->addChild(plank);

		b2PolygonShape shape;
		float32 diff;

		diff = ((SCREEN_SIZE_PX.height * 20)/320) - plank->boundingBox().size.height;

		shape.SetAsBox(
			plank->boundingBox().size.width/2/PTM_RATIO, 40.0/PTM_RATIO * SCREEN_SIZE_PX.height/640,
			b2Vec2(0, -plank->boundingBox().size.height/2/PTM_RATIO-diff/PTM_RATIO), 0);
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		fixtureDef.density = 2.0;
		plankBody->CreateFixture(&fixtureDef);
		b2RevoluteJointDef jd;
		jd.Initialize(lastBody, plankBody,
			plankBody->GetWorldPoint(b2Vec2(-plank->boundingBox().size.width/2/PTM_RATIO * SCREEN_SIZE_PX.height/640, 0)));
		jd.lowerAngle = CC_DEGREES_TO_RADIANS(-0.25);
		jd.upperAngle = CC_DEGREES_TO_RADIANS(0.25);
		jd.enableLimit = true;
		b2Joint *joint = world->CreateJoint(&jd);
		if (i == 0) { lastBridgeStartJoint = joint; }
		groundMaxX += (plank->boundingBox().size.width * 0.8);
		lastBody = plankBody;
		lastObject = plank;
	}

	b2RevoluteJointDef jd;
	jd.Initialize(lastBody, groundBody,
		lastBody->GetWorldPoint(
		b2Vec2(lastObject->boundingBox().size.width/2/PTM_RATIO * SCREEN_SIZE_PX.height/640, 0)));
	lastBridgeEndJoint = world->CreateJoint(&jd);
}

void Scene4ActionLayer::createLevel()
{
	this->createBackground();
	this->createGround3();
	this->createSpikesWithOffset(-(1200 * SCREEN_SIZE_PX.height)/320);
	this->createSpikesWithOffset(-(400 * SCREEN_SIZE_PX.height)/320);
	this->createBridge();
	this->createGround1();
	this->createSpikesWithOffset(-(1050 * SCREEN_SIZE_PX.height)/320);
	this->createSpikesWithOffset(-(100 * SCREEN_SIZE_PX.height)/320);
	this->createBridge();
	this->createGround3();
	this->createSpikesWithOffset(-(1700 * SCREEN_SIZE_PX.height)/320);
	this->createSpikesWithOffset(-(900 * SCREEN_SIZE_PX.height)/320);
	this->createBridge();
	this->createGround2();
	this->createSpikesWithOffset(-(1300 * SCREEN_SIZE_PX.height)/320);
	this->createSpikesWithOffset(-(900 * SCREEN_SIZE_PX.height)/320);
	this->createGround3();
	this->createSpikesWithOffset(-(1200 * SCREEN_SIZE_PX.height)/320);
	this->createSpikesWithOffset(-(400 * SCREEN_SIZE_PX.height)/320);
	this->createBridge();
	this->createGround3();
	this->createDigger();
	this->createGround3();
	this->createOffscreenSensorBody();
}

void Scene4ActionLayer::followCart()
{
	float fixedPosition = SCREEN_WIDTH/4;
	float newX = fixedPosition - cart->getPosition().x;
	newX = min(newX, fixedPosition);
	newX = max(newX, -groundMaxX-fixedPosition);
	CCPoint newPos = ccp(newX, this->getPosition().y);
	this->setPosition(newPos);
}

void Scene4ActionLayer::createSpikesWithOffset(int offset)
{
	Spikes *spikes;
	spikes = new Spikes();
	spikes->initWithWorld(world, ccp(groundMaxX + offset/2, (SCREEN_SIZE_PX.height*100)/640));
	spikes->autorelease();

	sceneSpriteBatchNode->addChild(spikes);
}

void Scene4ActionLayer::createDigger()
{
	digger = new Digger();
	digger->initWithWorld(world, ccp(groundMaxX - SCREEN_WIDTH * 0.8, SCREEN_HEIGHT/2));
	digger->autorelease();

	sceneSpriteBatchNode->addChild(digger);
	sceneSpriteBatchNode->addChild(digger->getWheelLSprite(), -1);
	sceneSpriteBatchNode->addChild(digger->getWheelRSprite(), -1);
}

void Scene4ActionLayer::createOffscreenSensorBody()
{
	float32 sensorWidth = groundMaxX + SCREEN_WIDTH*4;
	float32 sensorHeight = SCREEN_HEIGHT * 0.25;
	float32 sensorOffsetX = -SCREEN_WIDTH*2;
	float32 sensorOffsetY = -SCREEN_HEIGHT/2;
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(
		sensorOffsetX/PTM_RATIO + sensorWidth/2/PTM_RATIO,
		sensorOffsetY/PTM_RATIO + sensorHeight/2/PTM_RATIO);
	offscreenSensorBody = world->CreateBody(&bodyDef);
	b2PolygonShape shape;
	shape.SetAsBox(sensorWidth/2/PTM_RATIO, sensorHeight/2/PTM_RATIO);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.isSensor = true;
	fixtureDef.density = 0.0;
	offscreenSensorBody->CreateFixture(&fixtureDef);
}

void Scene4ActionLayer::gameOver( cocos2d::CCNode *pSender )
{
	GameManager::sharedGameManager()->runSceneWithID(kMainMenuScene);
}

