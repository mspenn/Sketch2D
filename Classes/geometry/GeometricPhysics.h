#ifndef __GEOMETRIC_PHYSICS_H__
#define __GEOMETRIC_PHYSICS_H__

#include "cocos2d.h"
#include "scene/DrawableSprite.h"

struct SpriteJoint
{
	DrawableSprite* _sprite;
	cocos2d::Vec2 _loc;
};
typedef vector<DrawableSprite*> Joints;
typedef vector<Joints> JointsList;


vector<cocos2d::Vec2> makePolygonShape(const vector<cocos2d::Vec2>& _path, cocos2d::Vec2 _baryCenter);
cocos2d::PhysicsBody* makePhysicsBodyAsPolygon(DrawableSprite* drawableSprite);
cocos2d::PhysicsBody* makePhysicsBodyAsPolygonWithTriangulation(DrawableSprite* drawableSprite);
cocos2d::PhysicsBody* makePhysicsBodyAsBall(DrawableSprite* drawableSprite);
cocos2d::PhysicsBody* makePhysicsBodyAsTriangle(DrawableSprite* drawableSprite);
cocos2d::PhysicsBody* makePhysicsBodyAsBox(DrawableSprite* drawableSprite);

#define DEFAULT_SNAP_EPSILON 0.8f
bool IsSnapedTo(DrawableSprite* a, DrawableSprite* b, float epsilon = DEFAULT_SNAP_EPSILON);

#endif