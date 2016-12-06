#ifndef __GEOMETRIC_PHYSICS_H__
#define __GEOMETRIC_PHYSICS_H__

#include "cocos2d.h"
#include "scene/DrawableSprite.h"

/**
* Sprite Joint Constraint, used in physics body
*/
struct SpriteJoint
{
	DrawableSprite* _sprite;	// a pointer to existing DrawableSprite object
	cocos2d::Vec2 _loc;			// relative location offset to sprite
};

/**
* Jonints List
*/
typedef vector<DrawableSprite*> Joints;
typedef vector<Joints> JointsList;

/**
 * Make polygon shape by default
 * Auto detect a approximate polygon shape and make convex hull
 * @param path			original polygon shape/stroke path		
 * @param baryCenter	centroid of polygon shape
 * @return				a approximate convex hull with points resampled, sorted by clockwise
 */
vector<cocos2d::Vec2> makePolygonShape(const vector<cocos2d::Vec2>& path, cocos2d::Vec2 baryCenter);

/**
 * Make polygon shape by default
 * Auto detect a approximate polygon shape and make a convex physics body
 * @param drawableSprite	a pointer to a existing drawable sprite object
 * @return					a pointer to PhysicsBody object which has a approximate convex shape
 * @see DrawableSprite
 * @see cocos2d::PhysicsBody
 */
cocos2d::PhysicsBody* makePhysicsBodyAsPolygon(DrawableSprite* drawableSprite);

/**
 * Make polygon shape with triangulation
 * Auto detect a approximate polygon shape and make a physics body
 * with triangulation, Concave polygon shape is also supported 
 * @param drawableSprite	a pointer to a existing drawable sprite object
 * @return					a pointer to PhysicsBody object which has a approximate shape
 * @see DrawableSprite
 * @see cocos2d::PhysicsBody
 */
cocos2d::PhysicsBody* makePhysicsBodyAsPolygonWithTriangulation(DrawableSprite* drawableSprite);

/**
 * Make polygon shape as ball/cycle shape
 * @param drawableSprite	a pointer to a existing drawable sprite object
 * @return					a pointer to PhysicsBody object which has a approximate shape
 * @see DrawableSprite
 * @see cocos2d::PhysicsBody
*/
cocos2d::PhysicsBody* makePhysicsBodyAsBall(DrawableSprite* drawableSprite);

/**
 * Make polygon shape as triangle shape
 * @param drawableSprite	a pointer to a existing drawable sprite object
 * @return					a pointer to PhysicsBody object which has a approximate shape
 * @see DrawableSprite
 * @see cocos2d::PhysicsBody
 */
cocos2d::PhysicsBody* makePhysicsBodyAsTriangle(DrawableSprite* drawableSprite);

/**
 * Make polygon shape as box/rectangle shape
 * @param drawableSprite	a pointer to a existing drawable sprite object
 * @return					a pointer to PhysicsBody object which has a approximate shape
 * @see DrawableSprite
 * @see cocos2d::PhysicsBody
 */
cocos2d::PhysicsBody* makePhysicsBodyAsBox(DrawableSprite* drawableSprite);

/**
 * Make polygon shape as groove shape
 * @param drawableSprite	a pointer to a existing drawable sprite object
 * @return					a pointer to PhysicsBody object which has a approximate shape
 * @see DrawableSprite
 * @see cocos2d::PhysicsBody
 */
cocos2d::PhysicsBody* makePhysicsBodyAsGroove(DrawableSprite* drawableSprite);

/**
 * Default snap epsilon
 * Snap to another node by compare the metric value with epsilon
 */
#define DEFAULT_SNAP_EPSILON 0.8f

/**
 * Check if Sprite A is can snaped to Sprite B subject to epsilon
 * @param A			a pointer to a existing drawable sprite object
 * @param B			a pointer to a existing drawable sprite object
 * @param epsilon	snap epsilon 	
 * @see DrawableSprite
 * @return true if a can snap to b, false otherwise
 */
bool IsSnapedTo(DrawableSprite* a, DrawableSprite* b, float epsilon = DEFAULT_SNAP_EPSILON);

/**
 * Initialize physics body colison mask
 */
void InitGeometricPhysicsMask();

#endif	/* __GEOMETRIC_PHYSICS_H__ */