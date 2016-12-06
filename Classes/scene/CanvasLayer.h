#ifndef __CANVAS_LAYER_H__
#define __CANVAS_LAYER_H__

#include "cocos2d.h"
#include "DrawableSprite.h"

/**
 * Canvas Layer for draw shapes
 */
class CanvasLayer : public cocos2d::Layer
{
public:

	/**
	 * Call when CanvasLayer enters the 'stage', override
	 * @see cocos2d::Node::onEnter
	 */
	virtual void onEnter();

	/**
	 * Call when mouse down
	 * @param event mouse event argument
	 * @see cocos2d::EventMouse
	 */
	virtual void onMouseDown(cocos2d::EventMouse* event);
	
	/**
	 * Call when mouse move
	 * @param event mouse event argument
	 * @see cocos2d::EventMouse
	 */
	virtual void onMouseMove(cocos2d::EventMouse* event);
	
	/**
	 * Call when mouse up
	 * @param event mouse event argument
	 * @see cocos2d::EventMouse
	 */
	virtual void onMouseUp(cocos2d::EventMouse* event);
	
	/**
	 * Static factory to create CanvasLayer object
	 */
	CREATE_FUNC(CanvasLayer);

protected:

	DrawableSprite* _currentDrawNode;				// current draw node
	cocos2d::EventListenerMouse* _mouseListener;	// mouse event listener
	cocos2d::Size _canvasSize;						// canvas size

	bool _isDrawing;								// drawing status
	cocos2d::Vec2 _startDrawLocation;				// start draw location
};

/**
 * Training Canvas Layer for draw shapes in training scene
 * with only one Draw Node
 */
class TrainingCanvasLayer :public CanvasLayer
{
public:

	/**
	 * Call when CanvasLayer enters the 'stage', override
	 * @see cocos2d::Node::onEnter
	 */
	virtual void onEnter()
	{ 
		CanvasLayer::onEnter(); 
		_currentDrawNode = DrawableSprite::create();
		this->addChild(_currentDrawNode);
	}

	/**
	 * Static factory to create TrainingCanvasLayer object
	 */
	CREATE_FUNC(TrainingCanvasLayer);

	/**
	 * Get current draw node
	 * @return a pointer to current DrawableSprite instance
	 */
	DrawableSprite* getDrawNode() const 
	{ 
		return this->_currentDrawNode;
	}
};

#endif	/* __CANVAS_LAYER_H__ */