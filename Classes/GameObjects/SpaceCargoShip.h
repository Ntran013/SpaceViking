#ifndef __SHIP_H__
#define __SHIP_H__

#include "GameObject.h"

class SpaceCargoShip : public GameObject, public GameplayLayerDelegate
{
protected:
	bool hasDroppedMallet;
	GameplayLayerDelegate *mDelegate;

public:
	//Public methods
	void dropCargo();
	bool init();
	void setDelegate(GameplayLayerDelegate *pDelegate);
	virtual void createObjectOfType(GameObjectType objectType, int initialHealth, cocos2d::CCPoint spawnLocation, int zValue);
	virtual void createPhaserWithDirection(PhaserDirection phaserDirection, cocos2d::CCPoint spawnPosition);
};
#endif