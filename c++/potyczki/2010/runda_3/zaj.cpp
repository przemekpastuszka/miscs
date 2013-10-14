#include <cstdio>

int LCS(char znaki[], int pX, int qX, int pY, int qY)
{
	int lengthX = qX - pX + 1;
	int lengthY = qY - pY + 1;
	
	int** c = new int*[lengthX + 1];
	for(int i = 0; i <= lengthX; ++i)
		c[i] = new int[lengthY + 1];
		
	for(int i = 1; i <= lengthX; ++i)
		c[i][0] = 0;
	for(int i = 0; i <= lengthY; ++i)
		c[0][i] = 0;
		
	for(int i = 1; i <= lengthX; ++i)
		for(int j = 1; j <= lengthY; ++j)
		{
			if(znaki[i + pX - 1] == znaki[j + pY - 1])
				c[i][j] = c[i - 1][j - 1] + 1;
			else
			{
				if(c[i - 1][j] >= c[i][j - 1])
					c[i][j] = c[i - 1][j];
				else
					c[i][j] = c[i][j - 1];
			}
		}	
		
	int result = c[lengthX][lengthY];
	for(int i = 0; i <= lengthX; ++i)
		delete [] c[i];
	delete [] c;
	return result;
}

int Compute(int n, char znaki[])
{
	int max = 0;
	for(int i = 1; i < n; ++i)
	{
		int result = LCS(znaki, 1, i, i + 1, n);
		if(result > max)
			max = result;
	}
	return n - 2 * max;
}

int main()
{
	int n;
	scanf("%d", &n);
	
	char* znaki = new char[n + 1];

	for(long i = 1; i <= n ;)
	{
		char c;
		scanf("%c", &c);

		znaki[i] = c;
		if(c >= 'a' && c <= 'z')
			++i;
	}

	int result = Compute(n, znaki);
	printf("%d", result);

	delete [] znaki;
	return 0;
}
