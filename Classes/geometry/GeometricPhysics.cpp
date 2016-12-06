#include "geometry/GeometricPhysics.h"
#include "geometry/GeometricMath.h"
#include "geometry/delaunay/Clarkson-Delaunay.h"

USING_NS_CC;

int PHYSICS_BODY_CONTACT_MASK	= 0x0001;
int PHYSICS_BODY_CATEGORY_MASK	= 0x0001;

#define MAKE_PHYSICS_BODY_MASK(physicsBody)\
do{\
	(physicsBody)->setContactTestBitmask(PHYSICS_BODY_CONTACT_MASK); \
	PHYSICS_BODY_CONTACT_MASK = PHYSICS_BODY_CONTACT_MASK << 1; \
	(physicsBody)->setCategoryBitmask(PHYSICS_BODY_CATEGORY_MASK); \
	PHYSICS_BODY_CATEGORY_MASK = PHYSICS_BODY_CATEGORY_MASK << 1; \
}while(0)

vector<Vec2> makePolygonShape(const vector<Vec2>& strokePath, Vec2 baryCenter)
{
#define EPSILON 10.0f
	int cnt = 0;
	vector<Vec2> path, poly;
	// calculate stroke path relative location to it's bary center/centroid 
	for (auto p = strokePath.begin(); p != strokePath.end(); p++)
	{
		path.push_back(*p - baryCenter);
	}
	Vec2 baryOffset;

	// do until the number of points is no longer changed
	do{
		cnt = path.size();
		poly.clear();

		// simplified/reduce path by RamerDouglasPeucker algorithm 
		RamerDouglasPeucker(path, EPSILON, poly);
		if (cnt == poly.size())break;

		// calculate convex hull for shape
		vector<Vec2> res;
		ConvexHull(poly, res);
		poly = res;

		// recalculate bary for new shape
		Vec2 offset, nbary;
		for (auto p = poly.begin(); p != poly.end(); p++)nbary += *p;
		nbary = nbary / poly.size(); offset = baryOffset - nbary;
		for (auto p = poly.begin(); p != poly.end(); p++)*p = *p + offset;
		baryOffset = nbary;

		// sort point by clock wise
		sort(poly.begin(), poly.end(), PointClockwiseComparator);
		path = poly;
	} while (cnt != poly.size());

	if (!poly.empty())
	{
		// restore with orignal bary center
		for (auto p = poly.begin(); p != poly.end(); p++)
		{
			(*p) = (*p) + baryOffset;
		}
	}
	cocos2d::log("make polygon shape end!");

	return poly;
}

PhysicsBody* makePhysicsBodyAsPolygon(DrawableSprite* drawableSprite)
{
	auto _baryCenter = drawableSprite->getBaryCenter();
	Vec2 offset = drawableSprite->getShapeCenter() - _baryCenter;
	// calculate a new polygon with approximate shape and less points
	auto poly = makePolygonShape(drawableSprite->getPath(), _baryCenter);

	// copy polygon
	Vec2* vers = new Vec2[poly.size()];
	for (int i = 0; i < poly.size(); i++)vers[i] = poly[i];

	// make physics body by cocos2d::PhysicsBody::createPolygon
	cocos2d::log("make polygon physics body start!");
	auto physicsBody = PhysicsBody::createPolygon(vers, poly.size(), PHYSICSBODY_MATERIAL_DEFAULT, -offset);
	cocos2d::log("make polygon physics body end!");

	// delete vertex array
	delete[] vers;

	// get content rectangle
	auto rect = drawableSprite->contentRect();
	float _xMin = rect.getMinX(), _xMax = rect.getMaxX(), _yMin = rect.getMinY(), _yMax = rect.getMaxY();

	// set mass by rectangle area size
	physicsBody->setMass((_xMax - _xMin)*(_yMax - _yMin));
	// set linear damping
	physicsBody->setLinearDamping(0.3f);
	// set physics body mask
	MAKE_PHYSICS_BODY_MASK(physicsBody);

	return physicsBody;
}

