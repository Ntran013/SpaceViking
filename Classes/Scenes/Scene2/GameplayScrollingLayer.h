#ifndef __GAMEPLAY_SCROLLING_LAYER_H__
#define __GAMEPLAY_SCROLLING_LAYER_H__

#include "cocos2d.h"
#include "Constants\Constants.h"
#include "GameObjects\Viking.h"
#include "GameControlLayer.h"

class GameplayScrollingLayer : public cocos2d::CCLayer
{
protected:
	cocos2d::CCSpriteBatchNode *sceneSpriteBatchNode;

	cocos2d::CCTMXTiledMap *tileMapNode;
	cocos2d::CCParallaxNode *parallaxNode;

public:
	bool init();
	LAYER_NODE_FUNC(GameplayScrollingLayer);
	void connectControlsWithJoystick(SneakyJoystick *leftJoystick, SneakyButton *jumpButton, SneakyButton *attackButton);
	void addScrollingBackground();
	void adjustLayer();
	void update(cocos2d::ccTime deltaTime);
	void addScrollingBackgroundWithParallax();
	void addScrollingBackgroundWithTileMap();
	void addScrollingBackgroundWithTileMapInsideParallax();
};

#endif