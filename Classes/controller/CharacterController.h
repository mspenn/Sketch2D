#ifndef __CHARACTER_CONTROLLER_H__
#define __CHARACTER_CONTROLLER_H__

#include "cocos2d.h"

// define character act type
/*#define ACT_FORWARD		"forward"
#define ACT_BACKWARD	"backward"
#define ACT_UPWARD		"upward"
#define ACT_DOWNWARD	"downward"
*/
typedef std::function<void(cocos2d::Node*, float)> DoAction;

class CharacterController :public cocos2d::Component
{
public:
	virtual bool init();
	
	virtual void onEnter();
	virtual void onExit();

	CharacterController();
	virtual ~CharacterController();

	/** Set event dispatcher for scene.
	*
	* @param dispatcher The event dispatcher of scene.
	*/
	virtual void setEventDispatcher(cocos2d::EventDispatcher* dispatcher);
	/** Get the event dispatcher of scene.
	*
	* @return The event dispatcher of scene.
	*/
	virtual cocos2d::EventDispatcher* getEventDispatcher() const { return _eventDispatcher; };

	void bindKeyPressedAction(cocos2d::EventKeyboard::KeyCode keyCode, DoAction action);
	void bindKeyReleasedAction(cocos2d::EventKeyboard::KeyCode keyCode, DoAction action);
	
	CREATE_FUNC(CharacterController);
protected:
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event);

	cocos2d::Director* _director;            //cached director pointer to improve rendering performance
	cocos2d::EventDispatcher* _eventDispatcher;  ///< event dispatcher used to dispatch all kinds of events

	cocos2d::Node* _componentNode;

	cocos2d::EventListenerKeyboard* _keyboardListener;

	std::map<cocos2d::EventKeyboard::KeyCode, DoAction> _keyPressedActionMap;
	std::map<cocos2d::EventKeyboard::KeyCode, DoAction> _keyReleasedActionMap;
};


#endif