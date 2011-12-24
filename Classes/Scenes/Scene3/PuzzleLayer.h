#ifndef __PUZZLE_LAYER_H__
#define __PUZZLE_LAYER_H__

#include "cocos2d.h"
#include "Constants\Constants.h"
#include "Singletons\GameManager.h"
#include "Box2D\Box2D.h"
#include "GLES-Render.h"
#include "SimpleQueryCallback.h"
#include "GameObjects\Box2DObjects\Box2DSprite.h"
#include "GameObjects\Box2DObjects\Meteor.h"
#include "GameObjects\Box2DObjects\Skull.h"
#include "GameObjects\Box2DObjects\Rock.h"
#include "GameObjects\Box2DObjects\IceBlock.h"
#include "GameObjects\Box2DObjects\LongBlock.h"
#include "GameObjects\Box2DObjects\FrozenOle.h"

class PuzzleLayer : public cocos2d::CCLayer 
{
public:
	~PuzzleLayer();
	static cocos2d::CCScene * scene();
	bool init();
	LAYER_NODE_FUNC(PuzzleLayer);
	void setUpWorld();
	void createBoxAtLocation(cocos2d::CCPoint location, cocos2d::CCSize size, float32 friction, float32 restitution, float32 density);
	void createBoxAtRandomLocation(cocos2d::ccTime dt);
	void createBodyAtLocation(cocos2d::CCPoint location, Box2DSprite *sprite, float32 friction, float32 restitution, float32 density, bool isBox);
	void setUpDebugDraw();
	virtual void draw();
	void registerWithTouchDispatcher();
	void update(cocos2d::ccTime dt);
	bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
	void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
	void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
	void createGround();
	virtual void keyBackClicked();
	void accelerometer(cocos2d::CCAccelerometer *accelerometer, cocos2d::CCAcceleration *acceleration);
	void createObjectAtLocation(GameObjectType type, cocos2d::CCPoint location);
	void createSensor();
	void instructions();
	void winComplete(cocos2d::CCNode *pSender);
	void win();
protected:
	b2World *world;
	GLESDebugDraw *debugDraw;
	b2Body *groundBody;
	b2MouseJoint *mouseJoint;
	cocos2d::CCSpriteBatchNode *sceneSpriteBatchNode;
	b2Body *frozenVikingBody;
	b2Body *sensorBody;
	bool hasWon;
};

#endif