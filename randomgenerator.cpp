#include <bits/stdc++.h>
using namespace std;

const int MAX = 52;


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

int main()
{
    srand(time(NULL));
    int n;
    printf("10000000\n");
    for (int i = 0; i < 10000000; i++)
    {
        n = rand() % 62 + 1;
        cout << printRandomString(n) << "\n";
        n = rand() % 200 +50;
        cout << printRandomString(n) << "\n";
    }
    return 0;
}