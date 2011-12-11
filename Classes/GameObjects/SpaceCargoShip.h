#ifndef __SHIP_H__
#define __SHIP_H__

#include "GameObject.h"

class SpaceCargoShip : public GameObject
{
protected:
	bool hasDroppedMallet;
	GameplayLayerDelegate *mDelegate;
	int soundNumberToPlay;

public:
	//Public methods
	void dropCargo();
	bool init();
	void setDelegate(GameplayLayerDelegate *pDelegate);
	void playSpaceCargoShipSound();
};
#endif