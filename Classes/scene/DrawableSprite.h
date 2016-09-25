#ifndef __DRAWABLE_SPRITE_H__
#define __DRAWABLE_SPRITE_H__

#include "cocos2d.h"
#include "gesture/GeometricRecognizer.h"

/*
enum NodeType
{
	CIRCLE = 0,
	RECTANGLE,
	TRIANGLE,
	SPRING
};*/

class DrawableSprite : public cocos2d::DrawNode
{
public:
	DrawableSprite();
	//virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	//void setNodeType(NodeType type){ this->_nodeType = type; }
	//NodeType getNodeType()const { return this->_nodeType; }
	void setGeoRecognizer(DollarRecognizer::GeometricRecognizer* grn){ this->_geoRecognizer = grn; }
	DollarRecognizer::MultiStrokeGesture& getMultiStrokeGesture(DollarRecognizer::MultiStrokeGesture&);

	DollarRecognizer::RecognitionResult recognize();

	void redraw();
	void addToPath(cocos2d::Vec2 from, cocos2d::Vec2 to);

	cocos2d::Vec2 getBaryCenter()const{ return _baryCenter; }
	cocos2d::Vec2 getShapeCenter() const{ return cocos2d::Vec2((_xMin + _xMax) / 2, (_yMin + _yMax) / 2); }
	const std::vector<cocos2d::Vec2>& getPath() const{ return _path; }

	cocos2d::Texture2D* createTexture();
	cocos2d::Rect contentRect()const{ return cocos2d::Rect(_xMin, _yMin, _xMax - _xMin, _yMax - _yMin); }

	void setReferenceSprite(DrawableSprite* ref){ this->_reference = ref; }
	DrawableSprite* getReferenceSprite()const{ return this->_reference; }

	bool empty()const{ return this->_path.empty(); }

	void setBrushColor(const cocos2d::Color4F& newColor){ this->_brushColor = newColor; }
	cocos2d::Color4F getBrushColor() const{ return this->_brushColor; }
	bool containsPoint(const cocos2d::Vec2& point);
	//bool isSnappedTo(const DrawableSprite& aSprite);
	// create function
	CREATE_FUNC(DrawableSprite);
	static DrawableSprite* createWithMultiStrokeGesture(const DollarRecognizer::MultiStrokeGesture&);

private:
	
	DollarRecognizer::GeometricRecognizer* _geoRecognizer;
	cocos2d::Vec2 _baryCenter;
	std::vector<cocos2d::Vec2> _path;
	float _xMin, _xMax, _yMin, _yMax;
	//NodeType _nodeType;
	DrawableSprite* _reference;
	cocos2d::Color4F _brushColor;
};

#endif