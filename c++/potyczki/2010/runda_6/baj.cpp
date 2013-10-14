#include <cstdio>
#include <stack>
#include <list>

struct Range
{
	long long p, q;
	Range(long long a, long long b) : p(a), q(b) {}
	Range() {}
};

/*void Intersect(std::list<Range>* a, std::list<Range>* b, std::list<Range>* intersection)
{
	for(std::list<Range>::iterator one = a -> begin(),
		std::list<Range>::iterator two = b -> begin();
		one != a -> end() && two != b -> end();)
		{
			Range result;
			result.p = (*one).p > (*two).p ? (*one).p : (*two).p;
			result.q = (*one).q < (*two).q ? (*one).q : (*two).q;
			if(result.p <= result.q)
				intersection -> push_back(result);
				
			if((*one).q < (*two).q)
				++one;
			else
				++two;
		}
}*/

Range Intersect(Range one, Range two)
{
	Range result;
	result.p = one.p > two.p ? one.p : two.p;
	result.q = one.q < two.q ? one.q : two.q;
	return result;
}


struct Branch
{
	long long succesors;
	std::list<Branch*> children;
	long long p, q;
};

struct Pair
{
	long long howManyLeft;
	Branch* branch;
	Pair(long long n, Branch *b) : branch(b), howManyLeft(n) {}
};

long long Compute(Branch *trunk)
{
	long long result = 0;
	
	std::stack<Range> subresults;
	std::stack<Pair> pending;
	pending.push(Pair(1, trunk));
	while(!pending.empty())
	{
		Pair p = pending.top();
		pending.pop();
		
		if(p.howManyLeft == 1)
		{
			pending.push(Pair(0, p.branch));
			for(std::list<Branch*>::iterator it = p.branch -> children.begin();
				it != p.branch -> children.end(); ++it)
				pending.push(Pair(1, *it));
		}
		else
		{
			if(p.branch -> succesors == 0)
				subresults.push(Range(p.branch -> p, p.branch -> q));
			else
			{
				Range* ranges = new Range[p.branch -> succesors];
				Range intersected(-1, -1);
				for(long long i = 0; i < p.branch -> succesors; ++i)
				{
					ranges[i] = subresults.top();
					subresults.pop();
					
					if(ranges[i].p > intersected.p)
						intersected = ranges[i];
				}
				for(long long i = 0; i < p.branch -> succesors; ++i)
				{
					Range temp = Intersect(intersected, ranges[i]);
					if(temp.p > temp.q)
						++result;
					else
						intersected = temp;
				}
				subresults.push(intersected);
				
				delete ranges;
			}
		}
	}
	
	if(subresults.top().p <= subresults.top().q)
		++result;
	
	return result;
}

int main()
{
	Branch* trunk = new Branch();
	scanf("%lld", &trunk -> succesors);
	if(trunk -> succesors == 0)
	{
		scanf("%lld", &trunk -> p);
		scanf("%lld", &trunk -> q);
	}
	else
	{
		std::stack<Pair> pairs;
		pairs.push(Pair(trunk -> succesors, trunk));
		while(!pairs.empty())
		{
			Pair p = pairs.top();
			pairs.pop();
		
			Branch* twig = new Branch();
			p.branch -> children.push_front(twig);
		
			if(p.howManyLeft > 1)
				pairs.push(Pair(p.howManyLeft - 1, p.branch));
			
			scanf("%lld", &twig -> succesors);
			if(twig -> succesors == 0)
			{
				scanf("%lld", &twig -> p);
				scanf("%lld", &twig -> q);
			} 
			else
				pairs.push(Pair(twig -> succesors, twig));
		}
	}
	
	long long result = Compute(trunk);
	printf("%lld", result);
	
	std::stack<Branch*> funeral;
	funeral.push(trunk);
	while(!funeral.empty())
	{
		Branch* b = funeral.top();
		funeral.pop();
		
		for(std::list<Branch*>::iterator it = b -> children.begin();
			it != b -> children.end(); ++it)
			funeral.push(*it);
		
		delete b;
	}
	
	return 0;
}
