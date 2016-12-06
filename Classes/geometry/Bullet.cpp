#include "geometry/Bullet.h"
#include "util/VisibleRect.h"
#include "resource/Resources.h"

USING_NS_CC;

#define BULLET_TAG 0x1000

BulletManager::BulletManager() 
:_velocity(Vec2(1000.0f, 0))
, m_bulletCache(nullptr)
{
}

BulletManager::~BulletManager()
{
	if (m_bulletCache)
	{
		m_bulletCache->release();
		m_bulletCache = nullptr;
	}
}

bool BulletManager::init()
{
	if (!Layer::init())return false;
	_winSize = VisibleRect::size();
	m_bulletCache = Array::create();
	m_bulletCache->retain();

	// create batch node for bullet and dusty sprites which appear many times
	m_spriteBatchNode = CCSpriteBatchNode::create(RES_IMAGE(bullet.png));
	m_spriteBatchNode->setPosition(Vec2::ZERO);
	this->addChild(m_spriteBatchNode);
	m_dustyBatchNode = CCSpriteBatchNode::create(RES_IMAGE(fire.png));
	m_dustyBatchNode->setPosition(Vec2::ZERO);
	this->addChild(m_dustyBatchNode);
	schedule(schedule_selector(BulletManager::updateBulletMoving));

	return true;
}

void BulletManager::onEnter()
{
	Layer::onEnter();

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(BulletManager::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

bool BulletManager::onContactBegin(const PhysicsContact& contact)
{
	auto a = contact.getShapeA()->getBody()->getNode();
	auto b = contact.getShapeB()->getBody()->getNode();
	// if a is a bullet, spawn dusty and remove it
	if (BULLET_TAG == a->getTag())
	{
		spawnBulletDusty(a->getPosition());
		a->removeFromParent();
		m_bulletCache->removeObject(a);
	}
	// if b is a bullet, spawn dusty and remove it
	if (BULLET_TAG == b->getTag())
	{
		spawnBulletDusty(b->getPosition());
		b->removeFromParent();
		m_bulletCache->removeObject(b);
	}
	return true;
}

void BulletManager::updateBulletMoving(float delta)
{
	Layer::update(delta);
	for (int i = 0; i < m_bulletCache->count(); ++i) 
	{
		Sprite* bullet = (Sprite*)m_bulletCache->objectAtIndex(i);
		Vec2 newLoc = bullet->getPosition() + _velocity * delta;
		bullet->setPosition(newLoc);

		// remove bullet sprite when sprite is out of visible bounder
		if (newLoc.x >= _winSize.width
			|| newLoc.x <= 0
			|| newLoc.y >= _winSize.height
			|| newLoc.y <= 0)
		{
			m_bulletCache->removeObjectAtIndex(i);
			m_spriteBatchNode->removeChild(bullet, true);
		}
	}
}

void BulletManager::spawnNewBullet(Vec2 spawnLocation)
{
	// spawn a bullet sprite
	// and initialize properties
	Sprite* newBullet = Sprite::createWithTexture(m_spriteBatchNode->getTexture());
	newBullet->setScale(0.4f);
	newBullet->setPosition(spawnLocation);
#define BULLET_RADIUS 12.0f
	PhysicsBody* physicsBody = PhysicsBody::createCircle(BULLET_RADIUS);
	physicsBody->setMass(INFINITY);
	// can notify onContact event when hit other sprites
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	newBullet->setPhysicsBody(physicsBody);
	newBullet->setTag(BULLET_TAG);
	m_spriteBatchNode->addChild(newBullet);
	m_bulletCache->addObject(newBullet);
}

void BulletManager::spawnBulletDusty(Vec2 spawnLocation)
{
	// spawn a dusty sprite
	// and initialize properties
	Sprite* newDusty = Sprite::createWithTexture(m_dustyBatchNode->getTexture());
	newDusty->setScale(0.4f);
	newDusty->setPosition(spawnLocation);
	m_dustyBatchNode->addChild(newDusty);
	// play fade in & out animation
	auto actionFadeIn = FadeIn::create(0.2f);
	auto actionFadeOut = FadeOut::create(0.4f);
	auto actionDone = CallFuncN::create([&](Node* node){node->removeFromParent(); });
	Sequence* sequence = Sequence::create(actionFadeIn, actionFadeOut, actionDone, NULL);
	newDusty->runAction(sequence);
}