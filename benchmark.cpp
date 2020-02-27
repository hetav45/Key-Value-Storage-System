#include <bits/stdc++.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "rbnewvectorized.cpp"
using namespace std;
char *random_key(int stringLength)
{
	// string key = "";
	string letters = "";
	char *key = (char *)malloc(sizeof(char) * stringLength + 1);
	for (char i = 'a'; i <= 'z'; i++)
		letters += i;
	for (char i = 'A'; i <= 'Z'; i++)
		letters += i;
	for (int i = 0; i < stringLength; i++)
		key[i] = letters[rand() % 52];
	key[stringLength] = '\0';

	return key;
}

char *random_value(int stringLength)
{
	char *value = (char *)malloc(sizeof(char) * stringLength + 1);

	// string value = "";
	string letters = "";
	for (int i = 0; i <= 255; i++)
		letters += char(i);

	for (int i = 0; i < stringLength; i++)
		value[i] = letters[rand() % 256];
	value[stringLength] = '\0';
	return value;
}
long CLOCKS_PER_SECOND = 1000000;
kvStore kv;
map<string, string> db;
int db_size = 0;
int num = 0;

// void *myThreadFun(void *vargp)
// {
// 	int transactions = 0;
// 	clock_t start = clock();
// 	int time = 10;
// 	clock_t tt = clock();
// 	while ((float(tt - start) / CLOCKS_PER_SECOND) <= time)
// 	{

// 		for (int i = 0; i < 10000; i++)
// 		{
// 			transactions += 1;
// 			int x = rand() % 5;
// 			if (x == 0)
// 			{
// 				Slice k1;
// 				// k1.data = (char *)malloc(sizeof(char) * 64);
// 				Slice k2;
// 				// k2.data = (char *)malloc(sizeof(char) * 256);

// 				// k1.data = (char *)malloc(sizeof(char) * 10);
// 				// k1.data = random_key(10);
// 				bool ans = kv.get(&k1, &k2);
// 				// free(k1.data);
// 				// free(k2.data);
// 			}
// 			else if (x == 1)
// 			{
// 				int k = rand() % 64 + 1;
// 				int v = rand() % 256 + 1;
// 				Slice key;
// 				// key.data = (char *)malloc(sizeof(char) * 64);
// 				strcpy(key.data,random_key(k));
// 				Slice value;
// 				// value.data = (char *)malloc(sizeof(char) * 256);
// 				strcpy(value.data,random_value(k));
// 				bool ans = kv.put(&key, &value);
// 				db_size++;
// 				// free(key.data);
// 				// free(value.data);

// 			}
// 			else if (x == 2)
// 			{
// 				int temp = db_size;
// 				if (temp == 0)
// 					continue;
// 				int rem = rand() % temp;
// 				Slice key, value;
// 				// key.data = (char *)malloc(sizeof(char) * 64);
// 				// value.data = (char *)malloc(sizeof(char) * 256);

// 				bool check = kv.get(rem, &key, &value);
// 				bool check2 = kv.del(&key);
// 				db_size--;
// 				// free(key.data);
// 				// free(value.data);
// 			}
// 			else if (x == 3)
// 			{
// 				int temp = db_size;
// 				if (temp == 0)
// 					continue;
// 				int rem = rand() % temp;
// 				Slice key, value;
// 				// key.data = (char *)malloc(sizeof(char) * 64);
// 				// value.data = (char *)malloc(sizeof(char) * 256);

// 				bool check = kv.get(rem+1, &key, &value);
// 				// free(key.data);
// 				// free(value.data);
// 			}
// 			else if (x == 4)
// 			{
// 				int temp = db_size;
// 				if (temp == 0)
// 					continue;
// 				int rem = rand() % temp;
// 				bool check = kv.del(rem+1);
// 				db_size--;
// 			}
// 		}
// 		tt = clock();
// 	}
// 	cout << transactions / time << endl;
// 	return NULL;
// }

