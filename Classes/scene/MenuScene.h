#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"

/**
 * Menu Scene
 */
class MenuScene :public cocos2d::Scene
{
public:

	/**
	 * Call when Menu Scene inititialized, override
	 * @see cocos2d::Node::init
	 */
	virtual bool init();
	
	/**
	 * Static factory function to create MenuScene
	 * return MenuScene instance
	 */
	CREATE_FUNC(MenuScene);

protected:

	/**
	 * On switching to training scene
	 * @param sender	event sender
	 * @see coco2d::Ref
	 */
	void onTraining(Ref* sender);

	/**
	 * On switching to gaming scene
	 * @param sender	event sender
	 * @see coco2d::Ref
	 */
	void onGaming(Ref* sender);

private:
	cocos2d::Label* _labelTraining;	// trining label
	cocos2d::Label* _labelGaming;	// gaming label
};

#endif	/* __MENU_SCENE_H__ */