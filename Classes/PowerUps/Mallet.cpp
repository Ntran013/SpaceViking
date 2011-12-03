#include "Mallet.h"

using namespace cocos2d;

Mallet::~Mallet()
{
	if (malletAnim)
	{
		malletAnim->release();
		malletAnim = NULL;
	}
}

void Mallet::changeState(CharacterStates newState)
{
	if (newState == kStateSpawning)
	{
		CCActionInterval *action = CCRepeatForever::actionWithAction(CCAnimate::actionWithAnimation(malletAnim, false));
		this->runAction(action);
	}

	else
	{
		this->setIsVisible(false);
		this->removeFromParentAndCleanup(true);
	}
}

void Mallet::updateStateWithDeltaTime(ccTime deltaTime, CCArray *listOfGameObjects)
{
	float groundHeight = this->getScreenSize().height * 0.065f;

	if (this->getPosition().y > groundHeight)
		this->setPosition(ccp(this->getPosition().x, this->getPosition().y - 5.0f));
}

void Mallet::initAnimations()
{
	this->setMalletAnim(this->loadPlistForAnimationWithName("malletAnim", "Plists/Mallet.plist"));
	malletAnim->retain();
}

bool Mallet::init()
{
	bool pRet = false;
	if (GameObject::init())
	{
		this->setGameObjectType(kPowerUpTypeMallet);
		this->initAnimations();
		this->changeState(kStateSpawning);
		pRet = true;
	}
	return pRet;
}