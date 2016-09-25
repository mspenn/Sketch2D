#ifndef __BULLET_H__
#define __BULLET_H__

#include "cocos2d.h"

class BulletManager :public cocos2d::Layer
{
public:
	BulletManager();
	~BulletManager();
	virtual bool init();
	virtual void onEnter();

	bool onContactBegin(const cocos2d::PhysicsContact& contact);

	void spawnNewBullet(cocos2d::Vec2 spawnLocation);
	void spawnBulletDusty(cocos2d::Vec2 spawnLocation);
	void updateBulletMoving(float delta);// override;
	CREATE_FUNC(BulletManager);
private:
	cocos2d::Array* m_bulletCache;
	cocos2d::SpriteBatchNode* m_spriteBatchNode;
	cocos2d::SpriteBatchNode* m_dustyBatchNode;
	cocos2d::Vec2 _velocity;
	cocos2d::Size _winSize;
};

#endif