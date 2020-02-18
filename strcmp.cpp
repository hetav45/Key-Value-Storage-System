#include<emmintrin.h>
#include<immintrin.h>
#include<stdint.h>
#include<stdio.h>
#include<bits/stdc++.h>
#include<time.h>

using namespace std;

int MAX = 52;

string printRandomString(int n)
{
    char alphabet[MAX] = {'a', 'b', 'c', 'd', 'e', 'f', 'g',
                          'h', 'i', 'j', 'k', 'l', 'm', 'n',
                          'o', 'p', 'q', 'r', 's', 't', 'u',
                          'v', 'w', 'x', 'y', 'z', 'A', 'B',
                          'C', 'D', 'E', 'F', 'G', 'H', 'I',
                          'J', 'K', 'K', 'M', 'N', 'O', 'P',
                          'Q', 'R', 'S', 'T', 'U', 'V', 'W',
                          'X', 'Y', 'Z'};

    string res = "";
    for (int i = 0; i < n; i++)
        res = res + alphabet[rand() % MAX];

    return res;
}

int sse42_strcmp(const char* s1, const char* s2) {

    if (s1 == s2) {
        return 0;
    }

    __m128i* ptr1 = reinterpret_cast<__m128i*>(const_cast<char*>(s1));
    __m128i* ptr2 = reinterpret_cast<__m128i*>(const_cast<char*>(s2));

    for (/**/; /**/; ptr1++, ptr2++) {

        const __m128i a = _mm_loadu_si128(ptr1);
        const __m128i b = _mm_loadu_si128(ptr2);

        const uint8_t mode =
            _SIDD_UBYTE_OPS |
            _SIDD_CMP_EQUAL_EACH |
            _SIDD_NEGATIVE_POLARITY |
            _SIDD_LEAST_SIGNIFICANT;
        
        if (_mm_cmpistrc(a, b, mode)) {
            // a & b are different (not counting past-zero bytes)
            const auto idx = _mm_cmpistri(a, b, mode);

            const uint8_t b1 = (reinterpret_cast<char*>(ptr1))[idx];
            const uint8_t b2 = (reinterpret_cast<char*>(ptr2))[idx];

            if (b1 < b2) {
                return -1;
            } else if (b1 > b2) {
                return +1;
            } else {
                return 0;
            }
        } else if (_mm_cmpistrz(a, b, mode)) {
            // a & b are same, but b contains a zero byte
            break;
        }
    }

    return 0;
}

int main() {
    int n = 1000000;
    char str1[n+1];

    cin >> str1;
    // string str2 = printRandomString(n);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);


    int ret = sse42_strcmp(&str1[0], &str1[0]);

    
    clock_gettime(CLOCK_MONOTONIC, &end);
    double tdiff = (end.tv_sec - start.tv_sec) + 1e-9 * (end.tv_nsec - start.tv_nsec);
    printf("time %f\n", tdiff);   

    cout << ret << "\n";


    clock_gettime(CLOCK_MONOTONIC, &start);
    
    ret = strcmp(&str1[0], &str1[0]);

    clock_gettime(CLOCK_MONOTONIC, &end);
    tdiff = (end.tv_sec - start.tv_sec) + 1e-9 * (end.tv_nsec - start.tv_nsec);
    printf("time %f\n", tdiff);
}