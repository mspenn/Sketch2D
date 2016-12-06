#include "Weapon.h"

USING_NS_CC;

void Weapon::onEnter()
{
	Sprite::onEnter();

	// initialize doAttackListener, subscribe EVENT_DO_ATTACK event 
	_doAttackListener = EventListenerCustom::create(EVENT_DO_ATTACK, [&](EventCustom* event){
		// cast UserData to AttackData pointer
		AttackData* ad = static_cast<AttackData*>(event->getUserData());
		// call doAttack to notify
		this->doAttack(*ad);
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_doAttackListener, this);
}

Pistal* Pistal::createWithTexture(Texture2D *texture, const Rect& rect, bool rotated)
{
	Pistal *sprite = new (std::nothrow) Pistal();
	if (sprite && sprite->initWithTexture(texture, rect, rotated))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool Pistal::initWithTexture(Texture2D *texture, const Rect& rect, bool rotated)
{
	if (!Weapon::initWithTexture(texture, rect, rotated))return false;
	
	// initialize bulletManager
	_bulletManager = BulletManager::create();

	return true;
}

bool Pistal::init()
{
	if(!Weapon::init())return false;

	// initialize bulletManager
	_bulletManager = BulletManager::create();

	return true;
}

Pistal::Pistal() 
:_gunman(nullptr)
, _bulletManager(nullptr)
{}

void Pistal::onEnter()
{
	Weapon::onEnter();
}

void Pistal::onExit()
{
	// stop all actions/animations when exit "stage"
	this->stopAllActions();

	Weapon::onExit();
}

void Pistal::doAttack(const AttackData& attackData)
{
	// check if this/this guman is possession of weapon
	// investigator is gunman when pistal is attached to a gunman
	// investigator is self when pistal has no owner
	if (attackData._investigator == this->_gunman
		|| attackData._investigator == this)
	{
		Vec2 loc = this->getPosition() + this->getContentSize() / 2;
		_bulletManager->spawnNewBullet(loc);
		log("pistal do attack!");
	}
}