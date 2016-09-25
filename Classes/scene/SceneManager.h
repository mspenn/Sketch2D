#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "cocos2d.h"
//#include "util/VisibleRect.h"

class SceneManager
{
public:
	static cocos2d::Scene *GetCanvasScene();
	static cocos2d::Scene *GetLogoScene();
	static cocos2d::Scene *GetGameScene();
	static cocos2d::Scene *GetTestScene(); 
	static cocos2d::Scene *GetTrainingScene();
	static cocos2d::Scene *GetMenuScene();
private:

};

#endif