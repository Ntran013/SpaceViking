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

		//this->addScrollingBackground();
		//this->addScrollingBackgroundWithParallax();
		//this->addScrollingBackgroundWithTileMap();
		this->addScrollingBackgroundWithTileMapInsideParallax();

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
	/*
	tempChar = (GameCharacter*) sceneSpriteBatchNode->getChildByTag(kRadarDishTagValue);
	if ((tempChar->getCharacterState() == kStateDead) && (tempChar->numberOfRunningActions() == 0))
		GameManager::sharedGameManager()->runSceneWithID(kLevelCompleteScene);
	*/
}

void GameplayScrollingLayer::addScrollingBackgroundWithParallax()
{
	CCSize levelSize = GameManager::sharedGameManager()->getDimensionsOfCurrentScene();

	CCSprite *BGLayer1;
	CCSprite *BGLayer2;
	CCSprite *BGLayer3;

	BGLayer1 = CCSprite::spriteWithFile("ParallaxBackgrounds/chap9_scrolling4iPhone-hd.png");
	BGLayer2 = CCSprite::spriteWithFile("ParallaxBackgrounds/chap9_scrolling2iPhone-hd.png");
	BGLayer3 = CCSprite::spriteWithFile("ParallaxBackgrounds/chap9_scrolling3iPhone-hd.png");

	parallaxNode = CCParallaxNode::node();
	parallaxNode->setPosition(ccp(levelSize.width/2.0f, SCREEN_HEIGHT/2.0f));
	float xOffset = 0;

	parallaxNode->addChild(BGLayer1, 40, ccp(1.0f, 1.0f), ccp(0.0f, 0.0f));

	xOffset = (levelSize.width/2) * 0.3f;
	parallaxNode->addChild(BGLayer2, 20, ccp(0.2f, 1.0f), ccp(xOffset, 0));

	xOffset = (levelSize.width/2) * 0.8f;
	parallaxNode->addChild(BGLayer3, 30, ccp(0.7f, 1.0f), ccp(xOffset, 0));

	parallaxNode->setScaleY(SCREEN_SIZE_PX.height/640);
	parallaxNode->setScaleX(SCREEN_SIZE_PX.width/960);

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

	xOffset = (levelSize.width / 2) * 0.8f;
	rockColumnsLayer->retain();
	rockColumnsLayer->removeFromParentAndCleanup(false);
	rockColumnsLayer->setAnchorPoint(ccp(0.5f, 0.5f));
	parallaxNode->addChild(rockColumnsLayer, 20, ccp(0.2,1), ccp(xOffset,0));
	rockColumnsLayer->release();

	xOffset = (levelSize.width / 2) * 0.3f;
	rockBoulderLayer->retain();
	rockBoulderLayer->removeFromParentAndCleanup(false);
	rockBoulderLayer->setAnchorPoint(ccp(0.5f, 0.5f));
	parallaxNode->addChild(rockBoulderLayer, 30, ccp(0.7,1), ccp(xOffset,0));
	rockBoulderLayer->release();

	parallaxNode->setScaleY(SCREEN_SIZE_PX.height/640);
	parallaxNode->setScaleX(SCREEN_SIZE_PX.width/960);

	this->addChild(parallaxNode, 1);
}