PhysicsBody* makePhysicsBodyAsPolygonWithTriangulation(DrawableSprite* drawableSprite)
{
	auto _baryCenter = drawableSprite->getBaryCenter();
	Vec2 offset = drawableSprite->getShapeCenter() - _baryCenter;
	// calculate a new polygon with approximate shape and less points
	auto poly = makePolygonShape(drawableSprite->getPath(), _baryCenter);
	
	// copy polygon
	float* ivers = new float[poly.size() * 2];
	for (unsigned int i = 0; i < poly.size(); i++)
		ivers[2 * i] = poly[i].x, ivers[2 * i + 1] = poly[i].y;

	WORD *triangleIndexList = nullptr;   // OUTPUT, this does not need initialization
	int *testPointsXY = nullptr;         // INPUT, I fill this with random points for testing
	int g_numTestPoints = poly.size();   // INPUT, the number of points.
	// I set this to 16 at the top of the test_triangulation.cpp, 
	// and you can adjust its value as the program is running by
	// pressing i or d on the keyboard
	int numTriangleVertices;   // OUTPUT, this does not need initialization

	triangleIndexList = BuildTriangleIndexList((void*)ivers,	// The array of points
		1.0f,													// Multiplication factor. For testing: max random value
		g_numTestPoints,										// The number of points
		2,														// 2, the list is XY points, not XYZ points
		1,														// 1, because I want the triangles clockwise
		&numTriangleVertices);
	Vec2* vers = new Vec2[numTriangleVertices];
	for (int i = 0; i < numTriangleVertices; i++)
	{
		int idx = triangleIndexList[i];
		vers[i] = Vec2(ivers[2 * idx], ivers[2 * idx + 1]);
	}
	free(triangleIndexList);

	// make physics body by cocos2d::PhysicsBody::createPolygon
	cocos2d::log("make polygon physics body start!");
	auto physicsBody = PhysicsBody::createPolygon(vers, numTriangleVertices, PHYSICSBODY_MATERIAL_DEFAULT, -offset);
	cocos2d::log("make polygon physics body end!");
	
	// delete vertex array
	delete[] ivers;
	delete[] vers;
	
	// get content rectangle
	auto rect = drawableSprite->contentRect();
	float _xMin = rect.getMinX(), _xMax = rect.getMaxX(), _yMin = rect.getMinY(), _yMax = rect.getMaxY();
	
	// set mass by rectangle area size
	physicsBody->setMass((_xMax - _xMin)*(_yMax - _yMin));
	// set linear damping
	physicsBody->setLinearDamping(0.3f);
	// set physics body mask
	MAKE_PHYSICS_BODY_MASK(physicsBody);
	
	return physicsBody;
}

PhysicsBody* makePhysicsBodyAsBall(DrawableSprite* drawableSprite)
{
	float radiusMax = 0;
	float radiusMin = std::numeric_limits<float>::max();
	auto _path = drawableSprite->getPath();
	auto _baryCenter = drawableSprite->getBaryCenter();

	// calculate the max/min raduis for sprite with ball shape
	for (auto p = _path.begin(); p != _path.end(); p++)
	{
		float dx = p->x - _baryCenter.x;
		float dy = p->y - _baryCenter.y;
		float d = sqrt(dx*dx + dy*dy);
		radiusMax = MAX(d, radiusMax);
		radiusMin = MIN(d, radiusMin);
	}

	// adjust shape center for physics body offset
	Vec2 offset = drawableSprite->getShapeCenter() - _baryCenter;
	float radius = (radiusMax + radiusMin) / 2;

	// make physics body by cocos2d::PhysicsBody::createCircle
	auto physicsBody = PhysicsBody::createCircle(radius, PHYSICSBODY_MATERIAL_DEFAULT, offset);
	
	// set mass by cycle area size
	physicsBody->setMass(radius*radius*3.14f);
	// set linear damping
	physicsBody->setLinearDamping(0.3f);
	// set physics body mask
	MAKE_PHYSICS_BODY_MASK(physicsBody);
	
	return physicsBody;
}

PhysicsBody* makePhysicsBodyAsTriangle(DrawableSprite* drawableSprite)
{
	// adjust shape center for physics body offset
	auto _baryCenter = drawableSprite->getBaryCenter();
	Vec2 offset = drawableSprite->getShapeCenter() - _baryCenter;
	
	// get content rectangle
	auto rect = drawableSprite->contentRect();
	float _xMin = rect.getMinX(), _xMax = rect.getMaxX(), _yMin = rect.getMinY(), _yMax = rect.getMaxY();

	// calculate the triangle points
	Vec2 vers[] = {
		Vec2(0, (_yMax - _yMin) / 2) - offset,
		Vec2((_xMax - _xMin) / 2, (-_yMax + _yMin) / 2) - offset,
		Vec2((-_xMax + _xMin) / 2, (-_yMax + _yMin) / 2) - offset
	};
	
	// make physics body by cocos2d::PhysicsBody::createPolygon
	auto physicsBody = PhysicsBody::createPolygon(vers, 3, PHYSICSBODY_MATERIAL_DEFAULT, offset);

	// set mass by triangle area size
	physicsBody->setMass((_xMax - _xMin)*(_yMax - _yMin) / 2);
	// set linear damping
	physicsBody->setLinearDamping(0.3f);
	// set physics body mask
	MAKE_PHYSICS_BODY_MASK(physicsBody);

	return physicsBody;
}

