#ifndef __CART_H__
#define __CART_H__

#include "Box2DSprite.h"

class Cart : public Box2DSprite 
{
protected:
	b2World *world;
	CC_SYNTHESIZE(Box2DSprite *, wheelL, WheelL);
	CC_SYNTHESIZE(Box2DSprite *, wheelR, WheelR);
	CC_SYNTHESIZE(b2Body *, wheelLBody, WheelLBody);
	CC_SYNTHESIZE(b2Body *, wheelRBody, WheelRBody);
	b2RevoluteJoint *wheelLJoint;
	b2RevoluteJoint *wheelRJoint;
public:
	bool initWithWorld(b2World *theWorld, cocos2d::CCPoint location);
	void createBodyAtLocation(cocos2d::CCPoint location);
	b2Body * createWheelWithSprite(Box2DSprite *sprite, b2Vec2 offset);
	void createWheels();
	void setMotorSpeed(float32 motorSpeed);
	void updateStateWithDeltaTime(cocos2d::ccTime deltaTime, cocos2d::CCArray *listOfGameObjects);
	void playJumpEffect();
	float32 fullMass();
	void jump();
};

#endif