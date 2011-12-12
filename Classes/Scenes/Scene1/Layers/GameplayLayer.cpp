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
		CCLOG("Loading Assets");
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("scene1atlasiPhone-hd.plist");
		sceneSpriteBatchNode = CCSpriteBatchNode::batchNodeWithFile("scene1atlasiPhone-hd.png");
		CCLOG("Loading Done");
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
		// IMPORTANT: release after initXXX(), release() here doesn't mean that the viking is deallocated yet, this simply decreases the reference by 1
		// delete viking; will cause a crash
		// the same thing applies for other objects in the createObjectOfType method
		viking->release();

		this->createObjectOfType(kEnemyTypeRadarDish, 100, ccp(screenSize.width * 0.878f, screenSize.height * 0.13f), 10);
		
		//CCLabelTTF *gameBeginLabel = CCLabelTTF::labelWithString("Game Start", "Helvetica", 64);
		gameBeginLabel = CCLabelBMFont::labelWithString("Game Start", "Fonts/SpaceVikingFont.fnt");
		gameBeginLabel->setPosition(ccp(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));

		this->addChild(gameBeginLabel);
		CCFiniteTimeAction *labelAction = CCSpawn::actions(CCScaleBy::actionWithDuration(0.6f, 2), CCFadeOut::actionWithDuration(0.6f), NULL);
		gameBeginLabel->runAction(labelAction);

		this->scheduleUpdate();

		this->schedule(schedule_selector(GameplayLayer::addEnemy), 10.0f);
		this->createObjectOfType(kEnemyTypeSpaceCargoShip, 0, ccp(screenSize.width * -0.5f, screenSize.height * 0.74f), 50);

		GameManager::sharedGameManager()->playBackgroundTrack(BACKGROUND_TRACK_OLE_AWAKES);
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
	// Set the attackButtonPosition to be 80 points above the jump button
	// So that their positions are fixed and do not change because of different screen resolutions
	attackButtonPosition = ccp(screenSize.width*0.93f, screenSize.height*0.11f + 80.0f);

	//Joystick
	SneakyJoystickSkinnedBase *joystickBase = SneakyJoystickSkinnedBase::node(); 
	joystickBase->setPosition(joystickBasePosition);
	joystickBase->setBackgroundSprite(CCSprite::spriteWithFile("dpadDown.png"));
	joystickBase->setThumbSprite(CCSprite::spriteWithFile("joystickDown.png"));
	SneakyJoystick *aJoystick = new SneakyJoystick();
	aJoystick->initWithRect(joystickBaseDimensions);

	joystickBase->setJoystick(aJoystick);
	/* IMPORTANT: do not need to retain here like in the book, because the setJoystick method does retain as well
	   In the book the setJoystick method is not called, instead the joystick is set directly, which we can't do in cocos2d-x
	   Same thing applies for the buttons below */

	//joystickBase->getJoystick()->retain();
	lefJoystick = joystickBase->getJoystick();
	this->addChild(joystickBase);

	//Jump button
	SneakyButtonSkinnedBase *jumpButtonBase = SneakyButtonSkinnedBase::node(); 
	jumpButtonBase->setPosition(jumpButtonPosition); // 13
	jumpButtonBase->setDefaultSprite(CCSprite::spriteWithFile("jumpUp.png")); // 14
	jumpButtonBase->setActivatedSprite(CCSprite::spriteWithFile("jumpDown.png")); // 15
	jumpButtonBase->setPressSprite(CCSprite::spriteWithFile("jumpDown.png")); // 16
	SneakyButton *aButton = new SneakyButton();
	aButton->initWithRect(jumpButtonDimensions);

	jumpButtonBase->setButton(aButton); // 17
	//jumpButtonBase->getButton()->retain(); // 18
	jumpButton = jumpButtonBase->getButton();
	jumpButton->setIsToggleable(false); // 19
	this->addChild(jumpButtonBase); // 20

	//Attack button
	SneakyButtonSkinnedBase *attackButtonBase = SneakyButtonSkinnedBase::node(); 
	attackButtonBase->setPosition(attackButtonPosition); // 13
	attackButtonBase->setDefaultSprite(CCSprite::spriteWithFile("handUp.png")); // 14
	attackButtonBase->setActivatedSprite(CCSprite::spriteWithFile("handDown.png")); // 15
	attackButtonBase->setPressSprite(CCSprite::spriteWithFile("handDown.png")); // 16
	SneakyButton *aButton1 = new SneakyButton();
	aButton1 && aButton1->initWithRect(attackButtonDimensions);

	attackButtonBase->setButton(aButton1); // 17
	//attackButtonBase->getButton()->retain(); // 18
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

	GameCharacter *tempChar = (GameCharacter*) sceneSpriteBatchNode->getChildByTag(kVikingSpriteTagValue);
	if ((tempChar->getCharacterState() == kStateDead) && (tempChar->numberOfRunningActions() == 0))
	{
		GameManager::sharedGameManager()->setHasPlayerDied(true);
		GameManager::sharedGameManager()->runSceneWithID(kLevelCompleteScene);
	}

	tempChar = (GameCharacter*) sceneSpriteBatchNode->getChildByTag(kRadarDishTagValue);
	if ((tempChar->getCharacterState() == kStateDead) && (tempChar->numberOfRunningActions() == 0) && gameBeginLabel->numberOfRunningActions() == 0)
		this->runAction(CCSequence::actions(CCCallFunc::actionWithTarget(this, callfunc_selector(GameplayLayer::displayLevelCleared)), CCDelayTime::actionWithDuration(0.6f), CCCallFunc::actionWithTarget(this, callfunc_selector(GameplayLayer::showLevelComplete)), NULL));
}