PhysicsBody* makePhysicsBodyAsBox(DrawableSprite* drawableSprite)
{
	// adjust shape center for physics body offset
	auto _baryCenter = drawableSprite->getBaryCenter();
	Vec2 offset = drawableSprite->getShapeCenter() - _baryCenter;

	// get content rectangle
	auto rect = drawableSprite->contentRect();
	float _xMin = rect.getMinX(), _xMax = rect.getMaxX(), _yMin = rect.getMinY(), _yMax = rect.getMaxY();

	// make physics body by cocos2d::PhysicsBody::createBox
	auto physicsBody = PhysicsBody::createBox(Size(Vec2(_xMax - _xMin, _yMax - _yMin) - offset), PHYSICSBODY_MATERIAL_DEFAULT, offset);
	
	// set mass by rectangle area size
	physicsBody->setMass((_xMax - _xMin)*(_yMax - _yMin));
	// set linear damping
	physicsBody->setLinearDamping(0.3f);
	// set physics body mask
	MAKE_PHYSICS_BODY_MASK(physicsBody);

	return physicsBody;
}

PhysicsBody* makePhysicsBodyAsGroove(DrawableSprite* drawableSprite)
{
	// adjust shape center for physics body offset
	auto _baryCenter = drawableSprite->getBaryCenter();
	Vec2 offset = drawableSprite->getShapeCenter() - _baryCenter;

	// get content rectangle
	auto rect = drawableSprite->contentRect();
	float _xMin = rect.getMinX(), _xMax = rect.getMaxX(), _yMin = rect.getMinY(), _yMax = rect.getMaxY();
	float width = _xMax - _xMin, height = _yMax - _yMin;
	
	// make empty physics body
	auto physicsBody = PhysicsBody::create();
	// offset position
	physicsBody->setPositionOffset(-offset);
	// calculate groove points
	// -----      -----
	// |   |      |   |
	// | 1 |______| 3 |
	// |  /	       \  |
	// | /    2     \ |
	// ----------------
	Point vert1[4] = { 
		Point(-width / 2, height / 2), 
		Point(-width / 4, height / 2),
		Point(-width / 4, 0.0f), 
		Point(-width / 2, -height / 2) 
	};
	physicsBody->addShape(PhysicsShapePolygon::create(vert1, 4));

	Point vert2[4] = { 
		Point(-width / 2, -height / 2), 
		Point(-width / 4, 0.0f), 
		Point(width / 4, 0.0f), 
		Point(width / 2, -height / 2) 
	};
	physicsBody->addShape(PhysicsShapePolygon::create(vert2, 4));

	Point vert3[4] = { 
		Point(width / 2, -height / 2), 
		Point(width / 4, 0.0f), 
		Point(width / 4, height / 2), 
		Point(width / 2, height / 2) };
	physicsBody->addShape(PhysicsShapePolygon::create(vert3, 4));

	// set mass by rectangle area size
	physicsBody->setMass((_xMax - _xMin)*(_yMax - _yMin));
	// set physics body mask
	MAKE_PHYSICS_BODY_MASK(physicsBody);

	return physicsBody;
}

bool IsSnapedTo(DrawableSprite* a, DrawableSprite* b, float epsilon)
{
	Rect coverRect = a->contentRect();
	float coverArea = coverRect.size.width*coverRect.size.height;

	// get intersection area between sprite a and sprite b
	Rect intersection = Intersection(b->contentRect(), coverRect);
	float intersectArea = intersection.size.width*intersection.size.height;

	// if intersect and metric by intersect area ratio
	// spirte a can snap to sprite b if metric is large than epsilon (by intersection area over sprite a rectangle area)
	return coverArea != 0 && intersectArea / coverArea >= epsilon;
}

void InitGeometricPhysicsMask()
{
	// initialize physics body mask
	PHYSICS_BODY_CONTACT_MASK = 0x0001;
	PHYSICS_BODY_CATEGORY_MASK = 0x0001;
}