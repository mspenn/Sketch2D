#include "CharacterController.h"

USING_NS_CC;

CharacterController::CharacterController()
{
	// set default scheduler and actionManager
	_director = Director::getInstance();

	// set defualt event dispatcher
	_eventDispatcher = _director->getEventDispatcher();
	_eventDispatcher->retain();

	// place holder node
	_componentNode = Node::create();
}

void CharacterController::onEnter()
{
	Component::onEnter();

	// create key boadr listener
	_keyboardListener = EventListenerKeyboard::create();
	_keyboardListener->onKeyPressed = CC_CALLBACK_2(CharacterController::onKeyPressed, this);
	_keyboardListener->onKeyReleased = CC_CALLBACK_2(CharacterController::onKeyReleased, this);
	
	// register key board listener to event dispatcher,
	// actually event is signaled by componentNode
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, _componentNode);

	// add place holder node to owner
	_owner->addChild(this->_componentNode);
}

void CharacterController::onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event)
{
	// query keyCode-Action map
	auto p = _keyPressedActionMap.find(keyCode);
	if (p != _keyPressedActionMap.end())
	{
		// handle event when Action is registered
		float deltaTime = _director->getDeltaTime();
		DoAction pAction = p->second;
		pAction(_owner, deltaTime);
	}
}

void CharacterController::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event)
{
	// query keyCode-Action map
	auto p = _keyReleasedActionMap.find(keyCode);
	if (p != _keyReleasedActionMap.end())
	{
		// handle event when Action is registered
		float deltaTime = _director->getDeltaTime();
		DoAction pAction = p->second;
		pAction(_owner, deltaTime);
	}
}

void CharacterController::onExit()
{
	// remove key board listener
	_eventDispatcher->removeEventListener(_keyboardListener);
	// dettach place holder node
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
	// add keyCode-Action pair to map
	auto p = _keyPressedActionMap.find(keyCode);
	if (p != _keyPressedActionMap.end())
	{
		log("WARNING: KEY[%d] PRESSED MAPPING AREALDY EXISTS!", keyCode);
	}
	_keyPressedActionMap.insert(std::pair<EventKeyboard::KeyCode, DoAction>(keyCode, action));
	
}

void CharacterController::bindKeyReleasedAction(EventKeyboard::KeyCode keyCode, DoAction action)
{
	// add keyCode-Action pair to map
	auto p = _keyReleasedActionMap.find(keyCode);
	if (p != _keyReleasedActionMap.end())
	{
		log("WARNING: KEY[%d] RELEASED MAPPING AREALDY EXISTS!", keyCode);
	}
	_keyReleasedActionMap.insert(std::pair<EventKeyboard::KeyCode, DoAction>(keyCode, action));

}