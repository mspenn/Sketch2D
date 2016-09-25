/*
#ifndef __GEOMETRIC_SPRITE_H__
#define __GEOMETRIC_SPRITE_H__

#include "cocos2d.h"

class GeometricSprite;
class SpriteGenerator;

class GeometricSprite: public cocos2d::Sprite
{
public:
	

private:

};

class SpriteGenerator
{
public:
	GeometricSprite* gen();
	void dogen()
	{
		for (auto pDrawNode = _drawNodeList.begin(); pDrawNode != _drawNodeList.end(); pDrawNode++)
		{
			auto ds = *pDrawNode;
			if (!ds->empty())
			{
				DollarRecognizer::RecognitionResult result = ds->_recognizeResult;
				//if (result.score < 0.6){ continue; }
				Vec2 position = ds->getShapeCenter();
				if (0 == result.name.compare("StaticMark"))
				{
					continue;
				}

				auto sprite = Sprite::createWithTexture(ds->createTexture(), ds->contentRect());
				sprite->setPosition(position);
				sprite->setFlipY(true);

				sprite->addComponent(ds->makePhysicsBody());
				sprite->addComponent(PawnController::create());
				gameLayer->addChild(sprite);
			}
		}
	}
private:
	//std::list<DrawableSprite*>& _drawNodeList;

};

#endif
*/