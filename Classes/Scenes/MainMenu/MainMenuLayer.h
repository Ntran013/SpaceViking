#ifndef __MAIN_MENU_LAYER_H__
#define __MAIN_MENU_LAYER_H__

#include "cocos2d.h"
#include "Constants\Constants.h"
#include "Singletons\GameManager.h"

class MainMenuLayer : public cocos2d::CCLayer
{
protected:
	cocos2d::CCMenu *mainMenu;
	cocos2d::CCMenu *sceneSelectMenu;
	cocos2d::CCLabelTTF *m_pLabelLoading;
public:
	LAYER_NODE_FUNC(MainMenuLayer);
	void buyBook(cocos2d::CCObject* pSender);
	void showOptions(cocos2d::CCObject* pSender);
	void playScene(cocos2d::CCObject *itemPassedIn);
	void loadResourcesAync();
	bool init();
	MainMenuLayer();
private:
	void displayMainMenu(cocos2d::CCObject* pSender);
	void displaySceneSelection(cocos2d::CCObject* pSender);
	void loadingCallBack(cocos2d::CCObject *psender);
};

#endif