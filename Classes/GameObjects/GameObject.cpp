#include "GameObject.h"

using namespace cocos2d;

bool GameObject::init()
{
	bool pRet = false;

	if(CCSprite::init())
	{
		CCLOG("GameObject init");
		screenSize = CCDirector::sharedDirector()->getWinSize();
		isActive = true;
		gameObjectType = kObjectTypeNone;
		//added to set scale
		this->setObjectScale();
		pRet = true;
	}

	return pRet;
}

void GameObject::changeState(CharacterStates newState)
{
	//CCLOG("GameObject->changeState method should be overridden");
}

void GameObject::updateStateWithDeltaTime(ccTime deltaTime, CCArray *listOfGameObjects) 
{
	//CCLOG("updateStateWithDeltaTime method should be overridden");
}

CCRect GameObject::adjustedBoundingBox()
{
	//CCLOG("GameObject adjustedBoundingBox should be overridden");
	return this->boundingBox();
}

// put in the name of the animation and the plist file - .plist
CCAnimation * GameObject::loadPlistForAnimationWithName(const char *animationName, const char *pszPlist)
{
	CCAnimation *animationToReturn = NULL;

	const char *plistPath = CCFileUtils::fullPathFromRelativePath(pszPlist);

	CCDictionary<std::string, CCObject*> *plistDictionary = CCFileUtils::dictionaryWithContentsOfFile(plistPath);

	if (plistDictionary == NULL)
	{
		CCLOG("Error reading plist: %s", pszPlist);
		return NULL;
	}

	//using a string to string dictionary to convert a CCString to float
	CCDictionary<std::string, CCString*> *animationSettings = (CCDictionary<std::string, CCString*> *) plistDictionary->objectForKey(std::string(animationName));
	if (animationSettings == NULL)
	{
		CCLOG("Could not locate AnimationWithName: %s", animationName);
		return NULL;
	}

	float animationDelay =  animationSettings->objectForKey(std::string("delay"))->toFloat();

	animationToReturn = CCAnimation::animation();
	animationToReturn->setDelay(animationDelay);
	
	string animationFramePrefix = animationSettings->objectForKey(std::string("filenamePrefix"))->toStdString();
	string animationFrames = animationSettings->objectForKey(std::string("animationFrames"))->toStdString();
	
	vector<string> animationFrameNumbers = split(animationFrames, ',');
	vector<string>::iterator it;
	string frameNumber = "0";

	for (it = animationFrameNumbers.begin(); it < animationFrameNumbers.end(); it++)
	{
		frameNumber = *it;
		char frameName[100] = {0};
		sprintf(frameName, "%s%s.png", animationFramePrefix.c_str(), frameNumber.c_str());
		animationToReturn->addFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName));
	}

	return animationToReturn;
}

// Scale down the object depending on different screen resolution
void GameObject::setObjectScale()
{
	if (SCREEN_HEIGHT == 480)
	{
		this->setScaleY(W480Scale);
		this->setScaleX(W480Scale);
	}

	else if (SCREEN_HEIGHT == 320)
	{
		this->setScaleY(W320Scale);
		this->setScaleX(W320Scale);
	}

	else if (SCREEN_HEIGHT == 720)
	{
		this->setScaleY(1.5);
		this->setScaleX(1.5);
	}
}