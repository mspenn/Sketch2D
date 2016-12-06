#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "cocos2d.h"
#include "geometry/Bullet.h"

#define EVENT_DO_ATTACK "onAttack"

/**
 * Attack Data for Do Attack Event
 */
struct AttackData
{
	// Constructor
	AttackData(cocos2d::Vec2 targetLoc, cocos2d::Node* investigator)
	:_targetLocation(targetLoc)
	, _investigator(investigator)
	{}

	cocos2d::Vec2	_targetLocation;	// attack location
	cocos2d::Node*	_investigator;		// attack investigator
};

/**
 * Weapon Sprite, abstract for different kinds of weapon
 * @see cocos2d::Sprite
 */
class Weapon :public cocos2d::Sprite
{
public:
	/**
	 * Call when Weapon enters the 'stage', override
	 * @see cocos2d::Node::onEnter
	 */
	virtual void onEnter();
	
	/** 
	 * Do attack event, pure virtual function, need to be override
	 * @param attack data, event argument to be send 
	 * @see AttackData
	 */
	virtual void doAttack(const AttackData&) = 0;

private:
	cocos2d::EventListenerCustom* _doAttackListener; //	attack event listener, @see cocos2d::EventListenerCustom
};

/**
 * Pistal Weapon Sprite, inherit from Weapon Sprite
 * @see Weapon
 */
class Pistal :public Weapon
{
public:

	// Constructor
	Pistal();

	/**
	 * Call when Pistal Sprite inititialize the 'stage', override
	 * @see cocos2d::Node::init
	 */
	virtual bool init();

	/**
	 * Call when Pistal Sprite enters the 'stage', override
	 * @see cocos2d::Node::onEnter
	 */
	virtual void onEnter();

	/**
	 * Call when Pistal Sprite exits the 'stage', override
	 * @see cocos2d::Node::onExit
	 */
	virtual void onExit();

	/**
	 * Do attack event, override
	 * @param attack data	event argument to be send
	 * @see AttackData
	 */
	virtual void doAttack(const AttackData&);

	/**
	 * Init Pistal Sprite with Texture, override
	 * @param texture	a pointer to an existing Texture2D object
	 * @param rect		only the contents inside rect of this texture will be applied for this sprite.
	 * @param rotated	whether or not the texture rectangle is rotated.
	 * @return true	if the sprite is initialized properly
	 * @return	false	otherwise.
	 * @see cocos2d::Sprite::initWithTexture
	 */
	virtual bool initWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect& rect, bool rotated);

	/**
	 * Static factory to create Pistal Sprite object
	 * @param texture	a pointer to an existing Texture2D object
	 * @param rect		only the contents inside rect of this texture will be applied for this sprite.
	 * @param rotated	whether or not the texture rectangle is rotated, default false 
	 * @return			a poiter to Pistal Sprite object
	 */
	static Pistal* createWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect& rect, bool rotated = false);

	/**
	 * Get Bullet object layer
	 * @see BulletManager
	 * @return	Bullet object layer
	 */
	BulletManager* getBulletLayer() const
	{
		return this->_bulletManager;
	}

	/**
	 * Get Bullet object layer
	 * @see BulletManager
	 * @return	Bullet object layer
	 */
	void setGunman(cocos2d::Node* gunman)
	{
		this->_gunman = gunman;
	}

	/**
	 * Get Gunman who is possession of this pistal
	 * @return	Gunman object pointer
	 */
	cocos2d::Node* getGunman()
	{
		return this->_gunman;
	}

private:
	BulletManager* _bulletManager;	// BulletManager for spawning bullets
	cocos2d::Node* _gunman;			// pointer to gunman
};

/**
* Axe Weapon Sprite, inherit from Weapon Sprite
* @see Weapon
*/
class Axe :public Weapon
{
public:

	/**
	 * Do attack event, override
	 * @param attack data	event argument to be send
	 * @see AttackData
	 */
	virtual void doAttack(const AttackData&);
private:

};

#endif	/* __WEAPON_H__ */