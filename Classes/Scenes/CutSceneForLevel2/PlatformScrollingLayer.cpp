#include "PlatformScrollingLayer.h"
#include "Singletons\GameManager.h"

using namespace cocos2d;
bool PlatformScrollingLayer::init()
{
	bool pRet = false;
	if (CCLayer::init())
	{
		srand(time(NULL));
		this->setIsTouchEnabled(true);
		hasBeenSkipped = false;
		this->createStaticBackground();
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ParallaxBackgrounds/ScrollingCloudsTextureAtlases/ScrollingCloudsTextureAtlasiPhone-hd.plist");
		scrollingBatchNode = CCSpriteBatchNode::batchNodeWithFile("ParallaxBackgrounds/ScrollingCloudsTextureAtlases/ScrollingCloudsTextureAtlasiPhone-hd.png");
		
		this->addChild(scrollingBatchNode);

		for (int x=0; x < 25; x++)
		{
			this->createCloud();
		}

		this->createVikingAndPlatform();

		pRet = true;
	}

	return pRet;
}

void PlatformScrollingLayer::createStaticBackground()
{
	CCSprite *background = CCSprite::spriteWithFile("ParallaxBackgrounds/ScrollingCloudsBackgrounds/tiles_grad_bkgrndiPhone-hd.png");

	background->setPosition(ccp(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));

	background->setScaleY(SCREEN_SIZE_PX.height/640);
	background->setScaleX(SCREEN_SIZE_PX.width/960);

	this->addChild(background);
}

void PlatformScrollingLayer::createCloud()
{
	int cloudToDraw = rand() % 6; //0 to 5
	char cloudFileName[100] = {0};
	sprintf(cloudFileName, "tiles_cloud%d.png", cloudToDraw);
	CCSprite *cloudSprite = CCSprite::spriteWithSpriteFrameName(cloudFileName);

	cloudSprite->setScaleY(SCREEN_SIZE_PX.height/640);
	cloudSprite->setScaleX(SCREEN_SIZE_PX.height/640);

	scrollingBatchNode->addChild(cloudSprite);
	this->resetCloudWithNode(cloudSprite);
}

void PlatformScrollingLayer::resetCloudWithNode(CCNode *node)
{
	CCNode *cloud = (CCNode*) node; 
	float xOffSet = cloud->boundingBox().size.width / 2; 
	int xPosition = SCREEN_WIDTH + 1 + xOffSet; 
	int yPosition = rand() % (int) SCREEN_HEIGHT; 
	cloud->setPosition(ccp(xPosition,yPosition)); 
	int moveDuration = rand() % kMaxCloudMoveDuration; 
	if (moveDuration < kMinCloudMoveDuration) 
		moveDuration = kMinCloudMoveDuration; 

	float offScreenXPosition = (xOffSet * -1) - 1; 

	CCFiniteTimeAction *moveAction = CCMoveTo::actionWithDuration(moveDuration, ccp(offScreenXPosition,cloud->getPosition().y));
	CCFiniteTimeAction *resetAction = CCCallFuncN::actionWithTarget(this, callfuncN_selector(PlatformScrollingLayer::resetCloudWithNode));
	CCFiniteTimeAction *sequenceAction = CCSequence::actions(moveAction,resetAction, NULL);

	cloud->runAction(sequenceAction); 

	int newZOrder = kMaxCloudMoveDuration - moveDuration;

	scrollingBatchNode->reorderChild(cloud, newZOrder);
}

void PlatformScrollingLayer::createVikingAndPlatform()
{
	int nextZValue = scrollingBatchNode->getChildren()->count() + 1;

	CCSprite *platform = CCSprite::spriteWithSpriteFrameName("platform.png");

	platform->setPosition(ccp(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.09f));

	platform->setScaleY(SCREEN_SIZE_PX.height/640);
	platform->setScaleX(SCREEN_SIZE_PX.height/640);

	scrollingBatchNode->addChild(platform, nextZValue);

	nextZValue = nextZValue + 1;

	CCSprite *viking = CCSprite::spriteWithSpriteFrameName("sv_anim_1.png");

	viking->setPosition(ccp(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.23f));

	viking->setScaleY(SCREEN_SIZE_PX.height/640);
	viking->setScaleX(SCREEN_SIZE_PX.height/640);

	scrollingBatchNode->addChild(viking, nextZValue);
}

void PlatformScrollingLayer::ccTouchesBegan(CCSet *touches, CCEvent *event)
{
	if (hasBeenSkipped == false)
	{
		// purge it first. otherwise an assertion error occurs
		CCSpriteFrameCache::purgeSharedSpriteFrameCache();
		GameManager::sharedGameManager()->runSceneWithID(kGameLevel2);
		hasBeenSkipped = true;
	}
}