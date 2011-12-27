#include "GameplayScrollingLayer.h"

void GameplayScrollingLayer::connectControlsWithJoystick(SneakyJoystick *leftJoystick, SneakyButton *jumpButton, SneakyButton *attackButton)
{
	Viking *viking = (Viking *) sceneSpriteBatchNode->getChildByTag(kVikingSpriteTagValue);
	viking->setJoystick(leftJoystick);
	viking->setAttackButton(attackButton);
	viking->setJumpButton(jumpButton);
}

void GameplayScrollingLayer::addScrollingBackground()
{
	CCSize levelSize = GameManager::sharedGameManager()->getDimensionsOfCurrentScene();
	CCSprite *scrollingBackground;
	scrollingBackground = CCSprite::spriteWithFile("ParallaxBackgrounds/FlatScrollingLayeriPhone-hd.png");
	scrollingBackground->setPosition(ccp(levelSize.width/2.0f, SCREEN_HEIGHT/2.0f));

	scrollingBackground->setScaleY(SCREEN_SIZE_PX.height/640);
	scrollingBackground->setScaleX(SCREEN_SIZE_PX.width/960);

	this->addChild(scrollingBackground);
}

bool GameplayScrollingLayer::init() 
{
	bool pRet = false;
	if (CCLayer::init()) 
	{
		srand(time(NULL));

		CCSize levelSize = GameManager::sharedGameManager()->getDimensionsOfCurrentScene();

		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("scene1atlasiPhone-hd.plist");
		sceneSpriteBatchNode = CCSpriteBatchNode::batchNodeWithFile("scene1atlasiPhone-hd.png");	
		
		this->addChild(sceneSpriteBatchNode, 20);
		Viking *viking = new Viking();
		viking->initWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("sv_anim_1.png"));

		viking->setJoystick(NULL);
		viking->setAttackButton(NULL);
		viking->setJumpButton(NULL);
		viking->setPosition(ccp(SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.14f));
		viking->setCharacterHealth(100);
		sceneSpriteBatchNode->addChild(viking, 1000, kVikingSpriteTagValue);
		viking->release();
		//this->addScrollingBackground();
		//this->addScrollingBackgroundWithParallax();
		//this->addScrollingBackgroundWithTileMap();
		this->addScrollingBackgroundWithTileMapInsideParallax();

		this->createObjectOfType(kEnemyTypeRadarDish, 100, ccp(SCREEN_WIDTH * 0.878f, SCREEN_HEIGHT * 0.13f), 10);
		this->createObjectOfType(kPowerUpTypeMallet, 0, ccp(levelSize.width * 0.2f, levelSize.height * 0.14f), 10);
		this->createObjectOfType(kPowerUpTypeHealth, 0, ccp(levelSize.width * 0.8f, levelSize.height * 0.14f), 10);

		this->schedule(schedule_selector(GameplayScrollingLayer::addEnemy), 6.0f);

		this->setIsKeypadEnabled(true);

		this->scheduleUpdate();
		pRet = true;
	}
	return pRet;
}

void GameplayScrollingLayer::adjustLayer()
{
	Viking *viking = (Viking *) sceneSpriteBatchNode->getChildByTag(kVikingSpriteTagValue);
	float vikingXPosition = viking->getPosition().x;
	float halfOfTheScreen = SCREEN_WIDTH/2.0f;
	CCSize levelSize = GameManager::sharedGameManager()->getDimensionsOfCurrentScene();

	if ((vikingXPosition > halfOfTheScreen) && (vikingXPosition < (levelSize.width - halfOfTheScreen))) 
	{
			// Background should scroll
			float newXPosition = halfOfTheScreen - vikingXPosition; 
			this->setPosition(ccp(newXPosition, this->getPosition().y)); 
	}
}

