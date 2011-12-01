#include "Health.h"

using namespace cocos2d;

Health::~Health()
{
	healthAnim->release();
}

void Health::changeState(CharacterStates newState)
{
	if (newState == kStateSpawning)
	{
		CCActionInterval *action = CCRepeatForever::actionWithAction(CCAnimate::actionWithAnimation(healthAnim, false));
		this->runAction(action);
	}

	else
	{
		this->setIsVisible(false);
		this->removeFromParentAndCleanup(true);
	}
}

void Health::updateStateWithDeltaTime(ccTime deltaTime, CCArray *listOfGameObjects)
{
	float groundHeight = this->getScreenSize().height * 0.065f;

	if (this->getPosition().y > groundHeight)
		this->setPosition(ccp(this->getPosition().x, this->getPosition().y - 5.0f));
}

void Health::initAnimations()
{
	this->setHealthAnim(this->loadPlistForAnimationWithName("healthAnim", "Plists/Health.plist"));
	healthAnim->retain();
}

bool Health::init()
{
	bool pRet = false;
	if (GameObject::init())
	{
		this->setScreenSize(CCDirector::sharedDirector()->getWinSize());
		this->setGameObjectType(kPowerUpTypeHealth);
		this->initAnimations();
		this->changeState(kStateSpawning);
		pRet = true;
	}
	return pRet;
}