/*
Implementation of Manacher's algorithm (quite dirty version - if u know what i mean)
Przemysław Pastuszka
2.05.2010
*/

#include <iostream>
#include <cstring>

void ManacherEven(long long radius[], char word[], long long n)
{
    long long i = 0;
    long long k = 0;
    while(i < n)
    {
        while(i - k >= 0 && i + 1 + k < n && word[i - k] == word[i + 1 + k])
            ++k;
        radius[i] = k;
        long long z = 1;
        for(; z < k && radius[i - z] != radius[i] - z; ++z)
            radius[i + z] = radius[i - z] < radius[i] - z ? radius[i - z] : radius[i] - z;
        i += z > 0 ? z : 1; k -= k < z ? 0 : z;
    }
}

void ManacherOdd(long long radius[], char word[], long long n)
{
    long long i = 0;
    long long k = 0;
    while(i < n)
    {
        while(i - k >= 0 && i + k < n && word[i - k] == word[i + k])
            ++k;
        radius[i] = k;
        long long z = 1;
        for(; z < k && radius[i - z] != radius[i] - z; ++z)
            radius[i + z] = radius[i - z] < radius[i] - z ? radius[i - z] : radius[i] - z;
        i += z > 0 ? z : 1; k -= k < z ? 0 : z;
    }
}

int main()
{
    char text[200];
    std::cin >> text;
    long long length = strlen(text);
    long long* radius = new long long[length];
    ManacherEven(radius, text, length);
    
    for(long long i = 0; i < length; ++i)
        if(radius[i] != 0)
        {
            for(long long j = i - radius[i] + 1; j <= i + radius[i]; ++j)
                std::cout << text[j];
            std::cout << std::endl;
        }
        
    ManacherOdd(radius, text, length);
    for(long long i = 0; i < length; ++i)
        if(radius[i] > 1)
        {
            for(long long j = i - radius[i] + 1; j < i + radius[i]; ++j)
                std::cout << text[j];
            std::cout << std::endl;
        }
    delete [] radius;
    return 0;
}
