#ifndef __GEOMETRIC_MATH_H__
#define __GEOMETRIC_MATH_H__

#include "cocos2d.h"

/**
 * Perpendicular Distance in 2D space
 * The distance from a point to a line is 
 * the shortest distance from a fixed point to any point on a fixed line in Euclidean geometry
 * @param p		fixed point
 * @param begin	the first point in the fixed line
 * @param end	the last point in the fixed line
 * @return the perpendicular distance
 */
float PerpendicularDistance(cocos2d::Vec2 p, cocos2d::Vec2 begin, cocos2d::Vec2 end);

/**
 * Ramer-Douglas-Peucker Algorithm
 * Reducing the number of points in a curve that is approximated by a series of points
 * @param points	points to be resampled
 * @param epsilon	user-defined, if the point furthest from the line segment is greater 
 *					than epsilon from the approximation then that point must be kept. 
 * @param result	points after resmapled
 */
void RamerDouglasPeucker(std::vector<cocos2d::Vec2>& points, float epsilon, std::vector<cocos2d::Vec2>& result);

/**
 * Euclidean Distance
 * @param a	point A
 * @param b	point B
 * @param result euclidean distance between point A and point B
 */
float EuclideanDistance(const cocos2d::Vec2& a, const cocos2d::Vec2& b);

/**
 * Manhattan Distance
 * @param a	point A
 * @param b	point B
 * @param result manhattan/city block distance between point A and point B
 */
float ManhattanDistance(const cocos2d::Vec2& a, const cocos2d::Vec2& b);

/**
 * Compare 2 points by anti-clockwise order, relative to the zero origin
 * @param a	point A
 * @param b	point B
 * return true if a is in anti-clockwise direction relative to b
 */
bool PointAntiClockwiseComparator(const cocos2d::Vec2& a, const cocos2d::Vec2& b);

/**
 * Compare 2 points by clockwise order, relative to the zero origin
 * @param a	point A
 * @param b	point B
 * return true if a is in clockwise direction relative to b
 */
bool PointClockwiseComparator(const cocos2d::Vec2& a, const cocos2d::Vec2& b);

/**
 * Calculate Intersection Rectangle Area between Rect A and Rect B
 * @param a	Rect A
 * @param b	Rect B
 * @see cocos2d::Rect
 * @return Intersection Rectangle
 */
cocos2d::Rect Intersection(const cocos2d::Rect& a, const cocos2d::Rect& b);

/**
 * Calculate Convex Hull
 * @param points	orignal points, with random shape
 * @param result	convex hull shape points 	
 */
void ConvexHull(std::vector<cocos2d::Vec2>& points, std::vector<cocos2d::Vec2>& result);

#endif	/* __GEOMETRIC_MATH_H__ */