#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "Constants\Constants.h"
#include "cocos2d.h"

class GameManager :	public cocos2d::CCObject 
{
	CC_SYNTHESIZE(bool, isMusicON, IsMusicON);
	CC_SYNTHESIZE(bool, isSoundEffectsON, IsSoundEffectsON);
	CC_SYNTHESIZE(bool, hasPlayerDied, HasPlayerDied);
	CC_SYNTHESIZE(int, killCount, KillCount);
	SceneTypes currentScene;

	//public methods
	static GameManager * sharedGameManager();
	void runSceneWithID(SceneTypes sceneID);
	void openSiteWithLinkType(LinkTypes linkTypeToOpen);
	bool init();
};
#endif