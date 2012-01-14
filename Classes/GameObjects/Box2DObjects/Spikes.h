#ifndef __SPIKE_H__
#define __SPIKE_H__

#include "cocos2d.h"
#include "Box2DSprite.h"

class Spikes : public Box2DSprite 
{
protected:
	b2World *world;
private:
	void createBodyAtLocation(cocos2d::CCPoint location);
public:
	bool initWithWorld(b2World *theWorld, cocos2d::CCPoint location);
};

#endif