#include "GameplayLayer.h"

using namespace cocos2d;

bool GameplayLayer::init()
{
	if(CCLayer::init())
	{
		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
		this->setIsTouchEnabled(true);

		vikingSprite = CCSprite::spriteWithFile("sv_anim_1.png");
		vikingSprite->setPosition(ccp(screenSize.width/2, screenSize.height*0.17f));

		this->addChild(vikingSprite);

		vikingSprite->setScaleX(screenSize.width/1024.0f);
		vikingSprite->setScaleY(screenSize.height/768.0f);

		this->initJoystickAndButtons();
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

