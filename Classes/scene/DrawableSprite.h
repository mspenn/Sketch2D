#ifndef __DRAWABLE_SPRITE_H__
#define __DRAWABLE_SPRITE_H__

#include "cocos2d.h"
#include "gesture/GeometricRecognizer.h"

/**
 * Drawable Sprite
 * Draw what you like in this sprite
 * @see cocos2d::DrawNode
 */
class DrawableSprite : public cocos2d::DrawNode
{
public:
	DrawableSprite();

	/**
	 * Set Geometric Recognizer
	 * @param grn	a pointer to existing GeometricRecognizer instance
	 * @see DollarRecognizer::GeometricRecognizer
	 */
	void setGeoRecognizer(DollarRecognizer::GeometricRecognizer* grn)
	{
		this->_geoRecognizer = grn;
	}

	/**
	 * Get MultiStrokeGesture from current drawn path
	 * @param a empty MultiStrokeGesture reference to store current path
	 */
	DollarRecognizer::MultiStrokeGesture& getMultiStrokeGesture(DollarRecognizer::MultiStrokeGesture&);

	/**
	 * Recognize shape
	 * The average of all the points
	 */
	DollarRecognizer::RecognitionResult recognize();

	/**
	 * Redraw path
	 */
	void redraw();

	/**
	 * Add a line to path
	 * @param from	a point the line from
	 * @param to	a point the line to
	 */
	void addToPath(cocos2d::Vec2 from, cocos2d::Vec2 to);

	/**
	 * Get Bary Center/Centroid
	 * The average of all the points
	 */
	cocos2d::Vec2 getBaryCenter() const
	{
		return _baryCenter;
	}

	/**
	 * Get Shape Center
	 * The centroid of the rectangle container
	 */
	cocos2d::Vec2 getShapeCenter() const
	{
		return cocos2d::Vec2((_xMin + _xMax) / 2, (_yMin + _yMax) / 2);
	}

	/**
	 * Get Shape Path
	 * The centroid of the rectangle container
	 */
	const std::vector<cocos2d::Vec2>& getPath() const
	{
		return _path;
	}

	/**
	 * Create Texture from current path
	 * @return a pointer to Texture2D instance
	 * @see cocos2d::Texture2D
	 */
	cocos2d::Texture2D* createTexture();

	/**
	 * Get Content Rectangle
	 * @return content rect area
	 * @see cocos2d::Rect
	 */
	cocos2d::Rect contentRect()const
	{
		return cocos2d::Rect(_xMin, _yMin, _xMax - _xMin, _yMax - _yMin);
	}

	/**
	 * Set Reference Sprite
	 * @param ref	a pointer to a existing DrawableSprite instance
	 */
	void setReferenceSprite(DrawableSprite* ref)
	{
		this->_reference = ref;
	}

	/**
	 * Get Reference Sprite
	 * @return	reference sprite
	 */
	DrawableSprite* getReferenceSprite() const
	{
		return this->_reference;
	}

	/**
	 * Check if path is empty
	 * @return true if path is empty, false otherwise
	 */
	bool empty() const
	{
		return this->_path.empty();
	}

	/**
	 * Set Brush Color
	 * @param newColor	new color to be set
	 * @see cocos2d::Color4F
	 */
	void setBrushColor(const cocos2d::Color4F& newColor)
	{
		this->_brushColor = newColor;
	}

	/**
	 * Get Brush Color
	 * @return brush color
	 * @see cocos2d::Color4F
	 */
	cocos2d::Color4F getBrushColor() const
	{
		return this->_brushColor;
	}

	/**
	 * If this sprite contains the specified point
	 * @param point	point to be checked
	 * @return true if this sprite contains the specified point, false otherwise
	 */
	bool containsPoint(const cocos2d::Vec2& point);

	/**
	 * Static factory to create DrawableSprite object with non-parameters
	 */
	CREATE_FUNC(DrawableSprite);

	/**
	 * Static factory to create DrawableSprite object with Geometric Recognizer
	 * @param geometric recognizer reference
	 * @return DrawableSprite instance
	 */
	static DrawableSprite* createWithMultiStrokeGesture(const DollarRecognizer::MultiStrokeGesture&);

private:

	DollarRecognizer::GeometricRecognizer*	_geoRecognizer;				// pointer to GeometricRecognizer instance
	cocos2d::Vec2							_baryCenter;				// bary center of current shape
	std::vector<cocos2d::Vec2>				_path;						// store shape path
	float									_xMin, _xMax, _yMin, _yMax;	// content rectangle achors
	cocos2d::Color4F						_brushColor;				// brush color used to draw shapes
	DrawableSprite*							_reference;					// not used
};

#endif	/* __DRAWABLE_SPRITE_H__ */