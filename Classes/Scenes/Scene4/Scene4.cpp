#include "Scene4.h"

using namespace cocos2d;

bool Scene4::init()
{
	bool pRet = false;

	if (CCScene::init())
	{
		Scene4UILayer *uiLayer = Scene4UILayer::node();
		this->addChild(uiLayer, 1);
		Scene4ActionLayer *actionLayer = new Scene4ActionLayer();
		actionLayer->initWithScene4UILayer(uiLayer);
		actionLayer->autorelease();
		this->addChild(actionLayer, 0);

		pRet = true;
	}
	
	return pRet;
}