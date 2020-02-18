// Implementing Red-Black Tree in C++
#include <bits/stdc++.h>
using namespace std;
struct Node
{
    char data[64];
    char value[256];
    Node *parent;
    Node *left;
    Node *right;
    int leftNo;
    int rightNo;
    int color;
};
struct Slice
{
    uint8_t size;
    char data[256];
};
typedef Node *NodePtr;
class RedBlackTree
{
private:
    NodePtr root[52][53];
    NodePtr TNULL;
    void initializeNULLNode(NodePtr node, NodePtr parent)
    {
        node->parent = parent;
        node->left = nullptr;
        node->right = nullptr;
        node->leftNo = 0;
        node->rightNo = 0;
        node->color = 0;
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
    void inOrderHelper(NodePtr node)
    {
        if (node != TNULL)
        {
            inOrderHelper(node->left);
            // cout << node->data << " ";
            inOrderHelper(node->right);
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
        // printf("N = %d left = %d right = %d\n",N,node->leftNo,node->rightNo);
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
    void deleteFix(NodePtr x, int index1, int index2)
    {
        NodePtr s;
        while (x != root[index1][index2] && x->color == 0)
        {
            if (x == x->parent->left)
            {
                s = x->parent->right;
                if (s->color == 1)
                {
                    s->color = 0;
                    x->parent->color = 1;
                    leftRotate(x->parent, index1, index2);
                    s = x->parent->right;
                }
                if (s->left->color == 0 && s->right->color == 0)
                {
                    s->color = 1;
                    x = x->parent;
                }
                else
                {
                    if (s->right->color == 0)
                    {
                        s->left->color = 0;
                        s->color = 1;
                        rightRotate(s, index1, index2);
                        s = x->parent->right;
                    }
                    s->color = x->parent->color;
                    x->parent->color = 0;
                    s->right->color = 0;
                    leftRotate(x->parent, index1, index2);
                    x = root[index1][index2];
                }
            }
            else
            {
                s = x->parent->left;
                if (s->color == 1)
                {
                    s->color = 0;
                    x->parent->color = 1;
                    rightRotate(x->parent, index1, index2);
                    s = x->parent->left;
                }
                if (s->right->color == 0 && s->right->color == 0)
                {
                    s->color = 1;
                    x = x->parent;
                }
                else
                {
                    if (s->left->color == 0)
                    {
                        s->right->color = 0;
                        s->color = 1;
                        leftRotate(s, index1, index2);
                        s = x->parent->left;
                    }
                    s->color = x->parent->color;
                    x->parent->color = 0;
                    s->left->color = 0;
                    rightRotate(x->parent, index1, index2);
                    x = root[index1][index2];
                }
            }
        }
        x->color = 0;
    }
    void rbTransplant(NodePtr u, NodePtr v, int index1, int index2)
    {
        if (u->parent == nullptr)
        {
            root[index1][index2] = v;
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
            // cout << "Key not found in the tree" << endl;
            return NULL;
        }
        y = z;
        return y->value;
    }
    bool deleteNodeHelper(NodePtr node, char *key, int index1, int index2)
    {
        NodePtr z = TNULL;
        NodePtr x, y;
        NodePtr temp = searchTreeHelper(node, key);
        if (temp != TNULL)
        {
            while (node != TNULL)
            {
                if (strcmp(node->data, key) == 0)
                {
                    z = node;
                    break;
                }
                if (strcmp(node->data, key) <= 0)
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
            if (z == TNULL)
            {
                return false;
            }
            y = z;
            int y_original_color = y->color;
            if (z->left == TNULL)
            {
                x = z->right;
                rbTransplant(z, z->right, index1, index2);
            }
            else if (z->right == TNULL)
            {
                x = z->left;
                rbTransplant(z, z->left, index1, index2);
            }
            else
            {
                y = minimum(z->right);
                y_original_color = y->color;
                x = y->right;
                if (y->parent == z)
                {
                    x->parent = y;
                }
                else
                {
                    rbTransplant(y, y->right, index1, index2);
                    y->right = z->right;
                    y->rightNo = z->rightNo - 1;
                    y->right->parent = y;
                }
                rbTransplant(z, y, index1, index2);
                y->left = z->left;
                y->leftNo = z->leftNo;
                y->left->parent = y;
                y->color = z->color;
            }
            delete z;
            if (y_original_color == 0)
            {
                deleteFix(x, index1, index2);
            }
            // printTree(index1,index2);
            return true;
        }
        else
        {
            return false;
        }
    }
    void insertFix(NodePtr k, int index1, int index2)
    {
        NodePtr u;
        while (k->parent->color == 1)
        {
            if (k->parent == k->parent->parent->right)
            {
                u = k->parent->parent->left;
                if (u->color == 1)
                {
                    u->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                }
                else
                {
                    if (k == k->parent->left)
                    {
                        k = k->parent;
                        rightRotate(k, index1, index2);
                    }
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    leftRotate(k->parent->parent, index1, index2);
                }
            }
            else
            {
                u = k->parent->parent->right;
                if (u->color == 1)
                {
                    u->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                }
                else
                {
                    if (k == k->parent->right)
                    {
                        k = k->parent;
                        leftRotate(k, index1, index2);
                    }
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    rightRotate(k->parent->parent, index1, index2);
                }
            }
            if (k == root[index1][index2])
            {
                break;
            }
        }
        root[index1][index2]->color = 0;
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
            string sColor = root->color ? "RED" : "BLACK";
            cout << root->data << "(" << sColor << ")  " << root->leftNo << "   " << root->rightNo << endl;
            printHelper(root->left, indent, false);
            printHelper(root->right, indent, true);
        }
    }
    // pair<char *, char *> *getNHelper(NodePtr root1, int k)
    // {
    //     // NodePtr root = root1;
    //     pair<char *, char *> *ret = (pair<char *, char *> *)malloc(sizeof(pair<char *, char *>));
    //     ret->first = (char *)malloc(sizeof(char) * 64);
    //     ret->second = (char *)malloc(sizeof(char) * 256);
    //     NodePtr node = getTreeHelper(root1,k);
    //     strcpy(ret->first,node->data);
    //     strcpy(ret->second,node->value);
    //     return ret;
    // }

public:
    RedBlackTree()
    {
        TNULL = new Node;
        TNULL->color = 0;
        TNULL->left = nullptr;
        TNULL->right = nullptr;
        for (int i = 0; i < 52; i++)
        {
            for (int j = 0; j < 53; j++)
            {
                root[i][j] = TNULL;
            }
        }
        // root = TNULL;
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
    void leftRotate(NodePtr x, int index1, int index2)
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
            this->root[index1][index2] = y;
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
    void rightRotate(NodePtr x, int index1, int index2)
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
            this->root[index1][index2] = y;
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
    bool put(Slice *key, Slice *value)
    {
        int index1, index2;
        if (key->data[1] != '\0')
        {
            index1 = code(key->data[0]);
            index2 = code(key->data[1]) + 1;
        }
        else
        {
            index1 = code(key->data[0]);
            index2 = 0;
        }
        NodePtr temp = searchTreeHelper(this->root[index1][index2], key->data);
        if (temp == TNULL)
        {
            // printf("%d %d\n",index1,index2);
            NodePtr node = new Node;
            node->parent = nullptr;
            strcpy(node->data, key->data);
            strcpy(node->value, value->data);
            node->left = TNULL;
            node->right = TNULL;
            node->color = 1;
            node->leftNo = 0;
            node->rightNo = 0;
            NodePtr y = nullptr;
            NodePtr x = this->root[index1][index2];
            while (x != TNULL)
            {
                y = x;
                if (strcmp(node->data, x->data) < 0)
                {
                    x->leftNo += 1;
                    x = x->left;
                }
                else if (strcmp(node->data, x->data) == 0)
                {
                    break;
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
                root[index1][index2] = node;
            }
            else if (strcmp(node->data, y->data) < 0)
            {
                y->left = node;
            }
            else if (strcmp(node->data, y->data) == 0)
            {
                strcpy(y->value, value->data);
                // printTree(index1, index2);
                return true;
            }
            else
            {

                y->right = node;
            }
            if (node->parent == nullptr)
            {
                node->color = 0;
                // printTree(index1, index2);

                return false;
            }
            if (node->parent->parent == nullptr)
            {
                // printTree(index1, index2);

                return false;
            }
            insertFix(node, index1, index2);
            // printTree(index1, index2);

            return false;
        }
        else
        {
            NodePtr node = new Node;
            node->parent = nullptr;
            strcpy(node->data, key->data);
            strcpy(node->value, value->data);
            node->left = TNULL;
            node->right = TNULL;
            node->color = 1;
            node->leftNo = 0;
            node->rightNo = 0;
            NodePtr y = nullptr;
            NodePtr x = this->root[index1][index2];
            while (x != TNULL)
            {
                y = x;
                if (strcmp(node->data, x->data) < 0)
                {
                    x = x->left;
                }
                else if (strcmp(node->data, x->data) == 0)
                {
                    strcpy(y->value, value->data);
                    // printTree(index1, index2);
                    return true;
                }
                else
                {
                    x = x->right;
                }
            }
        }
    }

    bool del(Slice *data)
    {
        int index1, index2;
        if (data->data[1] != '\0')
        {
            index1 = code(data->data[0]);
            index2 = code(data->data[1]) + 1;
        }
        else
        {
            index1 = code(data->data[0]);
            index2 = 0;
        }
        return deleteNodeHelper(this->root[index1][index2], data->data, index1, index2);
    }
    bool get(Slice *key, Slice *value)
    {
        int index1, index2;
        if (key->data[1] != '\0')
        {
            index1 = code(key->data[0]);
            index2 = code(key->data[1]) + 1;
        }
        else
        {
            index1 = code(key->data[0]);
            index2 = 0;
        }
        NodePtr temp = searchTreeHelper(this->root[index1][index2], key->data);
        if (temp == TNULL)
            return false;
        strcpy(value->data, temp->value);

        return true;
    }
    void printTree(int index1, int index2)
    {
        if (this->root[index1][index2])
        {
            printHelper(this->root[index1][index2], "", true);
        }
    }
    bool get(int N1, Slice *key, Slice *value)
    {
        int N = N1;
        int index1;
        int index2;
        bool flag = false;
        for (int i = 0; i < 52; i++)
        {
            for (int j = 0; j < 53; j++)
            {
                int number = 0;
                if (this->root[i][j] != TNULL && this->root[i][j]->data[0] != '\0')
                {
                    // printf("i=%d j=%d\n",i,j);
                    number = this->root[i][j]->leftNo + this->root[i][j]->rightNo + 1;
                }
                index1 = i;
                index2 = j;
                if (N - number <= 0)
                {
                    flag = true;
                    break;
                }
                N -= number;
            }
            if (flag == true)
            {
                break;
            }
        }
        if (flag == false)
        {
            return false;
        }
        // remaining error handling
        //
        // pair<char *, char *> *val = getNHelper(this->root[index1][index2], N);
        NodePtr node = getTreeHelper(this->root[index1][index2], N);
        if (node)
        {
            strcpy(key->data, node->data);
            strcpy(value->data, node->value);
            return true;
        }

        // key->size = strlen(node->data);
        // value->size = strlen(node->value);

        return false;
    }
    bool del(int N)
    {
        int index1;
        int index2;
        bool flag = false;

        for (int i = 0; i < 52; i++)
        {
            for (int j = 0; j < 53; j++)
            {
                int number = 0;
                if (root[i][j] != TNULL)
                {
                    number = root[i][j]->leftNo + root[i][j]->rightNo + 1;
                }
                index1 = i;
                index2 = j;
                if (N - number <= 0)
                {
                    flag = true;
                    break;
                }
                N -= number;
            }
            if (flag == true)
            {
                break;
            }
        }
        if (flag == false)
        {
            return false;
        }
        NodePtr node = getTreeHelper(this->root[index1][index2], N);
        return deleteNodeHelper(this->root[index1][index2], node->data, index1, index2);
    }
    int code(char c)
    {
        int val = (int)(c);
        if (val > 64 && val < 91)
        {
            return (val - 65);
        }
        else
        {
            return (val - 97) + 26;
        }
    }
};
// int main()
// {
//     Slice *a = (Slice *)malloc(sizeof(Slice));
//     Slice *b = (Slice *)malloc(sizeof(Slice));

//     // a->data = (char *)malloc(sizeof(char)*64);
//     // b->data = (char *)malloc(sizeof(char)*256);

//     RedBlackTree kv;
//     struct timespec start, end;
//     clock_gettime(CLOCK_MONOTONIC, &start);

//     char op[10];

//     for (; 1;)
//     {
//         scanf("%s", op);

//         if (strcmp(op, "put") == 0)
//         {
//             scanf("%s", a->data);
//             scanf("%s", b->data);
//             // printf("%s %s\n", a->data, b->data);
//             bool ret = kv.put(a, b);
//             printf("ret - %d\n", ret);
//         }
//         else if (strcmp(op, "get") == 0)
//         {
//             scanf("%s", a->data);
//             bool ret = kv.get(a, b);
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
//             bool ret = kv.get(index, a, b);
//             if (!ret)
//                 printf("ret - %d\n", ret);
//             else
//                 printf("ret - %d\tkey - %s\tval - %s\n", ret, a->data, b->data);
//         }
//         else if (strcmp(op, "del") == 0)
//         {
//             scanf("%s", a->data);
//             bool ret = kv.del(a);
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

//     return 0;
// }

// int main()
// {
//     Slice *a = (Slice *)malloc(sizeof(Slice));
//     Slice *b = (Slice *)malloc(sizeof(Slice));

//     int n;
//     scanf("%d", &n);
//     RedBlackTree kv;
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
