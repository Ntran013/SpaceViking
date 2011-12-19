#include "GameControlLayer.h"

using namespace cocos2d;

void GameControlLayer::initJoystickAndButtons()
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
	aJoystick->autorelease();

	joystickBase->setJoystick(aJoystick);
	/* IMPORTANT: do not need to retain here like in the book, because the setJoystick method does retain as well
	   In the book the setJoystick method is not called, instead the joystick is set directly, which we can't do in cocos2d-x
	   Same thing applies for the buttons below */

	//joystickBase->getJoystick()->retain();
	leftJoystick = joystickBase->getJoystick();
	this->addChild(joystickBase);

	//Jump button
	SneakyButtonSkinnedBase *jumpButtonBase = SneakyButtonSkinnedBase::node(); 
	jumpButtonBase->setPosition(jumpButtonPosition); // 13
	jumpButtonBase->setDefaultSprite(CCSprite::spriteWithFile("jumpUp.png")); // 14
	jumpButtonBase->setActivatedSprite(CCSprite::spriteWithFile("jumpDown.png")); // 15
	jumpButtonBase->setPressSprite(CCSprite::spriteWithFile("jumpDown.png")); // 16
	SneakyButton *aButton = new SneakyButton();
	aButton->initWithRect(jumpButtonDimensions);
	aButton->autorelease();

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
	aButton1->initWithRect(attackButtonDimensions);
	aButton1->autorelease();

	attackButtonBase->setButton(aButton1); // 17
	//attackButtonBase->getButton()->retain(); // 18
	attackButton = attackButtonBase->getButton();
	attackButton->setIsToggleable(false); // 19
	this->addChild(attackButtonBase); // 20

}

bool GameControlLayer::init()
{
	bool pRet = false;
	if (CCLayer::init())
	{
		this->setIsTouchEnabled(true);
		this->initJoystickAndButtons();
		CCLOG("GameControlLayer initialized");
		pRet = true;
	}

	return pRet;
}