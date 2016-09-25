#include "CharacterController.h"

USING_NS_CC;

CharacterController::CharacterController()
{
	// set default scheduler and actionManager
	_director = Director::getInstance();
	_eventDispatcher = _director->getEventDispatcher();
	_eventDispatcher->retain();
	_componentNode = Node::create();
}

CharacterController::~CharacterController()
{

}

bool CharacterController::init()
{
	if (!Component::init())
	{
		return false;
	}
	
	return true;
}

void CharacterController::onEnter()
{
	Component::onEnter();

	_keyboardListener = EventListenerKeyboard::create();
	_keyboardListener->onKeyPressed = CC_CALLBACK_2(CharacterController::onKeyPressed, this);
	_keyboardListener->onKeyReleased = CC_CALLBACK_2(CharacterController::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, _componentNode);

	_owner->addChild(this->_componentNode);
}

void CharacterController::onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event)
{
	auto p = _keyPressedActionMap.find(keyCode);
	if (p != _keyPressedActionMap.end())
	{
		float deltaTime = _director->getDeltaTime();
		DoAction pAction = p->second;
		pAction(_owner, deltaTime);
	}
}

void CharacterController::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event)
{
	auto p = _keyReleasedActionMap.find(keyCode);
	if (p != _keyReleasedActionMap.end())
	{
		float deltaTime = _director->getDeltaTime();
		DoAction pAction = p->second;
		pAction(_owner, deltaTime);
	}
}

void CharacterController::onExit()
{
	_eventDispatcher->removeEventListener(_keyboardListener);
	_owner->removeChild(this->_componentNode);
}

void CharacterController::setEventDispatcher(EventDispatcher* dispatcher)
{
	if (dispatcher != _eventDispatcher)
	{
		_eventDispatcher->removeEventListenersForTarget(_componentNode);
		CC_SAFE_RETAIN(dispatcher);
		CC_SAFE_RELEASE(_eventDispatcher);
		_eventDispatcher = dispatcher;
	}
}

void CharacterController::bindKeyPressedAction(EventKeyboard::KeyCode keyCode, DoAction action)
{
	auto p = _keyPressedActionMap.find(keyCode);
	if (p != _keyPressedActionMap.end())
	{
		log("WARNING: KEY[%d] PRESSED MAPPING AREALDY EXISTS!", keyCode);
	}
	_keyPressedActionMap.insert(std::pair<EventKeyboard::KeyCode, DoAction>(keyCode, action));
	
}

void CharacterController::bindKeyReleasedAction(EventKeyboard::KeyCode keyCode, DoAction action)
{
	auto p = _keyReleasedActionMap.find(keyCode);
	if (p != _keyReleasedActionMap.end())
	{
		log("WARNING: KEY[%d] RELEASED MAPPING AREALDY EXISTS!", keyCode);
	}
	_keyReleasedActionMap.insert(std::pair<EventKeyboard::KeyCode, DoAction>(keyCode, action));

}