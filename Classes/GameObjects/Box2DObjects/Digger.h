#ifndef __DIGGER_H__
#define __DIGGER_H__

#include "Cart.h"
#include "Box2DSprite.h"
#include "platform.h"
#include "SimpleAudioEngine.h"

class Digger : public Box2DSprite 
{
protected:
	b2Body *wheelLBody;
	b2Body *wheelRBody;
	b2RevoluteJoint *wheelLJoint;
	b2RevoluteJoint *wheelRJoint;
	b2Body *drillLBody;
	b2Body *drillRBody;
	b2Fixture *drillLFixture;
	b2Fixture *drillRFixture;
	cocos2d::cc_timeval movingStartTime;
	cocos2d::CCAnimation *rotateAnim;
	cocos2d::CCAnimation *drillAnim;

	CC_SYNTHESIZE(Box2DSprite *, wheelLSprite, WheelLSprite);
	CC_SYNTHESIZE(Box2DSprite *, wheelRSprite, WheelRSprite);

private:
	void createBodyWithWorld(b2World *world, cocos2d::CCPoint location);
	void createWheelsWithWorld(b2World *world);
	void createDrillWithWorld(b2World *world);
	void disableDrills();
	void enableDrills();
	void initAnimations();
public:
	bool initWithWorld(b2World *theWorld, cocos2d::CCPoint location);
	virtual void updateStateWithDeltaTime(cocos2d::ccTime deltaTime, cocos2d::CCArray *listOfGameObjects);
	virtual void changeState(CharacterStates newState);
	~Digger();
};

#endif