void GameplayScrollingLayer::update(ccTime deltaTime)
{
	CCArray *listOfGameObjects = sceneSpriteBatchNode->getChildren();
	CCObject *object = NULL;
	CCARRAY_FOREACH(listOfGameObjects, object)
	{
		GameObject *tempObj = (GameObject *) object; 
		tempObj->updateStateWithDeltaTime(deltaTime, listOfGameObjects);
	}

	this->adjustLayer();

	GameCharacter *tempChar = (GameCharacter*) sceneSpriteBatchNode->getChildByTag(kVikingSpriteTagValue);
	if ((tempChar->getCharacterState() == kStateDead) && (tempChar->numberOfRunningActions() == 0))
	{
		GameManager::sharedGameManager()->setHasPlayerDied(true);
		GameManager::sharedGameManager()->runSceneWithID(kLevelCompleteScene);
	}

	tempChar = (GameCharacter*) sceneSpriteBatchNode->getChildByTag(kRadarDishTagValue);
	if ((tempChar->getCharacterState() == kStateDead) && (tempChar->numberOfRunningActions() == 0))
		this->runAction(CCSequence::actions(CCCallFunc::actionWithTarget(this, callfunc_selector(GameplayScrollingLayer::displayLevelCleared)), CCDelayTime::actionWithDuration(0.6f), CCCallFunc::actionWithTarget(this, callfunc_selector(GameplayScrollingLayer::showLevelComplete)), NULL));
}

void GameplayScrollingLayer::addScrollingBackgroundWithParallax()
{
	CCSize levelSize = GameManager::sharedGameManager()->getDimensionsOfCurrentScene();

	CCSprite *BGLayer1;
	CCSprite *BGLayer2;
	CCSprite *BGLayer3;
	// IMPORTANT: instead of scaling the whole parallax node, scale individual layers to prevent backward scrolling on
	// higher resolutions
	BGLayer1 = CCSprite::spriteWithFile("ParallaxBackgrounds/chap9_scrolling4iPhone-hd.png");
	BGLayer1->setScaleY(SCREEN_SIZE_PX.height/640);
	BGLayer1->setScaleX(SCREEN_SIZE_PX.width/960);
	BGLayer2 = CCSprite::spriteWithFile("ParallaxBackgrounds/chap9_scrolling2iPhone-hd.png");
	BGLayer2->setScaleY(SCREEN_SIZE_PX.height/640);
	BGLayer2->setScaleX(SCREEN_SIZE_PX.width/960);
	BGLayer3 = CCSprite::spriteWithFile("ParallaxBackgrounds/chap9_scrolling3iPhone-hd.png");
	BGLayer3->setScaleY(SCREEN_SIZE_PX.height/640);
	BGLayer3->setScaleX(SCREEN_SIZE_PX.width/960);

	parallaxNode = CCParallaxNode::node();
	parallaxNode->setPosition(ccp(levelSize.width/2.0f, SCREEN_HEIGHT/2.0f));
	float xOffset = 0;

	parallaxNode->addChild(BGLayer1, 40, ccp(1.0f, 1.0f), ccp(0.0f, 0.0f));

	xOffset = (levelSize.width/2) * 0.3f;
	parallaxNode->addChild(BGLayer2, 20, ccp(0.2f, 1.0f), ccp(xOffset, 0));

	xOffset = (levelSize.width/2) * 0.8f;
	parallaxNode->addChild(BGLayer3, 30, ccp(0.7f, 1.0f), ccp(xOffset, 0));

	//parallaxNode->setScaleY(SCREEN_SIZE_PX.height/640);
	//parallaxNode->setScaleX(SCREEN_SIZE_PX.width/960);

	this->addChild(parallaxNode, 10);
}

void GameplayScrollingLayer::addScrollingBackgroundWithTileMap()
{

	tileMapNode = CCTMXTiledMap::tiledMapWithTMXFile("TileMaps/Level2TileMapiPhone-hd.tmx");
	
	tileMapNode->setScaleY(SCREEN_SIZE_PX.height/640);
	tileMapNode->setScaleX(SCREEN_SIZE_PX.width/960);

	this->addChild(tileMapNode);
}

