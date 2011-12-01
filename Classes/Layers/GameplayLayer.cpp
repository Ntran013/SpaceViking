#include "GameplayLayer.h"

using namespace cocos2d;

bool GameplayLayer::init()
{
	if(CCLayer::init())
	{
		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
		this->setIsTouchEnabled(true);

		srand(time(NULL));

		//vikingSprite = CCSprite::spriteWithFile("sv_anim_1.png");
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("scene1atlasiPhone.plist");
		sceneSpriteBatchNode = CCSpriteBatchNode::batchNodeWithFile("scene1atlasiPhone.png");

		this->addChild(sceneSpriteBatchNode, 0);
		this->initJoystickAndButtons();

		Viking *viking = new Viking();
		viking->initWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("sv_anim_1.png"));
		viking->setJoystick(lefJoystick);
		viking->setAttackButton(attackButton);
		viking->setJumpButton(jumpButton);
		viking->setPosition(ccp(screenSize.width * 0.35f, screenSize.height * 0.14f));
		viking->setCharacterHealth(100);
		sceneSpriteBatchNode->addChild(viking, kVikingSpriteZValue, kVikingSpriteTagValue);

		this->createObjectOfType(kEnemyTypeRadarDish, 100, ccp(screenSize.width * 0.878f, screenSize.height * 0.13f), 10);
		this->createObjectOfType(kEnemyTypeSpaceCargoShip, 100, ccp(screenSize.width * 0.5f, screenSize.height * 1.5f), 10);

		this->scheduleUpdate();
	}
	return true;
}

void GameplayLayer::initJoystickAndButtons()
{
	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
	CCRect joystickBaseDimensions = CCRectMake(0, 0, 128.0f, 128.0f); 
	CCRect jumpButtonDimensions = CCRectMake(0, 0, 64.0f, 64.0f); 
	CCRect attackButtonDimensions = CCRectMake(0, 0, 64.0f, 64.0f);

	CCPoint joystickBasePosition;
	CCPoint jumpButtonPosition;
	CCPoint attackButtonPosition;

	joystickBasePosition = ccp(screenSize.width*0.07f, screenSize.height*0.11f);
	jumpButtonPosition = ccp(screenSize.width*0.93f, screenSize.height*0.11f);
	attackButtonPosition = ccp(screenSize.width*0.93f, screenSize.height*0.35f);

	//Joystick
	SneakyJoystickSkinnedBase *joystickBase = SneakyJoystickSkinnedBase::node(); 
	joystickBase->setPosition(joystickBasePosition);
	joystickBase->setBackgroundSprite(CCSprite::spriteWithFile("dpadDown.png"));
	joystickBase->setThumbSprite(CCSprite::spriteWithFile("joystickDown.png"));
	SneakyJoystick *aJoystick = new SneakyJoystick();
	if (aJoystick && aJoystick->initWithRect(joystickBaseDimensions))
		aJoystick->autorelease();
	else
	{
		delete aJoystick; 
		aJoystick = NULL;		
	}
	joystickBase->setJoystick(aJoystick);
	joystickBase->getJoystick()->retain();
	lefJoystick = joystickBase->getJoystick();
	this->addChild(joystickBase);

	//Jump button
	SneakyButtonSkinnedBase *jumpButtonBase = SneakyButtonSkinnedBase::node(); 
	jumpButtonBase->setPosition(jumpButtonPosition); // 13
	jumpButtonBase->setDefaultSprite(CCSprite::spriteWithFile("jumpUp.png")); // 14
	jumpButtonBase->setActivatedSprite(CCSprite::spriteWithFile("jumpDown.png")); // 15
	jumpButtonBase->setPressSprite(CCSprite::spriteWithFile("jumpDown.png")); // 16
	SneakyButton *aButton = new SneakyButton();
	if (aButton && aButton->initWithRect(jumpButtonDimensions))
		aButton->autorelease();
	else
	{
		delete aButton; 
		aButton = NULL;		
	}
	jumpButtonBase->setButton(aButton); // 17
	jumpButtonBase->getButton()->retain(); // 18
	jumpButton = jumpButtonBase->getButton();
	jumpButton->setIsToggleable(false); // 19
	this->addChild(jumpButtonBase); // 20

	//Jump button
	SneakyButtonSkinnedBase *attackButtonBase = SneakyButtonSkinnedBase::node(); 
	attackButtonBase->setPosition(attackButtonPosition); // 13
	attackButtonBase->setDefaultSprite(CCSprite::spriteWithFile("handUp.png")); // 14
	attackButtonBase->setActivatedSprite(CCSprite::spriteWithFile("handDown.png")); // 15
	attackButtonBase->setPressSprite(CCSprite::spriteWithFile("handDown.png")); // 16
	SneakyButton *aButton1 = new SneakyButton();
	if (aButton1 && aButton1->initWithRect(attackButtonDimensions))
		aButton1->autorelease();
	else
	{
		delete aButton1; 
		aButton1 = NULL;		
	}
	attackButtonBase->setButton(aButton1); // 17
	attackButtonBase->getButton()->retain(); // 18
	attackButton = attackButtonBase->getButton();
	attackButton->setIsToggleable(false); // 19
	this->addChild(attackButtonBase); // 20

}

void GameplayLayer::update(ccTime deltaTime)
{
	CCArray *listOfGameObjects = sceneSpriteBatchNode->getChildren();
	CCObject *object = NULL;
	CCARRAY_FOREACH(listOfGameObjects, object)
	{
		GameCharacter *tempChar = (GameCharacter *) object; 
		tempChar->updateStateWithDeltaTime(deltaTime, listOfGameObjects);
	}
}

void GameplayLayer::createObjectOfType(GameObjectType objectType, int initialHealth, CCPoint spawnLocation, int zValue)
{
	if (objectType == kEnemyTypeRadarDish)
	{
		CCLOG("Creating the Radar Enemy");
		RadarDish *radarDish = new RadarDish();
		radarDish->initWithSpriteFrameName("radar_1.png");
		radarDish->setCharacterHealth(initialHealth);
		radarDish->setPosition(spawnLocation);
		sceneSpriteBatchNode->addChild(radarDish, zValue, kRadarDishTagValue);
		radarDish->release();
	}

	if (objectType == kPowerUpTypeMallet)
	{
		CCLOG("Creating a Mallet PowerUp");
		Mallet *mallet = new Mallet();
		mallet->initWithSpriteFrameName("mallet_1.png");
		mallet->setPosition(spawnLocation);
		sceneSpriteBatchNode->addChild(mallet);
		mallet->release();
	}

	if (objectType == kPowerUpTypeHealth)
	{
		CCLOG("Creating a Health PowerUp");
		Health *health = new Health();
		health->initWithSpriteFrameName("sandwich_1.png");
		health->setPosition(spawnLocation);
		sceneSpriteBatchNode->addChild(health);
		health->release();
	}

	if (objectType == kEnemyTypeSpaceCargoShip)
	{
		CCLOG("Creating the Cargo Ship Enemy");
		SpaceCargoShip *spaceCargoShip = new SpaceCargoShip();
		spaceCargoShip->setDelegate(this);
		spaceCargoShip->initWithSpriteFrameName("ship_2.png");
		spaceCargoShip->setPosition(spawnLocation);
		sceneSpriteBatchNode->addChild(spaceCargoShip, zValue);
		spaceCargoShip->release();
	}
}

void GameplayLayer::createPhaserWithDirection(PhaserDirection phaserDirection, CCPoint spawnPosition) 
{
	CCLOG("Placeholder for Chapter 5, see below");
	return;
}