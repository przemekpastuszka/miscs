#include <cstdio>
#include <algorithm>

struct Point
{
	long long x, y;
};

struct Range
{
	long long minY, maxY, x;
};

bool Cmp(Point a, Point b)
{
	if(a.x == b.x)
		return a.y < b.y;
	return a.x < b.x;
}

void ComputeRanges(Range ranges[], long long n, long long direction, long long d)
{
	long long startPoint = 0;
	if(direction == -1)
		startPoint = n - 1;
	
	long long minY = ranges[startPoint].minY, maxY = ranges[startPoint].maxY;
	ranges[startPoint].minY = ranges[startPoint].maxY = 0;
	
	for(long long i = 1; i < n; ++i)
	{
		long long current = startPoint + i * direction;
		Range temp = ranges[current];
		ranges[current].minY = minY;
		ranges[current].maxY = maxY;
		
		if(temp.minY < minY)
			minY = temp.minY;
		if(temp.maxY > maxY)
			maxY = temp.maxY;
	}
}

long long FillWithRanges(Point points[], long long n, long long d, Range ranges[])
{
	long long unique = -1;
	long long lastX = -d - 1;
	
	for(long long i = 0; i < n; ++i)
	{
		if(points[i].x != lastX)
		{
			++unique; 
			lastX = points[i].x;
			ranges[unique].minY = ranges[unique].maxY = points[i].y;
			ranges[unique].x = points[i].x;
		}
		else
		{
			if(points[i].y < ranges[unique].minY)
				ranges[unique].minY = points[i].y;
			if(points[i].y > ranges[unique].maxY)
				ranges[unique].maxY = points[i].y;
		}
	}
	
	return unique + 1;
}

long long Compute(long long n, long long d, Point points[])
{
	std::sort(points, points + n, Cmp);
	
	Range *rangesLeft = new Range[n], *rangesRight = new Range[n];
	
	long long howManyRanges = FillWithRanges(points, n, d, rangesLeft);
	FillWithRanges(points, n, d, rangesRight);
	
	
	ComputeRanges(rangesLeft, howManyRanges, 1, d);
	ComputeRanges(rangesRight, howManyRanges, -1, d);
	
	//for(long long i = 0; i < howManyRanges; ++i)
		//printf("%lld: %lld; %lld \n", rangesRight[i].x, rangesRight[i].minY, rangesRight[i].maxY);
	
	
	long long result = 0;
	long long lastX = rangesLeft[0].x;
	for(long long i = 0; i < howManyRanges - 1; ++i)
	{
		long long minY = 
			rangesLeft[i + 1].minY > rangesRight[i].minY ?
				rangesLeft[i + 1].minY:
				rangesRight[i].minY;
		long long maxY = 
			rangesLeft[i + 1].maxY < rangesRight[i].maxY ?
				rangesLeft[i + 1].maxY:
				rangesRight[i].maxY;
				
		long long t = (maxY - minY - 1) * (rangesLeft[i + 1].x - lastX - 1);
		t = t > 0 ? t : 0;
		result += t;
		lastX = rangesLeft[i + 1].x;
		
		minY = 
			rangesLeft[i + 1].minY > rangesRight[i + 1].minY ?
				rangesLeft[i + 1].minY:
				rangesRight[i + 1].minY;
		maxY = 
			rangesLeft[i + 1].maxY < rangesRight[i + 1].maxY ?
				rangesLeft[i + 1].maxY:
				rangesRight[i + 1].maxY;
		t = (maxY - minY - 1);
		t = t > 0 ? t : 0;
		result += t;
	}
	
	delete [] rangesLeft;
	delete [] rangesRight;
	
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
	}

	long long result = Compute(n, d, points);
	printf("%lld", result);

	delete [] points;
	return 0;
}
