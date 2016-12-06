#ifndef __CHARACTER_CONTROLLER_H__
#define __CHARACTER_CONTROLLER_H__

#include "cocos2d.h"

/**
 * Function Pointer for event callback DoAction
 * void (*DoAction)(cocos2d::Node*, float);
 * @param Node* pointer to sprite
 * @param float delta/elapsed time from previous tick
 * @return void
 */
typedef std::function<void(cocos2d::Node*, float)> DoAction;

/**
 * Character Controller
 * Responsed to user keyboard input event
 * It can be attached to cocos2d::Nodes
 * @see cocos2d::Component
 */
class CharacterController :public cocos2d::Component
{
public:

	/**
	 * Call when CharacterController enter the "stage", override
	 * @see cocos2d::Component::onEnter
	 */
	virtual void onEnter();

	/**
	 * Call when CharacterController enter the "stage", override
	 * @see cocos2d::Component::onExit
	 */
	virtual void onExit();

	// Constructor
	CharacterController();

	/**
	 * Set event dispatcher for scene.
	 * @param dispatcher The event dispatcher of scene.
	 */
	virtual void setEventDispatcher(cocos2d::EventDispatcher* dispatcher);

	/**
	 * Get the event dispatcher of scene.
	 * @return The event dispatcher of scene.
	 */
	virtual cocos2d::EventDispatcher* getEventDispatcher() const { return _eventDispatcher; };

	/**
	 * Bind key with action, action will be called when key pressed
	 * @param keyCode	key ascii code
	 * @param action	action callback
	 * @see DoAction
	 */
	void bindKeyPressedAction(cocos2d::EventKeyboard::KeyCode keyCode, DoAction action);

	/**
	 * Bind key with action, action will be called when key released
	 * @param keyCode	key ascii code
	 * @param action	action callback
	 * @see DoAction
	 */
	void bindKeyReleasedAction(cocos2d::EventKeyboard::KeyCode keyCode, DoAction action);

	/**
	 * Static factor for create CharacterController instance, with auto released manangement
	 */
	CREATE_FUNC(CharacterController);

protected:

	/**
	 * Call when key pressed
	 * @param KeyCode key ascii code
	 * @see cocos2d::EventKeyboard::KeyCode
	 * @param event	a pointer to event
	 */
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event);

	/**
	 * Call when key released
	 * @param KeyCode key ascii code
	 * @see cocos2d::EventKeyboard::KeyCode
	 * @param event	a pointer to event
	 */
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event);

	cocos2d::Director* _director;            			// cached director pointer to improve rendering performance
	cocos2d::EventDispatcher* _eventDispatcher;  		// event dispatcher used to dispatch all kinds of events

	cocos2d::Node* _componentNode;						// place holder component node for event dispatcher

	cocos2d::EventListenerKeyboard* _keyboardListener;	// key board listener for user input

	std::map<cocos2d::EventKeyboard::KeyCode, DoAction> _keyPressedActionMap;	// key-action map for key pressed
	std::map<cocos2d::EventKeyboard::KeyCode, DoAction> _keyReleasedActionMap;	// key-action map for key released
};


#endif	/* __CHARACTER_CONTROLLER_H__ */