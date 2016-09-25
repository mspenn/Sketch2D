#include "controller/PawnController.h"
#include "geometry/Weapon.h"

USING_NS_CC;

#define DEFAULT_SPEED 100.0f

PawnController::PawnController()
:_velocity(Vec2::ZERO),
_isMoveing(false)
{}

bool PawnController::init()
{
	if (!CharacterController::init())
	{
		return false;
	}

	bindKeyPressedAction(EventKeyboard::KeyCode::KEY_D, [&](Node* character, float deltaTime){
		this->setIsMoving(true);
		this->setVelocity(Vec2(DEFAULT_SPEED, 0.0f));
	});
	bindKeyPressedAction(EventKeyboard::KeyCode::KEY_A, [&](Node* character, float deltaTime){
		this->setIsMoving(true);
		this->setVelocity(Vec2(-DEFAULT_SPEED, 0.0f));
	}); 
	bindKeyReleasedAction(EventKeyboard::KeyCode::KEY_D, [&](Node* character, float deltaTime){
		this->setIsMoving(false);
		this->setVelocity(Vec2(-DEFAULT_SPEED, 0.0f));
	});
	bindKeyReleasedAction(EventKeyboard::KeyCode::KEY_A, [&](Node* character, float deltaTime){
		this->setIsMoving(false);
		this->setVelocity(Vec2(DEFAULT_SPEED, 0.0f));
	});
	bindKeyPressedAction(EventKeyboard::KeyCode::KEY_SPACE, [&](Node* character, float deltaTime){
		if (character)
		{
			character->getPhysicsBody()->applyForce(Vec2(0.0f, 100.0f));
		}
		log("DO JUMPING!");
	});

	return true;
}

void PawnController::onEnter()
{
	CharacterController::onEnter();
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseDown = [&](EventMouse* event)
	{
		EventCustom e(EVENT_DO_ATTACK);
		AttackData attackData(event->getLocationInView(), this->_owner);
		e.setUserData((void*)&attackData);
		_eventDispatcher->dispatchEvent(&e);
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, _componentNode);
}

void PawnController::update(float delta)
{
	if (_isMoveing)
	{
		Vec2 pos = _owner->getPosition();
		_owner->setPosition(pos + this->getVelocity()*delta);
	}
}