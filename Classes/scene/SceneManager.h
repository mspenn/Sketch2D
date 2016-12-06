#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "cocos2d.h"

/**
 * A simple scene factory
 */
class SceneManager
{
public:
	
	/**
	 * Get CanvasScene instance
	 * @return a new CanvasScene instance
	 */
	static cocos2d::Scene *GetCanvasScene();
	
	/**
	 * Get GameScene instance
	 * @return a new GameScene instance
	 */
	static cocos2d::Scene *GetGameScene();
	
	/**
	 * Get TrainingScene instance
	 * @return a new TrainingScene instance
	 */
	static cocos2d::Scene *GetTrainingScene();
	
	/**
	 * Get MenuScene instance
	 * @return a new MenuScene instance
	 */
	static cocos2d::Scene *GetMenuScene();
};

#endif	/* __SCENE_MANAGER_H__ */