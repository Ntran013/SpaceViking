#ifndef __G_LAYER_H__
#define __G_LAYER_H__

#include "cocos2d.h"
#include "JoystickClasses\SneakyButton.h"
#include "JoystickClasses\SneakyButtonSkinnedBase.h"
#include "JoystickClasses\SneakyJoystick.h"
#include "JoystickClasses\SneakyJoystickSkinnedBase.h"
#include "Constants\Constants.h"
#include "Constants\CommonProtocols.h"
#include "EnemyObjects\RadarDish.h"
#include "GameObjects\Viking.h"
#include "PowerUps\Mallet.h"
#include "PowerUps\Health.h"
#include "GameObjects\SpaceCargoShip.h"

class GameplayLayer : public cocos2d::CCLayer, public GameplayLayerDelegate
{
public:
	virtual bool init();
    LAYER_NODE_FUNC(GameplayLayer);
	void initJoystickAndButtons();
	void applyJoystick(SneakyJoystick *aJoystick, cocos2d::CCNode *tempNode, float deltaTime);
	void update(cocos2d::ccTime deltaTime);
	virtual void createObjectOfType(GameObjectType objectType, int initialHealth, CCPoint spawnLocation, int zValue);
	virtual void createPhaserWithDirection(PhaserDirection phaserDirection, CCPoint spawnPosition);
protected:
	SneakyJoystick *lefJoystick;
	SneakyButton *jumpButton;
	SneakyButton *attackButton;
	CCSpriteBatchNode *sceneSpriteBatchNode;
};
#endif