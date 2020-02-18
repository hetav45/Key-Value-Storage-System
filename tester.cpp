#include <bits/stdc++.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
//#include "workingrbtree.cpp"
using namespace std;

string random_key(int stringLength)
{
	string key = "";
	string letters = "";
	for (char i = 'a'; i <= 'z'; i++)
		letters += i;
	for (char i = 'A'; i <= 'Z'; i++)
		letters += i;
	for (int i = 0; i < stringLength; i++)
		key = key + letters[rand() % 52];

	return key;
}

int main()
{
	//RedBlackTree kv;
	for (int i = 0; i < 10000000; i++)
	{																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																								
		int k = rand() % 64 + 1;
		int v = rand() % 256 + 1;
		string key = random_key(k);
		string value = random_key(v);
		// db.insert(pair<string, string>(key, value));
		// kv.put((char *)key.c_str(), (char *)value.c_str());
		// db_size++;
		cout <<key<<" "<<value<<endl;
	}
}