void GameplayLayer::displayLevelCleared()
{
	gameBeginLabel = CCLabelBMFont::labelWithString("Level Cleared", "Fonts/SpaceVikingFont.fnt");
	gameBeginLabel->setPosition(ccp(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));

	this->addChild(gameBeginLabel);
	CCFiniteTimeAction *labelAction = CCSpawn::actions(CCScaleBy::actionWithDuration(0.6f, 2), CCFadeOut::actionWithDuration(0.6f), NULL);
	if (labelAction != NULL)
		gameBeginLabel->runAction(labelAction);		
}

void GameplayLayer::showLevelComplete()
{
	// purge the frame cache to prevent assertion error when switching scenes
	CCSpriteFrameCache::purgeSharedSpriteFrameCache();
	GameManager::sharedGameManager()->runSceneWithID(kLevelCompleteScene);
}

void GameplayLayer::createObjectOfType(GameObjectType objectType, int initialHealth, CCPoint spawnLocation, int zValue)
{
	if (objectType == kEnemyTypeRadarDish)
	{
		//CCLOG("Creating the Radar Enemy");
		RadarDish *radarDish = new RadarDish();
		radarDish->initWithSpriteFrameName("radar_1.png");
		radarDish->setCharacterHealth(initialHealth);
		radarDish->setPosition(spawnLocation);
		sceneSpriteBatchNode->addChild(radarDish, zValue, kRadarDishTagValue);
		radarDish->release();
	}
	else if (objectType == kEnemyTypeAlienRobot)
	{
		//CCLOG("Creating the Alien Robot");
		EnemyRobot *enemyRobot = new EnemyRobot();
		enemyRobot->setDelegate(this);
		enemyRobot->initWithSpriteFrameName("an1_anim1.png");
		enemyRobot->setCharacterHealth(initialHealth);
		enemyRobot->setPosition(spawnLocation);
		enemyRobot->changeState(kStateSpawning);
		sceneSpriteBatchNode->addChild(enemyRobot, zValue);
	#if ENEMY_STATE_DEBUG == 1
		CCLabelBMFont *debugLabel = CCLabelBMFont::labelWithString("NoneNone", "Fonts/SpaceVikingFont.fnt");
		this->addChild(debugLabel);
		enemyRobot->setMyDebugLabel(debugLabel);
	#endif
		enemyRobot->release();
	}

	else if (objectType == kPowerUpTypeMallet)
	{
		//CCLOG("Creating a Mallet PowerUp");
		Mallet *mallet = new Mallet();
		mallet->initWithSpriteFrameName("mallet_1.png");
		mallet->setPosition(spawnLocation);
		sceneSpriteBatchNode->addChild(mallet);
		mallet->release();
	}

	else if (objectType == kPowerUpTypeHealth)
	{
		//CCLOG("Creating a Health PowerUp");
		Health *health = new Health();
		health->initWithSpriteFrameName("sandwich_1.png");
		health->setPosition(spawnLocation);
		sceneSpriteBatchNode->addChild(health);
		health->release();
	}

	else if (objectType == kEnemyTypeSpaceCargoShip)
	{
		//CCLOG("Creating the Cargo Ship Enemy");
		SpaceCargoShip *spaceCargoShip = new SpaceCargoShip();
		// setDelegate before initing SpaceCargoShip, as the dropCargo function inside init() uses the delegate
		spaceCargoShip->setDelegate(this);
		spaceCargoShip->initWithSpriteFrameName("ship_2.png");
		spaceCargoShip->setPosition(spawnLocation);
		sceneSpriteBatchNode->addChild(spaceCargoShip, zValue);
		spaceCargoShip->release();
	}
}

void GameplayLayer::createPhaserWithDirection(PhaserDirection phaserDirection, CCPoint spawnPosition) 
{
	PhaserBullet *phaserBullet = new PhaserBullet(); 
	phaserBullet->initWithSpriteFrameName("beam_1.png");
	phaserBullet->setPosition(spawnPosition);
	phaserBullet->setMyDirection(phaserDirection);
	//Commented out as changeState() is called in the init function, in order to see the spawning animation
	//phaserBullet->setCharacterState(kStateSpawning);
	sceneSpriteBatchNode->addChild(phaserBullet);
	phaserBullet->release();
}

void GameplayLayer::addEnemy(ccTime deltaTime) 
{
	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
	RadarDish *radarDish = (RadarDish*) sceneSpriteBatchNode->getChildByTag(kRadarDishTagValue);
	if (radarDish != NULL) 
	{
		if (radarDish->getCharacterState() != kStateDead) 
		{
			this->createObjectOfType(kEnemyTypeAlienRobot, 100, ccp(screenSize.width * 0.195f, screenSize.height * 0.1432f), 2);
		} 
		else 		
			this->unschedule(schedule_selector(GameplayLayer::addEnemy));	
	}
}
