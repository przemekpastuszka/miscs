#include <stdio.h>
#include <string.h>

#define MAXINPUT 1000000
#define ABS(x) (x < 0 ? -x - 1 : x)

void ManacherEven(int radius[], char word[], long long n)
{
    int i = 0;
    int k = 0;
    while(i < n)
    {
        while(i - k >= 0 && i + 1 + k < n && word[i - k] == word[i + 1 + k])
            ++k;
        radius[i] = k;
        int z = 1;
        for(; z < k && radius[i - z] != radius[i] - z; ++z)
            radius[i + z] = radius[i - z] < radius[i] - z ? radius[i - z] : radius[i] - z;
        i += z > 0 ? z : 1; k -= k < z ? 0 : z;
    }
}

int main() {
    char word[MAXINPUT + 2];
    int radius[MAXINPUT + 2];
    int T;
    

    scanf("%d", &T);
    scanf("\r");
    scanf("\n");
    int z = 0;
    for(; z < T; ++z) {
        gets(word);
        int length = strlen(word);
        if(word[length - 1] == '\r') {
            --length;
        }
        ManacherEven(radius, word, length);

        int last_prefix = -1;
        int i = 0;
        for(; i < length; ++i) {
            if(i - ABS(radius[i]) <= last_prefix) {
                int next_prefix_index = 2 * i - last_prefix;
                radius[next_prefix_index] = -ABS(radius[next_prefix_index]) - 1;
            }
            if(radius[i] < 0) {
                last_prefix = i;
            }
        }

        if(last_prefix == length - 1) {
            printf("YES\n");
        }
        else {
            printf("NO\n");
        }
    }
    return 0;
}
