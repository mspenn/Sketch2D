#ifndef __BULLET_H__
#define __BULLET_H__

#include "cocos2d.h"

/**
 * Bullet Manager
 * Spawn bullets, update bullets location in this layer
 * @see cocos2d::Layer
 */
class BulletManager :public cocos2d::Layer
{
public:
	// Constructor
	BulletManager();

	// Deconstructor
	~BulletManager();

	/**
	 * Call when BulletManager initialized, override
	 * @see cocos2d::Layer::init
	 */
	virtual bool init();
	
	/**
	 * Call when BulletManager enters the 'stage', override
	 * @see cocos2d::Layer::onEnter
	 */
	virtual void onEnter();

	/**
	 * Event on Physics Contact Begin
	 * Call when bullet hit other sprite
	 * @param contact contact structure
	 * @see cocos2d::PhysicsContact
	 * @return true if called successful, false otherwise
	 */
	bool onContactBegin(const cocos2d::PhysicsContact& contact);

	/**
	 * Spawn new bullet
	 * @param spawnLocation location to spawn a new bullet
	 */
	void spawnNewBullet(cocos2d::Vec2 spawnLocation);

	/**
	 * Spawn bullet dusty when bullet hit other sprite
	 * @param spawnLocation location to spawn dusty
	 */
	void spawnBulletDusty(cocos2d::Vec2 spawnLocation);

	/**
	 * Update bullet location, called every tick
	 * Make bullet moving
	 * @param delta	elapsed time
	 */
	void updateBulletMoving(float delta);

	/**
	 * Static factory function to create BulletManager
	 * return BulletManager instance
	 */
	CREATE_FUNC(BulletManager);
private:
	cocos2d::Array*				m_bulletCache;		// Cache to store bullets
	cocos2d::SpriteBatchNode*	m_spriteBatchNode;	// Batch node to generate renderable bullet sprite
	cocos2d::SpriteBatchNode*	m_dustyBatchNode;	// Batch node to generate renderable dusty
	cocos2d::Vec2				_velocity;			// Bullet moving velocity
	cocos2d::Size				_winSize;			// Bullet layer size
};

#endif	/* __BULLET_H__ */