void GameplayScrollingLayer::addScrollingBackgroundWithTileMapInsideParallax()
{
	CCSize levelSize = GameManager::sharedGameManager()->getDimensionsOfCurrentScene();

	tileMapNode = CCTMXTiledMap::tiledMapWithTMXFile("TileMaps/Level2TileMapiPhone-hd.tmx");

	CCTMXLayer *groundLayer = tileMapNode->layerNamed("GroundLayer");
	CCTMXLayer *rockColumnsLayer = tileMapNode->layerNamed("RockColumnsLayer");
	CCTMXLayer *rockBoulderLayer = tileMapNode->layerNamed("RockBoulderLayer");

	parallaxNode = CCParallaxNode::node();
	parallaxNode->setPosition(ccp(levelSize.width / 2, SCREEN_HEIGHT / 2));
	float xOffset = 0.0f;

	xOffset = levelSize.width / 2;
	groundLayer->retain();
	groundLayer->removeFromParentAndCleanup(false);
	groundLayer->setAnchorPoint(ccp(0.5f, 0.5f));
	parallaxNode->addChild(groundLayer, 30, ccp(1,1), ccp(xOffset,0));
	groundLayer->release();
	groundLayer->setScaleY(SCREEN_SIZE_PX.height/640);
	groundLayer->setScaleX(SCREEN_SIZE_PX.width/960);

	xOffset = (levelSize.width / 2) * 0.8f;
	rockColumnsLayer->retain();
	rockColumnsLayer->removeFromParentAndCleanup(false);
	rockColumnsLayer->setAnchorPoint(ccp(0.5f, 0.5f));
	parallaxNode->addChild(rockColumnsLayer, 20, ccp(0.2,1), ccp(xOffset,0));
	rockColumnsLayer->release();
	rockColumnsLayer->setScaleY(SCREEN_SIZE_PX.height/640);
	rockColumnsLayer->setScaleX(SCREEN_SIZE_PX.width/960);

	xOffset = (levelSize.width / 2) * 0.3f;
	rockBoulderLayer->retain();
	rockBoulderLayer->removeFromParentAndCleanup(false);
	rockBoulderLayer->setAnchorPoint(ccp(0.5f, 0.5f));
	parallaxNode->addChild(rockBoulderLayer, 30, ccp(0.7,1), ccp(xOffset,0));
	rockBoulderLayer->release();
	rockBoulderLayer->setScaleY(SCREEN_SIZE_PX.height/640);
	rockBoulderLayer->setScaleX(SCREEN_SIZE_PX.width/960);

	//parallaxNode->setScaleY(SCREEN_SIZE_PX.height/640);
	//parallaxNode->setScaleX(SCREEN_SIZE_PX.width/960);

	this->addChild(parallaxNode, 1);
}

void GameplayScrollingLayer::displayLevelCleared()
{

}

void GameplayScrollingLayer::showLevelComplete()
{
	// purge the frame cache to prevent assertion error when switching scenes
	CCSpriteFrameCache::purgeSharedSpriteFrameCache();
	GameManager::sharedGameManager()->runSceneWithID(kLevelCompleteScene);
}

void GameplayScrollingLayer::createObjectOfType(GameObjectType objectType, int initialHealth, CCPoint spawnLocation, int zValue)
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

void GameplayScrollingLayer::createPhaserWithDirection(PhaserDirection phaserDirection, CCPoint spawnPosition) 
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

void GameplayScrollingLayer::addEnemy(ccTime deltaTime) 
{
	CCSize levelSize = GameManager::sharedGameManager()->getDimensionsOfCurrentScene();

	int randNumber = rand() % 2;

	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
	RadarDish *radarDish = (RadarDish*) sceneSpriteBatchNode->getChildByTag(kRadarDishTagValue);
	if (radarDish != NULL) 
	{
		if (radarDish->getCharacterState() != kStateDead) 
		{
			if (randNumber == 0)
				this->createObjectOfType(kEnemyTypeAlienRobot, 100, ccp(levelSize.width * 0.195f, levelSize.height * 0.1432f), 2);
			else if (randNumber == 1)
				this->createObjectOfType(kEnemyTypeAlienRobot, 100, ccp(levelSize.width * 0.795f, levelSize.height * 0.1432f), 2);
		} 
		else 		
			this->unschedule(schedule_selector(GameplayScrollingLayer::addEnemy));	
	}
}

void GameplayScrollingLayer::keyBackClicked()
{
	CCSpriteFrameCache::purgeSharedSpriteFrameCache();
	GameManager::sharedGameManager()->setHasPlayerDied(false);
	GameManager::sharedGameManager()->runSceneWithID(kMainMenuScene);
}
