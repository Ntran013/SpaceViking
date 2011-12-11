#include "OptionsLayer.h"

using namespace cocos2d;

void OptionsLayer::returnToMainMenu(CCObject* pSender)
{
	GameManager::sharedGameManager()->runSceneWithID(kMainMenuScene);
}

void OptionsLayer::showCredits(CCObject* pSender)
{
	GameManager::sharedGameManager()->runSceneWithID(kCreditsScene);
}

void OptionsLayer::musicTogglePressed(CCObject* pSender)
{
	if (GameManager::sharedGameManager()->getIsMusicON()) 
	{
		CCLOG("OptionsLayer-> Turning Game Music OFF");
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
		GameManager::sharedGameManager()->setIsMusicON(false);
	} 
	else 
	{
		CCLOG("OptionsLayer-> Turning Game Music ON");
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sounds/Music/VikingPreludeV1.mp3");
		GameManager::sharedGameManager()->setIsMusicON(true);
	}	
}

void OptionsLayer::SFXTogglePressed(CCObject* pSender)
{
	if (GameManager::sharedGameManager()->getIsSoundEffectsON()) 
	{
		CCLOG("OptionsLayer-> Turning Sound Effects OFF");
		GameManager::sharedGameManager()->setIsSoundEffectsON(false);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseAllEffects();
	} 
	else 
	{
		CCLOG("OptionsLayer-> Turning Sound Effects ON");
		GameManager::sharedGameManager()->setIsSoundEffectsON(true);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeAllEffects();
	}	
}

bool OptionsLayer::init()
{
	bool pRet = false;
	if (CCLayer::init())
	{
		CCSprite *background = CCSprite::spriteWithFile("Menus/Main/MainMenuBackground.png");
		background->setPosition(ccp(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
		this->addChild(background);

		CCLabelBMFont *musicOnLabelText = CCLabelBMFont::labelWithString("Music ON", "Fonts/VikingSpeechFont64.fnt");
		CCLabelBMFont *musicOffLabelText = CCLabelBMFont::labelWithString("Music OFF", "Fonts/VikingSpeechFont64.fnt");
		CCLabelBMFont *SFXOnLabelText = CCLabelBMFont::labelWithString("Sound Effects ON", "Fonts/VikingSpeechFont64.fnt");
		CCLabelBMFont *SFXOffLabelText = CCLabelBMFont::labelWithString("Sound Effects OFF", "Fonts/VikingSpeechFont64.fnt");

		CCMenuItemLabel *musicOnLabel = CCMenuItemLabel::itemWithLabel(musicOnLabelText, this, NULL);
		CCMenuItemLabel *musicOffLabel = CCMenuItemLabel::itemWithLabel(musicOffLabelText, this, NULL);
		CCMenuItemLabel *SFXOnLabel = CCMenuItemLabel::itemWithLabel(SFXOnLabelText, this, NULL);
		CCMenuItemLabel *SFXOffLabel = CCMenuItemLabel::itemWithLabel(SFXOffLabelText, this, NULL);

		CCMenuItemToggle *musicToggle = CCMenuItemToggle::itemWithTarget(this, 
																menu_selector(OptionsLayer::musicTogglePressed), 
																   musicOnLabel, musicOffLabel, NULL);
		
		CCMenuItemToggle *SFXToggle = CCMenuItemToggle::itemWithTarget(this, 
																menu_selector(OptionsLayer::SFXTogglePressed), 
																   SFXOnLabel, SFXOffLabel, NULL);
		
		CCLabelBMFont *creditsButtonLabel = CCLabelBMFont::labelWithString("Credits", "Fonts/VikingSpeechFont64.fnt");
		CCMenuItemLabel	*creditsButton = CCMenuItemLabel::itemWithLabel(creditsButtonLabel, this, menu_selector(OptionsLayer::showCredits));

		CCLabelBMFont *backButtonLabel = CCLabelBMFont::labelWithString("Back", "Fonts/VikingSpeechFont64.fnt");
		CCMenuItemLabel	*backButton = CCMenuItemLabel::itemWithLabel(backButtonLabel, this, menu_selector(OptionsLayer::returnToMainMenu));

		CCMenu *optionsMenu = CCMenu::menuWithItems(musicToggle,
							   SFXToggle,
							   creditsButton,
							   backButton, NULL);
		optionsMenu->alignItemsVerticallyWithPadding(60.0f);
		optionsMenu->setPosition(ccp(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT/2));
		this->addChild(optionsMenu);

       if (GameManager::sharedGameManager()->getIsMusicON() == false) {
            musicToggle->setSelectedIndex(1); // Music is OFF
        }
        
        if (GameManager::sharedGameManager()->getIsSoundEffectsON() == false) {
            SFXToggle->setSelectedIndex(1); // SFX are OFF
        }
		pRet = true;
	}

	return pRet;
}