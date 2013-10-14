#include <fstream>
#include <cstdlib>

using namespace std;

int main()
{
	ofstream wyj("test.in");
	
	wyj<<1000<<endl;
	for(long i = 0; i < 1000; ++i)
		wyj<<'a';
	wyj.close();
	return 0;
}
