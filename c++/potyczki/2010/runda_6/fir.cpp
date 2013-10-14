#include <cstdio>
#include <list>
#include <stack>	

const long long levelNr = 4;
long long levels[] = {1, 3, 7, 179};

struct Node
{
	long long lastWaypoint[levelNr];
	std::list<long long> succesors[levelNr];
	long long succesorsNr[levelNr];
	long long height;
	
	Node()
	{
		height = 0;
		
		for(long long i = 0; i < levelNr; ++i)
			lastWaypoint[i] = succesorsNr[i] = 0;
	}
};

void Insert(Node* workers, long long superior, long long fresh)
{
	workers[fresh].height = workers[superior].height + 1;
	for(long long i = 0; i < levelNr; ++i)
	{
		if(workers[fresh].height - workers[workers[superior].lastWaypoint[i]].height == levels[i])
		{
			workers[fresh].lastWaypoint[i] = fresh;
			workers[workers[superior].lastWaypoint[i]].succesors[i].push_front(fresh);
			++workers[workers[superior].lastWaypoint[i]].succesorsNr[i];
		}
		else
			workers[fresh].lastWaypoint[i] = workers[superior].lastWaypoint[i];
	}
	
}

long long Compute(Node* workers, long long worker, long long k)
{
	long long result = 0;
	
	long long desiredLevel = workers[worker].height + k;
	
	std::stack<long long> slaves;
	slaves.push(worker);
	
	while(!slaves.empty())
	{
		long long t = slaves.top();
		slaves.pop();

		for(long long i = levelNr - 1; i >= 0; --i)
			if(workers[t].lastWaypoint[i] == t && workers[t].height + levels[i] - 1 <= desiredLevel)
			{
				if(workers[t].height + levels[i] - 1 == desiredLevel)
					result += workers[t].succesorsNr[i];
				else
				{
					long long last = workers[t].lastWaypoint[i];
					for(std::list<long long>::iterator it = workers[last].succesors[i].begin();
						it != workers[last].succesors[i].end(); ++it)
							slaves.push(*it);
				}
				break;
			}
	}
	
	
	return result;
}

int main()
{
	long long n;
	scanf("%lld", &n);
	
	Node* workers = new Node[n + 2];
	for(long long i = 0; i < levelNr; ++i)
		workers[1].lastWaypoint[i] = 1;
	
	for(long long i = 0; i < n;)
	{
		char c;
		long long a, b;
		scanf("%c", &c);
		if(c == 'Z')
		{
			scanf("%lld", &b);
			scanf("%lld", &a);
			
			Insert(workers, a, b);
			++i;
		}
		if(c == 'P')
		{
			scanf("%lld", &a);
			scanf("%lld", &b);
			
			long long result = Compute(workers, a, b);
			printf("%lld \n", result);
			++i;
		}
	}
	
	delete [] workers;
	return 0;
}
