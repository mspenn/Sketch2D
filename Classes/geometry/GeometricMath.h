#ifndef __GEOMETRIC_MATH_H__
#define __GEOMETRIC_MATH_H__

#include "cocos2d.h"

float PerpendicularDistance(cocos2d::Vec2 p, cocos2d::Vec2 begin, cocos2d::Vec2 end);

void RamerDouglasPeucker(std::vector<cocos2d::Vec2>& points, float epsilon, std::vector<cocos2d::Vec2>& result);

float EuclideanDistance(const cocos2d::Vec2& a, const cocos2d::Vec2& b);

float ManhattanDistance(const cocos2d::Vec2& a, const cocos2d::Vec2& b);

bool PointAntiClockwiseComparator(const cocos2d::Vec2& a, const cocos2d::Vec2& b);

bool PointClockwiseComparator(const cocos2d::Vec2& a, const cocos2d::Vec2& b);

cocos2d::Rect Intersection(const cocos2d::Rect& a, const cocos2d::Rect& b);

void ConvexHull(std::vector<cocos2d::Vec2>& points, std::vector<cocos2d::Vec2>& result);

#endif