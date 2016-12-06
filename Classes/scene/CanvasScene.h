#ifndef __CANVAS_SCENE_H__
#define __CANVAS_SCENE_H__

#include "cocos2d.h"
#include "CanvasLayer.h"
#include "DrawableSprite.h"
#include "geometry/recognizer/GeometricRecognizerNode.h"
#include "geometry/handler/CommandHandler.h"

class CanvasScene;
class GameCanvasLayer;
class GameLayer;
class ToolLayer;

// Event
#define EVENT_RECOGNIZE_SUCCESS			"onRecognizeSuccess"
#define EVENT_RECOGNIZE_FAILED			"onRecognizeFailed"
#define EVENT_SIMULATE_START			"onSimulateStart"
#define EVENT_SIMULATE_STOP				"onSimulateStop"
#define EVENT_TOGGLE_PHYSICS_DEBUG_MODE	"onTogglePhysicsDebugMode"

class CanvasScene : public cocos2d::Scene
{
public:
	CanvasScene();

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	// a selector callback
	void startSimulate();
	void stopSimulate();
	//void menuCloseCallback(cocos2d::Ref* pSender);
	
	bool toggleDebugDraw();

	// implement the "static create()" method manually
	CREATE_FUNC(CanvasScene);
	
protected:
	cocos2d::Sprite* makeBall(cocos2d::Vec2 point, float radius, cocos2d::PhysicsMaterial material = cocos2d::PHYSICSBODY_MATERIAL_DEFAULT);
	cocos2d::Sprite* makeBox(cocos2d::Vec2 point, cocos2d::Size size, int color = 0, cocos2d::PhysicsMaterial material = cocos2d::PHYSICSBODY_MATERIAL_DEFAULT);
	cocos2d::Sprite* makeTriangle(cocos2d::Vec2 point, cocos2d::Size size, int color = 0, cocos2d::PhysicsMaterial material = cocos2d::PHYSICSBODY_MATERIAL_DEFAULT);

private:
	GameCanvasLayer* _canvasLayer;
	ToolLayer* _toolLayer;
	GameLayer* _gameLayer;

	cocos2d::EventListenerKeyboard* _keyboardListener;
	
	bool _debugDraw;
};

class GameCanvasLayer :public CanvasLayer
{
public:
	GameCanvasLayer() :_jointMode(false){}
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual void onMouseDown(cocos2d::EventMouse* event);
	void save();
	void recognize();
	void redrawCurrentNode();
	void train();
	void removeUnrecognizedSprite(DrawableSprite* target);
	void startGameSimulation();
	void stopGameSimulation();
	DrawableSprite* switchToNewDrawNode();
	GameLayer* createGameLayer();
	CREATE_FUNC(GameCanvasLayer);
private:
	bool _jointMode;
	std::list<DrawableSprite*> _drawNodeList;
	DrawSpriteResultMap _drawNodeResultMap;

	GeometricRecognizerNode* _geoRecognizer;

	PreCommandHandlerFactory _preCmdHandlers;

};

class PopupLayer : public cocos2d::Layer
{
public:
	virtual bool init();
	CREATE_FUNC(PopupLayer);
};

class ToolLayer : public cocos2d::Layer
{
public:
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	CREATE_FUNC(ToolLayer);

	void startSimulateCallback(cocos2d::Ref* pSender);
	void stopSimulateCallback(cocos2d::Ref* pSender);
	void startJointModeCallback(cocos2d::Ref* pSender);
	void stopJointModeCallback(cocos2d::Ref* pSender);
	void toggleDebugMode(bool isDebug);

	void setParentScene(CanvasScene* canvasScene){ this->_canvasScene = canvasScene; }
	void onRecognizeSuccess(cocos2d::EventCustom* event);
	void onRecognizeFailed(cocos2d::EventCustom* event);
private:
	cocos2d::EventListenerCustom* _recognizeSuccessListener;
	cocos2d::EventListenerCustom* _recognizeFailedListener;

	cocos2d::Sprite*	_spriteAssistant;
	cocos2d::Label*	_labelHint;
	cocos2d::MenuItemImage* _menuStartSimulate;
	cocos2d::MenuItemImage* _menuStopSimulate;
	cocos2d::MenuItemImage* _menuStartJoint;
	cocos2d::MenuItemImage* _menuStopJoint;
	cocos2d::MenuItemImage* _menuStartDebug;

	CanvasScene* _canvasScene;
};

class GameLayer : public cocos2d::Layer
{
public:

	GameLayer(std::list<DrawableSprite*>&, DrawSpriteResultMap&);

	virtual bool init();

	virtual void onEnter();

	virtual void onExit();

	static GameLayer *GameLayer::create(list<DrawableSprite*>&, DrawSpriteResultMap&, cocos2d::Scene*);
	//CREATE_FUNC(GameLayer);
//	void setDrawNodeList(std::list<DrawableSprite*>*drawNodeList){ this->_drawNodeList = drawNodeList; }
//	void setDrawNodeResultMap(DrawSpriteResultMap*drawNodeResultMap){ this->_drawNodeResultMap = drawNodeResultMap; }
private:
	std::list<DrawableSprite*>& _drawNodeList;
	DrawSpriteResultMap&		_drawNodeResultMap;
	GenSpriteResultMap			_genSpriteResultMap;
	PostCommandHandlerFactory	_postCmdHandlers;
};

#endif // __CANVAS_SCENE_H__