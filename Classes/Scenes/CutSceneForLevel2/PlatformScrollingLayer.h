#ifndef __PLATFORM_SCROLLING_LAYER_H__ 
#define __PLATFORM_SCROLLING_LAYER_H__

#include "cocos2d.h"
#include "Constants\Constants.h"

class PlatformScrollingLayer : public cocos2d::CCLayer 
{
protected:
	cocos2d::CCSpriteBatchNode *scrollingBatchNode;
	bool hasBeenSkipped;
public:
	bool init();
	void ccTouchesBegan(cocos2d::CCSet *touches, cocos2d::CCEvent *event);
	LAYER_NODE_FUNC(PlatformScrollingLayer);
private:
	void resetCloudWithNode(cocos2d::CCNode *node);
	void createCloud();
	void createVikingAndPlatform();
	void createStaticBackground();
};
#endif