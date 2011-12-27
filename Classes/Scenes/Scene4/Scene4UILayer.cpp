#include "Scene4UILayer.h"

using namespace cocos2d;


bool Scene4UILayer::init()
{
	bool pRet = false;
	
	if (CCLayer::init())
	{
		label = CCLabelTTF::labelWithString("", "Helvetica", 48.0);
		label->setPosition(ccp(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
		label->setIsVisible(false);
		this->addChild(label);

		this->setIsTouchEnabled(true);
		this->initButtons();

		pRet = true;
	}

	return pRet;
}

bool Scene4UILayer::displayText(const char *text, SelectorProtocol *target, SEL_CallFuncN selector)
{
	label->stopAllActions();
	label->setString(text);
	label->setIsVisible(true);
	label->setScale(0.0);
	label->setOpacity(255);

	CCScaleTo *scaleUp = CCScaleTo::actionWithDuration(0.5, 1.2);
	CCScaleTo *scaleBack = CCScaleTo::actionWithDuration(0.1, 1.0);
	CCDelayTime *delay = CCDelayTime::actionWithDuration(2.0);
	CCFadeOut *fade = CCFadeOut::actionWithDuration(0.5);
	CCHide *hide = CCHide::action();
	CCCallFuncN *onComplete = CCCallFuncN::actionWithTarget(target, selector);
	CCFiniteTimeAction *sequence = CCSequence::actions(scaleUp, scaleBack, delay, fade, hide, onComplete, NULL);
	label->runAction(sequence);

	return true;
}

void Scene4UILayer::initButtons()
{
	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
	CCRect jumpButtonDimensions = CCRectMake(0, 0, 64.0f, 64.0f); 
	CCRect attackButtonDimensions = CCRectMake(0, 0, 64.0f, 64.0f);

	CCPoint jumpButtonPosition;
	CCPoint attackButtonPosition;

	jumpButtonPosition = ccp(screenSize.width*0.93f, screenSize.height*0.11f);
	// Set the attackButtonPosition to be 80 points above the jump button
	// So that their positions are fixed and do not change because of different screen resolutions
	attackButtonPosition = ccp(screenSize.width*0.93f, screenSize.height*0.11f + 80.0f);

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