#ifndef __OPTIONS_LAYER_H__
#define __OPTIONS_LAYER_H__

#include "cocos2d.h"
#include "Constants\Constants.h"
#include "Singletons\GameManager.h"

class OptionsLayer : public cocos2d::CCLayer
{
public:
	LAYER_NODE_FUNC(OptionsLayer);
	void returnToMainMenu(cocos2d::CCObject* pSender);
	void showCredits(cocos2d::CCObject* pSender);
	void musicTogglePressed(cocos2d::CCObject* pSender);
	void SFXTogglePressed(cocos2d::CCObject* pSender);
	bool init();
};

#endif