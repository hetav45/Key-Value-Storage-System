#include <stdio.h>
#include <immintrin.h>

void keycopy(char *b, char *a)
{
    for (int i = 0; i < 2; i++)
    {
        _mm256_storeu_si256((__m256i *)&b[i * 32], _mm256_loadu_si256((const __m256i *)&a[i * 32]));
    }
    b[64] = '\0';
}

void valuecopy(char *b, char *a)
{
    for (int i = 0; i < 8; i++)
    {
        _mm256_storeu_si256((__m256i *)&b[i * 32], _mm256_loadu_si256((const __m256i *)&a[i * 32]));
    }
    b[256] = '\0';
}
// int main()
// {
//     char a[65];
//     scanf("%s",a);
//     char b[65];
//     keycopy()
// }