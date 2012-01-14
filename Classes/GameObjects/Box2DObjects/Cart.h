#ifndef __CART_H__
#define __CART_H__

#include "Box2DSprite.h"
#include "Box2DHelpers.h"

class Cart : public Box2DSprite 
{
protected:
	b2World *world;
	b2RevoluteJoint *wheelLJoint;
	b2RevoluteJoint *wheelRJoint;
	b2Body *legsBody;
	b2Body *trunkBody;
	b2Body *headBody;
	b2Body *helmBody;
	b2Body *armBody;
	CC_SYNTHESIZE_READONLY(Box2DSprite *, legs, Legs);
	CC_SYNTHESIZE_READONLY(Box2DSprite *, trunk, Trunk);
	CC_SYNTHESIZE_READONLY(Box2DSprite *, head, Head);
	CC_SYNTHESIZE_READONLY(Box2DSprite *, helm, Helm);
	CC_SYNTHESIZE_READONLY(Box2DSprite *, arm, Arm);
	CC_SYNTHESIZE(Box2DSprite *, wheelL, WheelL);
	CC_SYNTHESIZE(Box2DSprite *, wheelR, WheelR);
	CC_SYNTHESIZE(b2Body *, wheelLBody, WheelLBody);
	CC_SYNTHESIZE(b2Body *, wheelRBody, WheelRBody);

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
	b2Body * createPartAtLocation(b2Vec2 location, Box2DSprite *sprite);
	void createOle();
	void playHitEffect();
	virtual void changeState(CharacterStates newState);
};

#endif