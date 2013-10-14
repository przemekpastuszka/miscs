#include <iostream>
#include <cfloat>
#include <cmath>
using namespace std;



float P(int k, float sK)
{
	float result = sK;
	for(int i = 0; i < k - 1; ++i)
		result *= 2;
	return result;
}

int main()
{
	/*float ck[24];
	ck[2] = 0;
	for(int k = 3; k < 24; ++k)
	{
		ck[k] = sqrt((1 + ck[k - 1]) / 2);
		cout << ck[k] << endl;
	}*/
	
	float sk[24];
	sk[2] = 1;
	for(int i = 3; i <= 24; ++i)
	{
		//sk[i] = sqrt((1 - sqrt(1 - sk[i - 1] * sk[i - 1])) / 2);
		sk[i] = (sqrt(2) / 2) * sk[i - 1] / (sqrt(1 + sqrt(1 - sk[i - 1] * sk[i - 1])));
	//	cout << sk[i] << endl;
	}
	for(int k = 3; k <= 24; ++k)
		cout << P(k, sk[k]) << endl;
	return 0;
}
