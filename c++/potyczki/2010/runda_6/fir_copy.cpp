#include <cstdio>
#include <list>
#include <stack>

struct Node
{
	std::list<long long> children;
	long long childrenCount;
	long long height;
	
	Node()	: height(0), childrenCount(0) {}
};

void Insert(Node* workers, long long superior, long long fresh)
{
	//workers[fresh] = new Node();
	workers[fresh].height = workers[superior].height + 1;
	workers[superior].children.push_front(fresh);
	++workers[superior].childrenCount;
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
		
		if(workers[t].height == desiredLevel)
			result += workers[t].childrenCount;
		else
			for(std::list<long long>::iterator it = workers[t].children.begin();
				it != workers[t].children.end(); ++it)
					slaves.push(*it); 
	}
	
	return result;
}

int main()
{
	long long n;
	scanf("%lld", &n);
	
	Node* workers = new Node[n + 2];
	//workers[1] = new Node();
	
	for(long long i = 0; i < n;)
	{
		char c;
		long long a, b;
		scanf("%c", &c);
		if(c == 'Z')
		{
			scanf("%lld", &a);
			scanf("%lld", &b);
			
			Insert(workers, b, a);
			
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
