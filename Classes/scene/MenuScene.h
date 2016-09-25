#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"

class MenuScene :public cocos2d::Scene
{
public:
	virtual bool init();

	CREATE_FUNC(MenuScene);

protected:
	void onTraining(Ref* sender);
	void onGaming(Ref* sender);

private:
	cocos2d::Label* _labelTraining;
	cocos2d::Label* _labelGaming;
};

#endif