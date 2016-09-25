#ifndef __CANVAS_LAYER_H__
#define __CANVAS_LAYER_H__

#include "cocos2d.h"
#include "DrawableSprite.h"

class CanvasLayer : public cocos2d::Layer
{
public:
	virtual void onEnter();
	//virtual void onExit();
	virtual void onMouseDown(cocos2d::EventMouse* event);
	virtual void onMouseMove(cocos2d::EventMouse* event);
	virtual void onMouseUp(cocos2d::EventMouse* event);

	CREATE_FUNC(CanvasLayer);
protected:
	DrawableSprite* _currentDrawNode;
	cocos2d::EventListenerMouse* _mouseListener;
	cocos2d::Size _canvasSize;

	bool _isDrawing;
	cocos2d::Vec2 _startDrawLocation;
};

class TrainingCanvasLayer :public CanvasLayer
{
public:
	virtual void onEnter()
	{ CanvasLayer::onEnter(); _currentDrawNode = DrawableSprite::create(); this->addChild(_currentDrawNode); }
	CREATE_FUNC(TrainingCanvasLayer);

	DrawableSprite* getDrawNode()const { return this->_currentDrawNode; };
};

#endif