#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "cocos2d.h"
#include "geometry/Bullet.h"

#define EVENT_DO_ATTACK "onAttack"

struct AttackData
{
	AttackData(cocos2d::Vec2 targetLoc, cocos2d::Node* investigator)
	:_targetLocation(targetLoc)
	, _investigator(investigator)
	{}

	cocos2d::Vec2 _targetLocation;
	cocos2d::Node* _investigator;
};

class Weapon :public cocos2d::Sprite
{
public:
	virtual void onEnter();
	virtual void doAttack(const AttackData&) = 0;
private:
	cocos2d::EventListenerCustom* _doAttackListener;
};

class Pistal :public Weapon
{
public:
	Pistal();
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual void doAttack(const AttackData&);
	virtual bool initWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect& rect, bool rotated);
	static Pistal* createWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect& rect, bool rotated = false);

	BulletManager* getBulletLayer() const{ return this->_bulletManager; }
	void setGunman(cocos2d::Node* gunman){ this->_gunman = gunman; }
	cocos2d::Node* getGunman(){ return this->_gunman; }
private:
	BulletManager* _bulletManager;
	cocos2d::Node* _gunman;
};

class Axe :public Weapon
{
public:
	virtual void doAttack(const AttackData&);
private:

};
#endif