#include <fstream>
#include <cstdlib>

using namespace std;

int main()
{
	ofstream wyj("test.in");
	
	wyj<<100000<<endl;
	for(long i = 0; i < 100000; ++i)
		wyj<<rand()<<" ";
	wyj.close();
	return 0;
}
