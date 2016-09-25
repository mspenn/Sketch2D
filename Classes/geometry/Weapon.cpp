#include "Weapon.h"

USING_NS_CC;

void Weapon::onEnter()
{
	Sprite::onEnter();

	_doAttackListener = EventListenerCustom::create(EVENT_DO_ATTACK, [&](EventCustom* event){
		AttackData* ad = static_cast<AttackData*>(event->getUserData());
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
	
	_bulletManager = BulletManager::create();
	//this->getParent()->addChild(_bulletManager);
	return true;
}

bool Pistal::init()
{
	if(!Weapon::init())return false;
	
	_bulletManager = BulletManager::create();
	//this->getParent()->addChild(_bulletManager);
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
	this->stopAllActions();
	//this->getParent()->removeChild(_bulletManager);
	Weapon::onExit();
}

void Pistal::doAttack(const AttackData& attackData)
{
	if (attackData._investigator == this->_gunman
		|| attackData._investigator == this)
	{
		Vec2 loc = this->getPosition() + this->getContentSize() / 2;
		_bulletManager->spawnNewBullet(loc);
		log("pistal do attack!");
	}
}