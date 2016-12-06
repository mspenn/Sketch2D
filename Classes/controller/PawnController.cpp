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
	// CharacterController::init
	if (!CharacterController::init())
	{
		return false;
	}

	// register Key D Pressed Action
	bindKeyPressedAction(EventKeyboard::KeyCode::KEY_D, [&](Node* character, float deltaTime){
		this->setIsMoving(true);
		this->setVelocity(Vec2(DEFAULT_SPEED, 0.0f));
	});
	// register Key A Pressed Action
	bindKeyPressedAction(EventKeyboard::KeyCode::KEY_A, [&](Node* character, float deltaTime){
		this->setIsMoving(true);
		this->setVelocity(Vec2(-DEFAULT_SPEED, 0.0f));
	}); 
	// register Key D Released Action
	bindKeyReleasedAction(EventKeyboard::KeyCode::KEY_D, [&](Node* character, float deltaTime){
		this->setIsMoving(false);
		this->setVelocity(Vec2(-DEFAULT_SPEED, 0.0f));
	});
	// register Key A Released Action
	bindKeyReleasedAction(EventKeyboard::KeyCode::KEY_A, [&](Node* character, float deltaTime){
		this->setIsMoving(false);
		this->setVelocity(Vec2(DEFAULT_SPEED, 0.0f));
	});
	// register Key Space Pressed Action
	bindKeyPressedAction(EventKeyboard::KeyCode::KEY_SPACE, [&](Node* character, float deltaTime){
		ActionInterval* jumpBy = JumpBy::create(4, Vec2::ZERO, 100, 1);
		Action* jumpAction = Sequence::create(jumpBy, NULL);
		character->runAction(jumpAction);
	});

	return true;
}

void PawnController::onEnter()
{
	CharacterController::onEnter();

	// initialize mouse listener
	auto mouseListener = EventListenerMouse::create();

	// register mouse down action
	mouseListener->onMouseDown = [&](EventMouse* event)
	{
		EventCustom e(EVENT_DO_ATTACK);
		AttackData attackData(event->getLocationInView(), this->_owner);
		e.setUserData((void*)&attackData);
		_eventDispatcher->dispatchEvent(&e);
	};

	// register event listener to event dispather
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, _componentNode);
}

void PawnController::update(float delta)
{
	// check is moving in every tick
	if (_isMoveing)
	{
		// update postion
		Vec2 pos = _owner->getPosition();
		_owner->setPosition(pos + this->getVelocity()*delta);
	}
}