void CalculateDiffs(int X[], int Y[], int length, long long result[])
{	
	result[0] = Y[0] - X[0];
	for(int j = 1; j < length; ++j)
		result[j] = result[j - 1] * 10 + Y[j] - X[j];
		
	for(int j = 0; j < length; ++j)
		++result[j];
}
