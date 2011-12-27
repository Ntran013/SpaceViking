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
	CCRect leftButtonDimensions = CCRectMake(0, 0, 64.0f, 64.0f); 
	CCRect attackButtonDimensions = CCRectMake(0, 0, 64.0f, 64.0f);

	CCPoint leftButtonPosition;
	CCPoint rightButtonPosition;

	leftButtonPosition = ccp(screenSize.width*0.07f, screenSize.height/2);
	// Set the attackButtonPosition to be 80 points above the jump button
	// So that their positions are fixed and do not change because of different screen resolutions
	rightButtonPosition = ccp(screenSize.width*0.93f, screenSize.height/2);

	//Jump button
	SneakyButtonSkinnedBase *leftButtonBase = SneakyButtonSkinnedBase::node(); 
	leftButtonBase->setPosition(leftButtonPosition); // 13
	leftButtonBase->setDefaultSprite(CCSprite::spriteWithFile("jumpUp.png")); // 14
	leftButtonBase->setActivatedSprite(CCSprite::spriteWithFile("jumpDown.png")); // 15
	leftButtonBase->setPressSprite(CCSprite::spriteWithFile("jumpDown.png")); // 16
	SneakyButton *aButton = new SneakyButton();
	aButton->initWithRect(leftButtonDimensions);
	aButton->autorelease();

	leftButtonBase->setButton(aButton); // 17
	//jumpButtonBase->getButton()->retain(); // 18
	leftButton = leftButtonBase->getButton();
	leftButton->setIsToggleable(false); // 19
	this->addChild(leftButtonBase); // 20

	//Attack button
	SneakyButtonSkinnedBase *rightButtonBase = SneakyButtonSkinnedBase::node(); 
	rightButtonBase->setPosition(rightButtonPosition); // 13
	rightButtonBase->setDefaultSprite(CCSprite::spriteWithFile("handUp.png")); // 14
	rightButtonBase->setActivatedSprite(CCSprite::spriteWithFile("handDown.png")); // 15
	rightButtonBase->setPressSprite(CCSprite::spriteWithFile("handDown.png")); // 16
	SneakyButton *aButton1 = new SneakyButton();
	aButton1->initWithRect(attackButtonDimensions);
	aButton1->autorelease();

	rightButtonBase->setButton(aButton1); // 17
	//attackButtonBase->getButton()->retain(); // 18
	rightButton = rightButtonBase->getButton();
	rightButton->setIsToggleable(false); // 19
	this->addChild(rightButtonBase); // 20
}