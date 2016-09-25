#ifndef __RECOGNIZED_SPRITE_H__
#define __RECOGNIZED_SPRITE_H__

#include "cocos2d.h"
#include "gesture/GeometricRecognizer.h"
#include "scene/DrawableSprite.h"
#include "resource/Resources.h"

struct RecognizedSprite
{
	RecognizedSprite(const DollarRecognizer::RecognitionResult& result, DrawableSprite* drawNode)
	:_result(result)
	, _drawNode(drawNode)
	, _priority(5)
	{
	}
	DollarRecognizer::RecognitionResult _result;
	DrawableSprite* _drawNode;
	std::string getGeometricType() const
	{
		if (!_result.name.empty())
		{
			int idx = _result.name.find_first_of(GEOMETRY_SAMPLE_SEPERATOR);
			return _result.name.substr(0, idx);
		}
		return _result.name; 
	}
	bool isType(const std::string& typeName){ return 0 == typeName.compare(this->getGeometricType()); }
	int _priority;
	bool operator < (const RecognizedSprite& r){ return this->_priority < r._priority; }
	bool operator > (const RecognizedSprite& r){ return this->_priority > r._priority; }
};

struct RecognizedSpriteLessCmp
{
	bool operator()(const RecognizedSprite* a, const RecognizedSprite* b)
	{
		return a->_priority < b->_priority;
	}
};

#endif