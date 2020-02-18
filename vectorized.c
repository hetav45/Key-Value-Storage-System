#include <stdio.h>
#include <immintrin.h>

void keycopy(char *a, char *b)
{
    for (int i = 0; i < 2; i++)
    {
        _mm256_storeu_si256((__m256i *)&b[i * 32], _mm256_loadu_si256((const __m256i *)&a[i * 32]));
    }
    b[64] = '\0';
}

void valuecopy(char *a, char *b)
{
    for (int i = 0; i < 8; i++)
    {
        _mm256_storeu_si256((__m256i *)&b[i * 32], _mm256_loadu_si256((const __m256i *)&a[i * 32]));
    }
    b[256] = '\0';
}