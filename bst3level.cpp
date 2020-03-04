// Implementing Red-Black Tree in C++
#include <pthread.h>
#include <bits/stdc++.h>
using namespace std;
struct Node
{
    char data[66];
    char value[258];
    int key_size;
    int value_size;
    Node *parent;
    Node *left;
    Node *right;
    int leftNo;
    int rightNo;
};
struct Slice
{
    uint8_t size;
    char *data;
};
typedef Node *NodePtr;
class kvStore
{
private:
    NodePtr root[52][53][53];
    pthread_mutex_t mutex[52][53][53];
    pthread_mutex_t mutex_no;
    // pthread_mutex_t mutex_global;

    NodePtr TNULL;
    int no_of_element[55][55];
    int first[55];
    void initializeNULLNode(NodePtr node, NodePtr parent)
    {
        node->parent = parent;
        node->left = nullptr;
        node->right = nullptr;
        node->leftNo = 0;
        node->rightNo = 0;
    }
    void preOrderHelper(NodePtr node)
    {
        if (node != TNULL)
        {
            // cout << node->data << " ";
            preOrderHelper(node->left);
            preOrderHelper(node->right);
        }
    }

    void inOrderHelper(NodePtr node, int *j, int k, char *key, char *value)
    {
        if (node != TNULL)
        {
            inOrderHelper(node->left, j, k, key, value);
            (*j)++;
            if (*j == k)
            {
                strncpy(key, node->data, node->key_size);
                strncpy(value, node->value, node->value_size);
                return;
            }
            inOrderHelper(node->right, j, k, key, value);
        }
    }
    void postOrderHelper(NodePtr node)
    {
        if (node != TNULL)
        {
            postOrderHelper(node->left);
            postOrderHelper(node->right);
            // cout << node->data << " ";
        }
    }
    NodePtr searchTreeHelper(NodePtr node, char *key)
    {
        // printf("Insearh *%s* *%s*\n",node->data,key);
        if (node == TNULL || strcmp(key, node->data) == 0)
        {
            return node;
        }
        if (strcmp(key, node->data) < 0)
        {
            return searchTreeHelper(node->left, key);
        }
        return searchTreeHelper(node->right, key);
    }
    NodePtr getTreeHelper(NodePtr node, int N)
    {
        if (node == TNULL || ((node->leftNo + 1) == N))
        {
            return node;
        }
        else if (N > node->leftNo + 1)
        {
            return getTreeHelper(node->right, N - node->leftNo - 1);
        }
        else
        {
            return getTreeHelper(node->left, N);
        }
    }
    //
    void rbTransplant(NodePtr u, NodePtr v, int index1, int index2, int index3)
    {
        if (u->parent == nullptr)
        {
            root[index1][index2][index3] = v;
        }
        else if (u == u->parent->left)
        {
            u->parent->left = v;
        }
        else
        {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }
    char *searchHelper(NodePtr node, char *key)
    {
        NodePtr z = TNULL;
        NodePtr x, y;
        while (node != TNULL)
        {
            if (strcmp(node->data, key) == 0)
            {
                z = node;
            }
            if (strcmp(node->data, key) <= 0)
            {
                node = node->right;
            }
            else
            {
                node = node->left;
            }
        }
        if (z == TNULL)
        {
            return NULL;
        }
        y = z;
        return y->value;
    }
    bool deleteNodeHelper(NodePtr node, char *key, int index1, int index2, int index3)
    {
        NodePtr z = TNULL;
        NodePtr x, y;
        NodePtr temp = searchTreeHelper(node, key);
        if (temp != TNULL)
        {
            pthread_mutex_lock(&mutex_no);
            // printf("no lock acquired\n");
            no_of_element[index1][index2]--;
            first[index1]--;

            pthread_mutex_unlock(&mutex_no);
            // printf("no lock given\n");

            while (node != TNULL)
            {
                if (strcmp(node->data, key) == 0)
                {
                    z = node;
                    break;
                }
                if (strcmp(node->data, key) < 0)
                {
                    node->rightNo -= 1;
                    node = node->right;
                }
                else
                {
                    node->leftNo -= 1;
                    node = node->left;
                }
            }
            while (node != TNULL)
            {
                if (strcmp(node->data, key) == 0)
                {
                    z = node;
                }
                if (strcmp(node->data, key) <= 0)
                {
                    node = node->right;
                }
                else
                {
                    node = node->left;
                }
            }
            if (z == TNULL)
            {
                return false;
            }
            y = z;
            if (z->left == TNULL)
            {
                x = z->right;
                rbTransplant(z, z->right, index1, index2, index3);
            }
            else if (z->right == TNULL)
            {
                x = z->left;
                rbTransplant(z, z->left, index1, index2, index3);
            }
            else
            {
                y = minimum(z->right);
                x = y->right;
                if (y->parent == z)
                {
                    x->parent = y;
                }
                else
                {
                    rbTransplant(y, y->right, index1, index2, index3);
                    y->right = z->right;
                    y->rightNo = z->rightNo - 1;
                    y->right->parent = y;
                }
                rbTransplant(z, y, index1, index2, index3);
                y->left = z->left;
                y->leftNo = z->leftNo;
                y->left->parent = y;
            }
            delete z;
            return true;
        }
        else
        {
            return false;
        }
    }
    void printHelper(NodePtr root, string indent, bool last)
    {
        if (root != TNULL)
        {
            cout << indent;
            if (last)
            {
                cout << "R----";
                indent += "   ";
            }
            else
            {
                cout << "L----";
                indent += "|  ";
            }
            cout << root->data << "("
                 << "  "
                 << ")  " << root->leftNo << "   " << root->rightNo << endl;
            printHelper(root->left, indent, false);
            printHelper(root->right, indent, true);
        }
    }

public:
    kvStore(int n)
    {
        TNULL = new Node;
        TNULL->left = nullptr;
        TNULL->right = nullptr;
        for (int i = 0; i < 52; i++)
        {
            for (int j = 0; j < 53; j++)
            {
                for (int k = 0; k < 53; k++)
                {
                    root[i][j][k] = TNULL;
                }
            }
        }

        //init mutex locks
        // pthread_mutex_init(&mutex_global, NULL);

        if (pthread_mutex_init(&mutex_no, NULL))
        {
            printf("error initiating mutex locks\n");
            exit(1);
        }

        for (int i = 0; i < 52; i++)
        {
            for (int j = 0; j < 53; j++)
            {
                for (int k = 0; k < 53; k++)
                {
                    if (pthread_mutex_init(&mutex[i][j][k], NULL))
                    {
                        printf("error initiating mutex locks\n");
                        exit(1);
                    }
                }
            }
        }
        for (int  i = 0; i < 54; i++)
        {
            first[i]=0;
        }
        
        for (int i = 0; i < 53; i++)
        {
            for (int j = 0; j < 54; j++)
            {
                no_of_element[i][j] = 0;
            }
        }
        
        
    }
    NodePtr succ(NodePtr x)
    {
        NodePtr temp = x;
        while (temp->left != TNULL)
        {
            temp = temp->left;
        }
        return temp;
    }
    NodePtr minimum(NodePtr node)
    {
        while (node->left != TNULL)
        {
            node->leftNo -= 1;
            node = node->left;
        }
        return node;
    }
    NodePtr maximum(NodePtr node)
    {
        while (node->right != TNULL)
        {
            node = node->right;
        }
        return node;
    }
    NodePtr successor(NodePtr x)
    {
        if (x->right != TNULL)
        {
            return minimum(x->right);
        }
        NodePtr y = x->parent;
        while (y != TNULL && x == y->right)
        {
            x = y;
            y = y->parent;
        }
        return y;
    }
    NodePtr predecessor(NodePtr x)
    {
        if (x->left != TNULL)
        {
            return maximum(x->left);
        }
        NodePtr y = x->parent;
        while (y != TNULL && x == y->left)
        {
            x = y;
            y = y->parent;
        }
        return y;
    }
    void leftRotate(NodePtr x, int index1, int index2, int index3)
    {
        NodePtr y = x->right;
        x->right = y->left;
        x->rightNo = y->leftNo;
        if (y->left != TNULL)
        {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr)
        {
            this->root[index1][index2][index3] = y;
        }
        else if (x == x->parent->left)
        {
            x->parent->left = y;
        }
        else
        {
            x->parent->right = y;
        }
        y->left = x;
        y->leftNo = x->leftNo + x->rightNo + 1;
        x->parent = y;
    }
    void rightRotate(NodePtr x, int index1, int index2, int index3)
    {
        NodePtr y = x->left;
        x->left = y->right;
        x->leftNo = y->rightNo;
        if (y->right != TNULL)
        {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr)
        {
            this->root[index1][index2][index3] = y;
        }
        else if (x == x->parent->right)
        {
            x->parent->right = y;
        }
        else
        {
            x->parent->left = y;
        }
        y->right = x;
        y->rightNo = x->leftNo + 1 + x->rightNo;
        x->parent = y;
    }
    bool put(Slice &key, Slice &value)
    {
        // pthread_mutex_lock(&mutex_global);

        int index1, index2, index3;
        if (key.size > 2)
        {
            index1 = code(key.data[0]);
            index2 = code(key.data[1]) + 1;
            index3 = code(key.data[2]) + 1;
        }
        else if (key.size == 2)
        {
            index1 = code(key.data[0]);
            index2 = code(key.data[1]) + 1;
            index3 = 0;
        }
        else
        {
            index1 = code(key.data[0]);
            index2 = 0;
            index3 = 0;
        }

        if (index1 < 0 || index2 < 0 || index3 < 0)
        {
            // cout << "index error" << endl;
            // pthread_mutex_unlock(&mutex_global);

            return false;
        }
        char *key1 = (char *)(malloc(key.size + 1));
        strncpy(key1, key.data, key.size);
        key1[key.size] = 0;
        pthread_mutex_lock(&mutex[index1][index2][index3]);
        NodePtr temp = searchTreeHelper(this->root[index1][index2][index3], key1);
        if (temp == TNULL)
        {
            NodePtr node = new Node;
            pthread_mutex_lock(&mutex_no);
            // printf("no lock acquired in put\n");
            no_of_element[index1][index2]++;
            first[index1]++;

            pthread_mutex_unlock(&mutex_no);
            // printf("no lock given in put\n");

            node->parent = nullptr;
            strncpy(node->data, key.data, key.size);
            strncpy(node->value, value.data, value.size);
            node->data[key.size] = 0;
            node->value[value.size] = 0;
            node->key_size = key.size;
            node->value_size = value.size;

            node->left = TNULL;
            node->right = TNULL;
            node->leftNo = 0;
            node->rightNo = 0;
            NodePtr y = nullptr;
            NodePtr x = this->root[index1][index2][index3];
            while (x != TNULL)
            {
                y = x;
                if (strcmp(node->data, x->data) < 0)
                {
                    x->leftNo += 1;
                    x = x->left;
                }
                else
                {
                    x->rightNo += 1;
                    x = x->right;
                }
            }
            node->parent = y;
            if (y == nullptr)
            {
                root[index1][index2][index3] = node;
            }
            else if (strcmp(node->data, y->data) < 0)
            {
                y->left = node;
            }
            else
            {

                y->right = node;
            }
            if (node->parent == nullptr)
            {

                pthread_mutex_unlock(&mutex[index1][index2][index3]);
                // pthread_mutex_unlock(&mutex_global);
                free(key1);
                return false;
            }
            if (node->parent->parent == nullptr)
            {

                pthread_mutex_unlock(&mutex[index1][index2][index3]);
                // pthread_mutex_unlock(&mutex_global);
                free(key1);
                return false;
            }
            // insertFix(node, index1, index2);

            pthread_mutex_unlock(&mutex[index1][index2][index3]);
            // pthread_mutex_unlock(&mutex_global);
            free(key1);
            return false;
        }
        else
        {
            NodePtr node = new Node;
            node->parent = nullptr;

            strncpy(node->data, key.data, key.size);
            strncpy(node->value, value.data, value.size);
            node->data[key.size] = 0;
            node->value[value.size] = 0;

            node->key_size = key.size;
            node->value_size = value.size;

            node->left = TNULL;
            node->right = TNULL;
            node->leftNo = 0;
            node->rightNo = 0;
            NodePtr y = nullptr;
            NodePtr x = this->root[index1][index2][index3];
            while (x != TNULL)
            {
                y = x;
                if (strcmp(node->data, x->data) < 0)
                {
                    x = x->left;
                }
                else if (strcmp(node->data, x->data) == 0)
                {
                    strncpy(y->value, value.data, value.size);
                    y->value_size = value.size;
                    y->value[value.size] = 0;
                    pthread_mutex_unlock(&mutex[index1][index2][index3]);
                    // pthread_mutex_unlock(&mutex_global);
                    free(key1);
                    return true;
                }
                else
                {
                    x = x->right;
                }
            }
            pthread_mutex_unlock(&mutex[index1][index2][index3]);
            // pthread_mutex_unlock(&mutex_global);
            free(key1);
            return false;
        }
    }

    bool del(Slice &data)
    {
        char *key1 = (char *)malloc(data.size + 1);
        strncpy(key1, data.data, data.size);
        key1[data.size] = 0;
        // pthread_mutex_lock(&mutex_global);

        int index1, index2, index3;
        if (data.size > 2)
        {
            index1 = code(data.data[0]);
            index2 = code(data.data[1]) + 1;
            index3 = code(data.data[2]) + 1;
        }
        else if (data.size == 2)
        {
            index1 = code(data.data[0]);
            index2 = code(data.data[1]) + 1;
            index3 = 0;
        }
        else
        {
            index1 = code(data.data[0]);
            index2 = 0;
            index3 = 0;
        }

        if (index1 < 0 || index2 < 0 || index3 < 0)
        {
            // cout << "index error" << endl;
            // pthread_mutex_unlock(&mutex_global);
            free(key1);
            return false;
        }
        // pthread_mutex_lock(&mutex_global);
        pthread_mutex_lock(&mutex[index1][index2][index3]);
        // printf("Index 1 %d index 2 %d for key: %s\n",index1,index2,data.data);
        bool ret = deleteNodeHelper(this->root[index1][index2][index3], key1, index1, index2, index3);
        pthread_mutex_unlock(&mutex[index1][index2][index3]);

        // pthread_mutex_unlock(&mutex_global);

        // pthread_mutex_unlock(&mutex_global);
        free(key1);
        return ret;
    }
    bool get(Slice &key, Slice &value)
    {
        char *key1 = (char *)malloc((key.size + 1) * (sizeof(char)));
        strncpy(key1, key.data, key.size);
        key1[key.size] = 0;
        //  malloc()
        // pthread_mutex_lock(&mutex_global);
        int index1, index2, index3;
        if (key.size > 2)
        {
            index1 = code(key.data[0]);
            index2 = code(key.data[1]) + 1;
            index3 = code(key.data[2]) + 1;
        }
        else if (key.size == 2)
        {
            index1 = code(key.data[0]);
            index2 = code(key.data[1]) + 1;
            index3 = 0;
        }
        else
        {
            index1 = code(key.data[0]);
            index2 = 0;
            index3 = 0;
        }

        if (index1 < 0 || index2 < 0 || index3 < 0)
        {
            // cout << "index error" << endl;
            // pthread_mutex_unlock(&mutex_global);
            free(key1);
            return false;
        }
        pthread_mutex_lock(&mutex[index1][index2][index3]);
        // printf("Index 1 %d index 2 %d for key: *%s* *%s*\n",index1,index2,key.data,key1);
        NodePtr temp = searchTreeHelper(this->root[index1][index2][index3], key1);
        if (temp == TNULL)
        {
            pthread_mutex_unlock(&mutex[index1][index2][index3]);
            // pthread_mutex_unlock(&mutex_global);
            free(key1);
            return false;
        }
        value.data = (char *)malloc(temp->value_size + 1);
        value.data = temp->value;
        value.size = temp->value_size;
        // printf("%d supplied %d noded\n",value.size,temp->value_size);
        pthread_mutex_unlock(&mutex[index1][index2][index3]);

        // pthread_mutex_unlock(&mutex_global);
        free(key1);
        return true;
    }
    // void printTree(int index1, int index2)
    // {
    //     if (this->root[index1][index2])
    //     {
    //         printHelper(this->root[index1][index2], "", true);
    //     }
    // }
    bool get(int N1, Slice &key, Slice &value)
    {

        // pthread_mutex_lock(&mutex_global);

        int N = N1 + 1;
        int index1 = 0;
        int index2 = 0;
        int index3 = 0;
        bool flag = false;
        for (int i = 0; i < 52; i++)
        {
            pthread_mutex_lock(&mutex_no);

            int number = first[i];

            pthread_mutex_unlock(&mutex_no);

            if (N - number <= 0)
            {
                index1 = i;
                break;
            }
            N -= number;
        }
        for(int i=0;i<53;i++)
        {
            pthread_mutex_lock(&mutex_no);
            int number = no_of_element[index1][i];
            pthread_mutex_unlock(&mutex_no);
            if(N - number <= 0)
            {
                index2 = i;
                break;
            }
            N-= number;
        }
        for (int j = 0; j < 53; j++)
        {
            pthread_mutex_lock(&mutex[index1][index2][j]);

            int number = 0;
            if (this->root[index1][index2][j] != TNULL && this->root[index1][index2][j]->data[0] != '\0')
            {
                number = this->root[index1][index2][j]->leftNo + this->root[index1][index2][j]->rightNo + 1;
            }

            pthread_mutex_unlock(&mutex[index1][index2][j]);

            index3 = j;
            if (N - number <= 0)
            {
                flag = true;
                break;
            }
            N -= number;
        }
        // printf("Index1 Index2 %d %d \n",index1,index2);
        if (flag == false)
        {
            // pthread_mutex_unlock(&mutex_global);
            return false;
        }

        if (index1 < 0 || index2 < 0 || index3 < 0)
        {
            // cout << "index error" << endl;
            // pthread_mutex_unlock(&mutex_global);
            return false;
        }
        pthread_mutex_lock(&mutex[index1][index2][index3]);
        NodePtr node = getTreeHelper(this->root[index1][index2][index3], N);

        if (node)
        {
            key.size = node->key_size;
            value.size = node->value_size;
            key.data = (char *)(malloc(1 + key.size));
            value.data = (char *)(malloc(1 + value.size));
            strncpy(key.data, node->data, key.size);
            strncpy(value.data, node->value, value.size);

            pthread_mutex_unlock(&mutex[index1][index2][index3]);
            // pthread_mutex_unlock(&mutex_global);
            return true;
        }

        pthread_mutex_unlock(&mutex[index1][index2][index3]);
        // pthread_mutex_unlock(&mutex_global);
        return false;
    }
    bool del(int N)
    {
        Slice *key = new Slice;
        Slice *value = new Slice;
        // key->data = new char[68];
        // value->data = new char[260];
        if (get(N, *key, *value) == true)
        {
            return del(*key);
        }
        return false;
    }
    int code(char c)
    {
        int val = (int)(c);
        if (val > 64 && val < 91)
        {
            return (val - 65);
        }
        else if (val > 96 && val < 123)
        {
            return (val - 97) + 26;
        }
        else
        {
            // since for index2 we add 1 to the return value
            return -2;
        }
    }

    // void debug()
    // {
    //     // for (int i = 0; i < 52; i++)
    //     // {
    //     int i = 0;
    //     for (int j = 0; j < 53; j++)
    //     {
    //         cout << i << ' ' << j << endl;
    //         printTree(i, j);
    //     }
    //     cout << endl;
    //     // }
    // }
};
// int main()
// {
//     kvStore bst;
//     // int n;
//     // cin >> n;
//     // char key[64];
//     // char value[256];
//     // Slice key,value;
//     Slice *key = new Slice;
//     Slice *value = new Slice;
//     key->data = new char[64];
//     value->data = new char[256];
//     // clock_gettime(CLOCK_MONOTONIC, &start);

//     for (int i = 0; i < 10000000; i++)
//     {
//         cin >> key->data >> value->data;
//         bst.put(*key, *value);
//     }
//     //bst.printTree();
//     // cout << endl
//     //      << "After deleting" << endl;
//     // //bst.deleteNode(13);
//     // bst.printTree();
// }
// int main()
// {
//     Slice *a = (Slice *)malloc(sizeof(Slice));
//     Slice *b = (Slice *)malloc(sizeof(Slice));

//     a->data = (char *)malloc(sizeof(char) * 64);
//     b->data = (char *)malloc(sizeof(char) * 256);

//     kvStore kv;
//     struct timespec start, end;
//     clock_gettime(CLOCK_MONOTONIC, &start);

//     char op[10];
//     // scanf("%s",op);
//     for (; 1;)
//     {
//         scanf("%s", op);

//         if (strcmp(op, "put") == 0)
//         {
//             scanf("%s", a->data);
//             scanf("%s", b->data);
//             // printf("%s %s\n", a->data, b->data);
//             bool ret = kv.put(*a, *b);
//             printf("ret - %d\n", ret);
//         }
//         else if (strcmp(op, "get") == 0)
//         {
//             scanf("%s", a->data);
//             bool ret = kv.get(*a, *b);
//             // printf("%d\n", ret);
//             fflush(stdout);
//             if (!ret)
//                 printf("ret - %d\n", ret);
//             else
//                 printf("ret - %d\tval - %s\n", ret, b->data);
//         }
//         else if (strcmp(op, "getn") == 0)
//         {
//             int index;
//             scanf("%d", &index);
//             bool ret = kv.get(index, *a, *b);
//             if (!ret)
//                 printf("ret - %d\n", ret);
//             else
//                 printf("ret - %d\tkey - %s\tval - %s\n", ret, a->data, b->data);
//         }
//         else if (strcmp(op, "del") == 0)
//         {
//             scanf("%s", a->data);
//             bool ret = kv.del(*a);
//             printf("ret - %d\n", ret);
//         }
//         else if (strcmp(op, "deln") == 0)
//         {
//             int index;
//             scanf("%d", &index);
//             bool ret = kv.del(index);
//             printf("ret - %d\n", ret);
//         }
//         else if (strcmp(op, "exit") == 0)
//         {
//             break;
//         }
//     }

// return 0;
// }

// int main()
// {
//     Slice *a = (Slice *)malloc(sizeof(Slice));
//     Slice *b = (Slice *)malloc(sizeof(Slice));

//     int n;
//     scanf("%d", &n);
//     kvStore kv;
//     struct timespec start, end;
//     clock_gettime(CLOCK_MONOTONIC, &start);

//     for (int i = 0; i < n; i++)
//     {

//         a->data = (char *)malloc(64 * sizeof(char));
//         scanf("%s", a->data);
//         b->data = (char *)malloc(256 * sizeof(char));
//         scanf("%s", b->data);
//         kv.put(a, b);
//     }
//     clock_gettime(CLOCK_MONOTONIC, &end);
//     double tdiff = (end.tv_sec - start.tv_sec) + 1e-9 * (end.tv_nsec - start.tv_nsec);
//     while (1)
//     {
//         Slice *key = (Slice *)malloc(sizeof(Slice));
//         Slice *value = (Slice *)malloc(sizeof(Slice));
//         int N;
//         // scanf("%d", &N);
//         key->data = (char *)malloc(sizeof(char) * 64);
//         scanf("%s", key->data);
//         // int N1=N;
//         kv.get(key, value);
//         // kv.printTree(26,27);
//         // kv.get(N, key, value);

//         kv.printTree(26, 27);
//     }
//     return 0;
// }
