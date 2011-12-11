#include "SpaceCargoShip.h"

using namespace cocos2d;

void SpaceCargoShip::setDelegate(GameplayLayerDelegate *pDelegate)
{
	mDelegate = pDelegate;
}

void SpaceCargoShip::dropCargo()
{
	CCPoint cargoDropPosition = ccp(this->getScreenSize().width/2, this->getScreenSize().height);
	if (hasDroppedMallet = false)
	{
		CCLOG("SpaceCargoShip --> Mallet Powerup was created!");
		hasDroppedMallet = true;
		mDelegate->createObjectOfType(kPowerUpTypeMallet, 0.0f, cargoDropPosition, 50);
	}
	else
	{
		CCLOG("SpaceCargoShip --> Mallet Powerup was created!");
		mDelegate->createObjectOfType(kPowerUpTypeMallet, 0.0f, cargoDropPosition, 50);	
	}
}

void SpaceCargoShip::playSpaceCargoShipSound()
{
	if (soundNumberToPlay < 2) 
		PLAYSOUNDEFFECT(SPACECARGOSHIP_FAR);
	else if (soundNumberToPlay == 2) 
		PLAYSOUNDEFFECT(SPACECARGOSHIP_CLOSE_1);
	else if (soundNumberToPlay == 3)
		PLAYSOUNDEFFECT(SPACECARGOSHIP_CLOSE_2);
	else if (soundNumberToPlay == 4) 
		PLAYSOUNDEFFECT(SPACECARGOSHIP_CLOSE_3);
	
	soundNumberToPlay = soundNumberToPlay + 1;
	
	if (soundNumberToPlay > 4) 
		soundNumberToPlay = 0;
	
}

bool SpaceCargoShip::init()
{
	bool pRet = false;
	if (GameObject::init())
	{
		CCLOG("SpaceCargoShip init");
		soundNumberToPlay = 0;
		hasDroppedMallet = false;
		float shipHeight = this->getScreenSize().height * 0.71f;
		CCPoint position1 = ccp(this->getScreenSize().width * -0.48f, shipHeight);
		CCPoint position2 = ccp(this->getScreenSize().width * 2.0f, shipHeight);
		CCPoint position3 = ccp(position2.x * -1.0f, shipHeight);
		CCPoint offScreen = ccp(this->getScreenSize().width * -1.0f, this->getScreenSize().height * -1.0f);

		CCActionInterval *action = CCRepeatForever::actionWithAction((CCActionInterval *) CCSequence::actions(CCDelayTime::actionWithDuration(2.0f),
																						CCMoveTo::actionWithDuration(0.01f, position1),
																						CCScaleTo::actionWithDuration(0.01f, 0.5f),
																						CCFlipX::actionWithFlipX(true),
																						CCCallFunc::actionWithTarget(this, callfunc_selector(SpaceCargoShip::playSpaceCargoShipSound)),
																						CCMoveTo::actionWithDuration(8.5f, position2),
																						CCScaleTo::actionWithDuration(0.1f, 1.0f),
																						CCFlipX::actionWithFlipX(false),
																						CCCallFunc::actionWithTarget(this, callfunc_selector(SpaceCargoShip::playSpaceCargoShipSound)),
																						CCMoveTo::actionWithDuration(7.5f, position3),
																						CCScaleTo::actionWithDuration(0.1f, 2.0f),
																						CCFlipX::actionWithFlipX(true),
																						CCCallFunc::actionWithTarget(this, callfunc_selector(SpaceCargoShip::playSpaceCargoShipSound)),
																						CCMoveTo::actionWithDuration(6.5, position2),
																						CCFlipX::actionWithFlipX(false),
																						CCScaleTo::actionWithDuration(0.1f, 2.0f),
																						CCCallFunc::actionWithTarget(this, callfunc_selector(SpaceCargoShip::playSpaceCargoShipSound)),
																						CCMoveTo::actionWithDuration(5.5, position3),
																						CCFlipX::actionWithFlipX(true),
																						CCScaleTo::actionWithDuration(0.1f, 4.0f),
																						CCCallFunc::actionWithTarget(this, callfunc_selector(SpaceCargoShip::playSpaceCargoShipSound)),
																						CCMoveTo::actionWithDuration(4.5f, position2),
																						CCCallFunc::actionWithTarget(this, callfunc_selector(SpaceCargoShip::dropCargo)),
																						CCCallFunc::actionWithTarget(this, callfunc_selector(SpaceCargoShip::playSpaceCargoShipSound)),
																						CCMoveTo::actionWithDuration(0.0f, offScreen),
																						NULL));
		this->GameObject::runAction(action);																
		pRet = true;
	}
	return pRet;

}
