#include "geometry/GeometricPhysics.h"
#include "geometry/GeometricMath.h"
#include "geometry/delaunay/Clarkson-Delaunay.h"

USING_NS_CC;

int PHYSICS_BODY_CONTACT_MASK = 0x0001;
#define MAKE_PHYSICS_BODY_MASK(physicsBody)\
do{\
	(physicsBody)->setContactTestBitmask(PHYSICS_BODY_CONTACT_MASK << 1); \
}while(0)

vector<Vec2> makePolygonShape(const vector<Vec2>& _path, Vec2 _baryCenter)
{
#define EPSILON 10.0f
	int cnt = 0;
	vector<Vec2> path, poly;
	for (auto p = _path.begin(); p != _path.end(); p++)
	{
		path.push_back(*p - _baryCenter);
	}
	Vec2 baryOffset;
	do{
		cnt = path.size();
		poly.clear();
		RamerDouglasPeucker(path, EPSILON, poly);
		if (cnt == poly.size())break;

		vector<Vec2> res;
		ConvexHull(poly, res);
		poly = res;
		// recalc bary
		Vec2 offset, nbary;
		for (auto p = poly.begin(); p != poly.end(); p++)nbary += *p;
		nbary = nbary / poly.size(); offset = baryOffset - nbary;
		for (auto p = poly.begin(); p != poly.end(); p++)*p = *p + offset;
		baryOffset = nbary;

		sort(poly.begin(), poly.end(), PointClockwiseComparator);
		path = poly;
	} while (cnt != poly.size());

	if (!poly.empty())
	{
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
	auto poly = makePolygonShape(drawableSprite->getPath(), _baryCenter);
	Vec2* vers = new Vec2[poly.size()];
	for (int i = 0; i < poly.size(); i++)vers[i] = poly[i];
	cocos2d::log("make polygon physics body start!");
	auto physicsBody = PhysicsBody::createPolygon(vers, poly.size(), PHYSICSBODY_MATERIAL_DEFAULT, -offset);
	cocos2d::log("make polygon physics body end!");
	delete[] vers;
	auto rect = drawableSprite->contentRect();
	float _xMin = rect.getMinX(), _xMax = rect.getMaxX(), _yMin = rect.getMinY(), _yMax = rect.getMaxY();
	physicsBody->setMass((_xMax - _xMin)*(_yMax - _yMin));
	MAKE_PHYSICS_BODY_MASK(physicsBody);
	return physicsBody;
}

PhysicsBody* makePhysicsBodyAsPolygonWithTriangulation(DrawableSprite* drawableSprite)
{
	auto _baryCenter = drawableSprite->getBaryCenter();
	Vec2 offset = drawableSprite->getShapeCenter() - _baryCenter;
	auto poly = makePolygonShape(drawableSprite->getPath(), _baryCenter);
	float* ivers = new float[poly.size() * 2];
	for (unsigned int i = 0; i < poly.size(); i++)
		ivers[2 * i] = poly[i].x, ivers[2 * i + 1] = poly[i].y;

	WORD *triangleIndexList = nullptr;   // OUTPUT, this does not need initialization
	int *testPointsXY = nullptr;         // INPUT, I fill this with random points for testing
	int g_numTestPoints = poly.size();       // INPUT, the number of points.
	// I set this to 16 at the top of the test_triangulation.cpp, 
	// and you can adjust its value as the program is running by
	// pressing i or d on the keyboard
	int numTriangleVertices;   // OUTPUT, this does not need initialization

	triangleIndexList = BuildTriangleIndexList((void*)ivers,       // The array of points
		1.0f,          // Multiplication factor. For testing: max random value
		g_numTestPoints,         // The number of points
		2,                      // 2: the list is XY points, not XYZ points
		1,                     // 1, because I want the triangles clockwise
		&numTriangleVertices);
	Vec2* vers = new Vec2[numTriangleVertices];
	for (int i = 0; i < numTriangleVertices; i++)
	{
		int idx = triangleIndexList[i];
		vers[i] = Vec2(ivers[2 * idx], ivers[2 * idx + 1]);
	}
	free(triangleIndexList);
	cocos2d::log("make polygon physics body start!");
	//AutoPolygon::
	auto physicsBody = PhysicsBody::createPolygon(vers, numTriangleVertices, PHYSICSBODY_MATERIAL_DEFAULT, -offset);
	cocos2d::log("make polygon physics body end!");
	delete[] ivers;
	delete[] vers;
	auto rect = drawableSprite->contentRect();
	float _xMin = rect.getMinX(), _xMax = rect.getMaxX(), _yMin = rect.getMinY(), _yMax = rect.getMaxY();
	physicsBody->setMass((_xMax - _xMin)*(_yMax - _yMin));
	MAKE_PHYSICS_BODY_MASK(physicsBody);
	return physicsBody;
}

PhysicsBody* makePhysicsBodyAsBall(DrawableSprite* drawableSprite)
{
	float radiusMax = 0;
	float radiusMin = std::numeric_limits<float>::max();
	auto _path = drawableSprite->getPath();
	auto _baryCenter = drawableSprite->getBaryCenter();
	for (auto p = _path.begin(); p != _path.end(); p++)
	{
		float dx = p->x - _baryCenter.x;
		float dy = p->y - _baryCenter.y;
		float d = sqrt(dx*dx + dy*dy);
		radiusMax = MAX(d, radiusMax);
		radiusMin = MIN(d, radiusMin);
	}
	Vec2 offset = drawableSprite->getShapeCenter() - _baryCenter;
	float radius = (radiusMax + radiusMin) / 2;
	auto physicsBody = PhysicsBody::createCircle(radius, PHYSICSBODY_MATERIAL_DEFAULT, offset);
	physicsBody->setMass(radius*radius*3.14f);
	MAKE_PHYSICS_BODY_MASK(physicsBody);
	return physicsBody;
}

PhysicsBody* makePhysicsBodyAsTriangle(DrawableSprite* drawableSprite)
{
	auto _baryCenter = drawableSprite->getBaryCenter();
	Vec2 offset = drawableSprite->getShapeCenter() - _baryCenter;
	auto rect = drawableSprite->contentRect();
	float _xMin = rect.getMinX(), _xMax = rect.getMaxX(), _yMin = rect.getMinY(), _yMax = rect.getMaxY();
	Vec2 vers[] = {
		Vec2(0, (_yMax - _yMin) / 2) - offset,
		Vec2((_xMax - _xMin) / 2, (-_yMax + _yMin) / 2) - offset,
		Vec2((-_xMax + _xMin) / 2, (-_yMax + _yMin) / 2) - offset
	};
	auto physicsBody = PhysicsBody::createPolygon(vers, 3, PHYSICSBODY_MATERIAL_DEFAULT, offset);
	physicsBody->setMass((_xMax - _xMin)*(_yMax - _yMin) / 2);
	MAKE_PHYSICS_BODY_MASK(physicsBody);
	return physicsBody;
}

PhysicsBody* makePhysicsBodyAsBox(DrawableSprite* drawableSprite)
{
	auto _baryCenter = drawableSprite->getBaryCenter();
	auto rect = drawableSprite->contentRect();
	float _xMin = rect.getMinX(), _xMax = rect.getMaxX(), _yMin = rect.getMinY(), _yMax = rect.getMaxY();
	Vec2 offset = drawableSprite->getShapeCenter() - _baryCenter;
	auto physicsBody = PhysicsBody::createBox(Size(Vec2(_xMax - _xMin, _yMax - _yMin) - offset), PHYSICSBODY_MATERIAL_DEFAULT, offset);
	physicsBody->setMass((_xMax - _xMin)*(_yMax - _yMin));
	MAKE_PHYSICS_BODY_MASK(physicsBody);
	return physicsBody;
}

PhysicsBody* makePhysicsBodyAsGroove(DrawableSprite* drawableSprite)
{
	auto _baryCenter = drawableSprite->getBaryCenter();
	auto rect = drawableSprite->contentRect();
	float _xMin = rect.getMinX(), _xMax = rect.getMaxX(), _yMin = rect.getMinY(), _yMax = rect.getMaxY();
	Vec2 offset = drawableSprite->getShapeCenter() - _baryCenter;
	auto physicsBody = PhysicsBody::createBox(Size(Vec2(_xMax - _xMin, _yMax - _yMin) - offset), PHYSICSBODY_MATERIAL_DEFAULT, offset);
	physicsBody->setMass((_xMax - _xMin)*(_yMax - _yMin));
	MAKE_PHYSICS_BODY_MASK(physicsBody);
	return physicsBody;
}

bool IsSnapedTo(DrawableSprite* a, DrawableSprite* b, float epsilon)
{
	Rect coverRect = a->contentRect();
	float coverArea = coverRect.size.width*coverRect.size.height;
	Rect intersection = Intersection(b->contentRect(), coverRect);
	float intersectArea = intersection.size.width*intersection.size.height;
	return coverArea != 0 && intersectArea / coverArea >= epsilon;
}