int main()
{
	srand(time(NULL));
	for (int i = 0; i < 10000; i++)
	{
		// cout<<"start"<<endl;
		int k = rand() % 64 + 1;
		int v = rand() % 256 + 1;
		Slice key, value;
		key.data = new char[65];
		value.data = new char[257];
		//key.data = (char *)malloc(sizeof(char) * k);
		strcpy(key.data, random_key(k));
		// value.data = (char *)malloc(sizeof(char) * v);
		strcpy(value.data, random_value(k));
		db.insert(pair<string, string>(key.data, value.data));
		kv.put(key, value);
		db_size++;
		// cout<<"end"<<endl;
		// free(value.data);
		// free(key.data);
	}
	//cout<<"after"<<endl;

	bool incorrect = false;

	for (int i = 0; i < 10000; i++)
	{
		int x = rand() % 5;
		if (x == 0)
		{
			Slice key;
			key.data =new char[65];
			// key.data = (char *)malloc(sizeof(char) * 64);
			strcpy(key.data, random_key(10));
			Slice value;
			value.data =new char[257];
			// value.data = (char *)malloc(sizeof(char) * 256);
			// string k = random_key(10);
			bool ans = kv.get(key, value);
			map<string, string>::iterator itr = db.find(key.data);
			if ((ans == false && itr != db.end()) || (ans == true && itr == db.end()))
			{
				printf("x=%d\n", x);
				incorrect = true;
			}
		}
		else if (x == 1)
		{
			int k = rand() % 64 + 1;
			int v = rand() % 256 + 1;
			Slice key;
			key.data =new char[65];
			// key.data = (char *)malloc(sizeof(char) * 64);

			// key.data = (char *)malloc(sizeof(char) * k);
			strcpy(key.data, random_key(k));
			Slice value;
			value.data =new char[257];
			// value.data = (char *)malloc(sizeof(char) * 256);
			strcpy(value.data, random_value(k));
			Slice value1;
			value1.data =new char[257];
			
			// value1.data = (char *)malloc(sizeof(char) * 256);

			db.insert(pair<string, string>(key.data, value.data));
			bool check1 = kv.get(key, value1);
			bool ans = kv.put(key, value);
			bool check2 = kv.get(key, value1);
			db_size++;
			if (check2 == false || check1 != ans)
			{
				printf("x=%d\n", x);
				incorrect = true;
			}
			// free(value.data);
			// free(value1.data);
			// free(key.data);
		}
		else if (x == 2)
		{
			int max_size = db.size();
			int rem = rand() % max_size;
			map<string, string>::iterator itr = db.begin();
			for (int i = 0; i < rem; i++)
				itr++;
			Slice key;
			key.data =new char[65];
			// key.data = (char *)malloc(sizeof(char) * 64);

			// key.data = (char *)malloc(sizeof(char) * itr->first.size());
			// string key = itr->first;
			strcpy(key.data, itr->first.c_str());
			bool check = kv.del(key);
			db_size--;
			db.erase(itr);
			Slice value;
			value.data =new char[257];
			// value.data = (char *)malloc(sizeof(char) * 256);

			bool check2 = kv.get(key, value);
			if (check2 == true)
			{
				printf("x=%d\n", x);

				incorrect = true;
			}
			// free(key.data);
			// free(value.data);
		}
		else if (x == 3)
		{
			int max_size = db.size();
			int rem = rand() % max_size;
			Slice key, value;
			key.data =new char[65];
			value.data =new char[257];
			// key.data = (char *)malloc(sizeof(char) * 64);
			// value.data = (char *)malloc(sizeof(char) * 256);
			// printf("%p %p\n",key.data,value.data);

			bool check = kv.get(rem + 1, key, value);
			// cout<<"value after this"<<endl;
			// cout << key.data<<" "<<value.data<<"2"<<endl;
			//fflush(stdout);
			map<string, string>::iterator itr = db.begin();
			for (int i = 0; i < rem; i++)
				itr++;
			// cout<<"value before this"<<endl;
			// cout<< itr->first.c_str()<<" "<<itr->second.c_str()<<"2"<<endl;
			if (strcmp(key.data, itr->first.c_str()) != 0 )//|| strcmp(value.data, itr->second.c_str()) != 0)
			{

				printf("%s %s %s %s\n", key.data, itr->first.c_str(), value.data, itr->second.c_str());
				incorrect = true;
			}
			// printf("%d\n",incorrect);
			// free(key.data);
			// free(value.data);
		}
		else if (x == 4)
		{
			int max_size = db.size();
			int rem = rand() % max_size;
			map<string, string>::iterator itr = db.begin();
			for (int i = 0; i < rem; i++)
				itr++;
			// string key = itr->first;
			Slice key;
			key.data =new char[65];
			// key.data = (char *)malloc(sizeof(char) * 64);

			// key.data = (char *)malloc(sizeof(char) * itr->first.size());
			// string key = itr->first;
			strcpy(key.data, itr->first.c_str());
			bool check = kv.del(rem + 1);
			db.erase(itr);
			db_size--;
			Slice value;
			value.data =new char[257];
			// value.data = (char *)malloc(sizeof(char) * 256);

			bool check2 = kv.get(key, value);
			if (check2 == true)
			{
				printf("x=%d\n", x);
				incorrect = true;
			}
			// free(key.data);
			// free(value.data);
		}
	}
	// if (incorrect == true)
	// {
	// 	cout << 0 << endl;
	// 	return 0;
	// }
	// // int threads = 4;

	// // pthread_t tid[threads];
	// // for (int i = 0; i < threads; i++)
	// // {
	// // 	tid[i] = i;
	// 	pthread_create(&tid[i], NULL, myThreadFun, (void *)&tid[i]);
	// }
	// for (int i = 0; i < threads; i++)
	// 	pthread_join(tid[i], NULL);
	return 0;
}