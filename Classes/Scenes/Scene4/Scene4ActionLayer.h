#ifndef __SCENE4_ACTION_LAYER_H__
#define __SCENE4_ACTION_LAYER_H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "GLES-Render.h"
#include "Constants\Constants.h"
#include "Scene4UILayer.h"
#include "GameObjects\Box2DObjects\Cart.h"
#include "SimpleQueryCallback.h"

class Scene4ActionLayer : public cocos2d::CCLayer
{
protected:
	b2World *world;
	GLESDebugDraw *debugDraw;
	cocos2d::CCSpriteBatchNode *sceneSpriteBatchNode;
	b2Body *groundBody;
	b2MouseJoint *mouseJoint;
	Cart *cart;
	Scene4UILayer *uiLayer;
public:
	~Scene4ActionLayer();
	void setUpWorld();
	void setUpDebugDraw();
	void createGround();
	void createCartAtLocation(cocos2d::CCPoint location);
	void registerWithTouchDispatcher();
	bool initWithScene4UILayer(Scene4UILayer *scene4UILayer);
	void update(cocos2d::ccTime dt);
	virtual void draw();
	bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
	void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
	void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
	void keyBackClicked();
	void accelerometer(cocos2d::CCAccelerometer *accelerometer, cocos2d::CCAcceleration *acceleration);
};

#endif