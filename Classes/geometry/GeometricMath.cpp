#include "GeometricMath.h"

USING_NS_CC;
using namespace std;

#define SQR(x) ((x)*(x))

void RamerDouglasPeuckerSegment(vector<Vec2>& points, float epsilon, vector<Vec2>& result, int begin, int end);

float PerpendicularDistance(Vec2 p, Vec2 begin, Vec2 end)
{
	if (begin.x == end.x)
	{
		return abs(p.x - begin.x);
	}
	else
	{
		float k = (end.y - begin.y) / (end.x - begin.x);
		return abs(k*p.x - p.y + (begin.y - k*begin.x)) / sqrt(k*k + 1);
	}
}

void RamerDouglasPeucker(vector<Vec2>& points, float epsilon, vector<Vec2>& result)
{
	RamerDouglasPeuckerSegment(points, epsilon, result, 0, points.size() - 1);
}

void RamerDouglasPeuckerSegment(vector<Vec2>& points, float epsilon, vector<Vec2>& result, int begin, int end)
{
	if (points.empty()) return;

	// check the point with max perpendicular distance to point begin & end
	float dmax = 0;
	int idx = 0;
	for (int i = begin + 1; i <= end - 1; i++)
	{
		float d = PerpendicularDistance(points[i], points[begin], points[end]);
		if (d > dmax)
		{
			idx = i;
			dmax = d;
		}
	}

	// if distance is larger than user-defined epsilon
	if (dmax > epsilon)
	{
		// do recursively
		RamerDouglasPeuckerSegment(points, epsilon, result, begin, idx);
		RamerDouglasPeuckerSegment(points, epsilon, result, idx, end);

	}
	else
	{
		// otherwise, just ignore the points between point begin & end
		if (result.empty())result.push_back(points[begin]);
		result.push_back(points[end]);
	}
}

float EuclideanDistance(const Vec2& a, const Vec2& b)
{
	return sqrt(SQR(a.x - b.x) + SQR(a.y - b.y));
}

float ManhattanDistance(const Vec2& a, const Vec2& b)
{
	return abs(a.x - b.x) + abs(a.y - b.y);
}

// vector cross product
inline float CrossVec2(const Vec2& a, const Vec2& b)
{
	return a.x*b.y - b.x*a.y;
}

bool PointClockwiseComparator(const Vec2& a, const Vec2& b)
{
	if (a.x >= 0 && b.x < 0)return true;
	if (a.x < 0 && b.x >= 0)return false;
	if (a.x == 0 && b.x == 0)return a.y>b.y;

	// vector cross product
	int c = CrossVec2(a, b);
	//if (c<0){ log("a: %f, %f < b: %f, %f", a.x, a.y, b.x, b.y); }
	//else if (c>0){ log("a: %f, %f > b: %f, %f", a.x, a.y, b.x, b.y); }
	if (c)return (c < 0) ? true : false;
	else 
	{
		// if collinear, choose the farthest
		int d = SQR(a.x) - SQR(b.x) + SQR(a.y) - SQR(b.y); 
		//if (d<0){ log("a: %f, %f < b: %f, %f", a.x, a.y, b.x, b.y); }
		//else if (d>0){ log("a: %f, %f > b: %f, %f", a.x, a.y, b.x, b.y); }
		return (d < 0) ? true : false;
	}
}

bool PointAntiClockwiseComparator(const Vec2& a, const Vec2& b)
{
	return !PointClockwiseComparator(a, b);
}

Rect Intersection(const Rect& rect1, const Rect& rect2)
{
	float xa1 = rect1.getMinX();
	float ya1 = rect1.getMinY();

	float xa2 = rect1.getMaxX();
	float ya2 = rect1.getMaxY();

	float xb1 = rect2.getMinX();
	float yb1 = rect2.getMinY();

	float xb2 = rect2.getMaxX();
	float yb2 = rect2.getMaxY();

	float xc1, yc1;
	float xc2, yc2;

	xc1 = MAX(xa1, xb1);
	yc1 = MAX(ya1, yb1);

	xc2 = MIN(xa2, xb2);
	yc2 = MIN(ya2, yb2);

	return CCRectMake(xc1, yc1, MAX(xc2 - xc1, 0), MAX(yc2 - yc1, 0));
}

void ConvexHull(vector<Vec2>& points, vector<Vec2>& result)
{
	if (points.size() <= 2){ result = points; return; }
	int idx = 0;
	result.clear();
	for (int i = 1; i<points.size(); i++)
	{
		if ((points[idx].x >points[i].x)
			|| ((points[idx].x == points[i].x) && (points[idx].y) > points[i].y))
			idx = i;
	}

	// swap points[0] and points[idx], and calc relative points
	Vec2 relativePoint = points[idx]; points[idx] = points[0]; points[0] = relativePoint;
	for (int i = 0; i < points.size(); i++)
	{
		points[i] = points[i] - relativePoint;
	}

	// sort points by clockwise
	sort(points.begin() + 1, points.end(), PointClockwiseComparator);

	// graham scan algorithm
	result.push_back(points[0]);
	result.push_back(points[1]);
	for (int i = 2; i < points.size(); i++)
	{
		Vec2 *top1, *top2;

		while (top1 = &result[result.size() - 1],
			top2 = &result[result.size() - 2],
			CrossVec2(*top2 - *top1, points[i] - *top1) <= 0)
		{
			result.pop_back();
			if (result.size() < 2)break;
		}
		result.push_back(points[i]);
	}

	// output result, add back relative points offset
	for (int i = 0; i < result.size(); i++)result[i] = result[i] + relativePoint;
}