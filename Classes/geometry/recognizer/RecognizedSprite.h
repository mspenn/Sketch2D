#ifndef __RECOGNIZED_SPRITE_H__
#define __RECOGNIZED_SPRITE_H__

#include "cocos2d.h"
#include "gesture/GeometricRecognizer.h"
#include "scene/DrawableSprite.h"
#include "resource/Resources.h"

#define DEFAULT_PRIORITY 5

/**
 * Recognized Sprite, a drawable sprite recognized by dollar recognizer
 */
struct RecognizedSprite
{
	// Constructor
	RecognizedSprite(const DollarRecognizer::RecognitionResult& result, DrawableSprite* drawNode)
	:_result(result)
	, _drawNode(drawNode)
	, _priority(DEFAULT_PRIORITY)
	{
	}

	/**
	 * Get Geometric Type
	 * @return Geometric type string
	 */
	inline std::string getGeometricType() const
	{
		if (!_result.name.empty())
		{
			int idx = _result.name.find_first_of(GEOMETRY_SAMPLE_SEPERATOR);
			return _result.name.substr(0, idx);
		}
		return _result.name; 
	}

	/**
	 * Check type if equals given type name
	 * @param typename	given geometric type name
	 * @return Geometric type string
	 */
	inline bool isType(const std::string& typeName) const
	{
		return 0 == typeName.compare(this->getGeometricType());
	}

	/**
	 * Override Operator <, compare with priority
	 * @return true if priority small than given one, false otherwise
	 */
	inline bool operator < (const RecognizedSprite& r) const
	{
		return this->_priority < r._priority;
	}

	/**
	 * Override Operator >, compare with priority
	 * @return true if priority large than given one, false otherwise
	 */
	inline bool operator > (const RecognizedSprite& r) const
	{
		return this->_priority > r._priority;
	}

	DollarRecognizer::RecognitionResult _result;	// recognize result given by dollar recognizer
	DrawableSprite*						_drawNode;	// a pointer to a target drawable sprite object
	int									_priority;	// priority
};

struct RecognizedSpriteLessCmp
{	
	/**
	 * Override Operator (), compare with priority
	 * Compare function for sort 
	 * @return true if priority small than given one, false otherwise
	 */
	inline bool operator() (const RecognizedSprite* a, const RecognizedSprite* b) const
	{
		return a->_priority < b->_priority;
	}
};

#endif	/* __RECOGNIZED_SPRITE_H__ */