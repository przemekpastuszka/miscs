#include <cstdio>
#include <algorithm>

struct Point
{
	long long x, y;
	bool bad;
};

bool Cmp(Point a, Point b)
{
	if(a.x == b.x)
		return a.y < b.y;
	return a.x < b.x;
}

void MarkBad(Point points[], long long n, long long direction, long long d)
{
	long long startPoint = 0;
	if(direction == -1)
		startPoint = n - 1;
	
	points[startPoint].bad = true;
	Point minY, maxY, lastMin, lastMax;
	lastMin.x = lastMax.x = (d + 1) * -direction;
	lastMin.y = d + 1;
	lastMax.y = -d - 1;
	minY = maxY = points[startPoint];
	
	for(long long i = 1; i < n; ++i)
	{
		long long current = startPoint + i * direction;
		if(points[current].y >= maxY.y)
		{
			points[current].bad = true;
			if(points[current].x != maxY.x)
				lastMax = maxY;
			maxY = points[current];
		}
		if(points[current].y <= minY.y)
		{
			points[current].bad = true;
			if(points[current].x != minY.x)
				lastMin = minY;
			minY = points[current];
		}
		if(points[current].x == maxY.x
			&& points[current].y >= lastMax.y)
				points[current].bad = true;
		if(points[current].x == minY.x
			&& points[current].y <= lastMin.y)
				points[current].bad = true;
	}
}

long long Compute(long long n, long long d, Point points[])
{
	std::sort(points, points + n, Cmp);
	MarkBad(points, n, 1, d);
	MarkBad(points, n, -1, d);
	
	long long result = 0;
	for(long long i = 0; i < n; ++i)
		if(!points[i].bad)
			++result;
	return result;
}

int main()
{
	long long n, d;
	scanf("%lld", &n);
	scanf("%lld", &d);
	
	Point* points = new Point[n];

	for(long long i = 0; i < n ; ++i)
	{
		scanf("%lld", &points[i].x);
		scanf("%lld", &points[i].y);
		points[i].bad = false;
	}

	long long result = Compute(n, d, points);
	printf("%lld", result);

	delete [] points;
	return 0;
}
