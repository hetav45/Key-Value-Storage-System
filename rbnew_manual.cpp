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
    char *data;
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
            cout << node->data << " ";
            preOrderHelper(node->left);
            preOrderHelper(node->right);
        }
    }
    void inOrderHelper(NodePtr node)
    {
        if (node != TNULL)
        {
            inOrderHelper(node->left);
            cout << node->data << " ";
            inOrderHelper(node->right);
        }
    }
    void postOrderHelper(NodePtr node)
    {
        if (node != TNULL)
        {
            postOrderHelper(node->left);
            postOrderHelper(node->right);
            cout << node->data << " ";
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
        // printf("IN func %s, data %s\n",node->data,root[26][27]->data);
        if (node == TNULL || ((node->leftNo + 1) == N))
        {
            return node;
        }
        else if (N > node->leftNo)
        {
            return getTreeHelper(node->right, N - node->leftNo + 1);
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
            //cout << "Key not found in the tree" << endl;
            return NULL;
        }
        y = z;
        return y->value;
    }
    bool deleteNodeHelper(NodePtr node, char *key, int index1, int index2)
    {
        NodePtr z = TNULL;
        NodePtr x, y;
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
            // cout << "Key not found in the tree" << endl;
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
        return true;
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
    // void preorder()
    // {
    //     preOrderHelper(this->root);
    // }
    // void inorder()
    // {
    //     inOrderHelper(this->root);
    // }
    // void postorder()
    // {
    //     postOrderHelper(this->root);
    // }
    // NodePtr searchTree(char *k)
    // {
    //     return searchTreeHelper(this->root, k);
    // }
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
            return true;
        }
        else
        {
            y->right = node;
        }
        if (node->parent == nullptr)
        {
            node->color = 0;
            return false;
        }
        if (node->parent->parent == nullptr)
        {
            return false;
        }
        insertFix(node, index1, index2);
        return false;
    }

    bool del(Slice *data)
    {
        int index1 = code(data->data[0]);
        int index2 = code(data->data[1]) + 1;

        return deleteNodeHelper(this->root[index1][index2], data->data, index1, index2);
    }
    bool get(Slice *key, Slice *value)
    {

        int index1 = code(key->data[0]);
        int index2 = code(key->data[1]) + 1;
        value->data = searchHelper(this->root[index1][index2], key->data);
        value->size = strlen(value->data);
        if (value->size)
            return true;
        else
            return false;
    }
    void printTree(int index1, int index2)
    {
        if (this->root[index1][index2])
        {
            printHelper(this->root[index1][index2], "", true);
        }
    }
    // int getNumber()
    // {
    //     if (root != TNULL)
    //     {
    //         return root->leftNo + root->rightNo + 1;
    //     }
    //     else
    //     {
    //         return 0;
    //     }
    // }
    bool get(int N1, Slice *key, Slice *value)
    {
        // printf("inititila %s %s\n",root[26][27]->data,key->data);
        int N = N1;
        int index1;
        int index2;
        bool flag;
        for (int i = 0; i < 52; i++)
        {
            flag = false;
            for (int j = 0; j < 53; j++)
            {
                int number = 0;
                if (this->root[i][j] != TNULL)
                {
                    number = this->root[i][j]->leftNo + this->root[i][j]->rightNo + 1;
                    // printf("\n\nblaha aba %d %d %d\n\n",number,i,j);
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
        // printf("ind 1 %d ind 2 %d \n",index1,index2);
        // pair<char *, char *> *val = getNHelper(this->root[index1][index2], N);
        // printf("root check %s\n",this->root[index1][index2]->data);
        NodePtr node = getTreeHelper(this->root[index1][index2], N);

        key->size = strlen(node->data);
        key->data = (char *)malloc(sizeof(char) * key->size);
        strcpy(key->data, node->data);
        value->size = strlen(node->value);
        value->data = (char *)malloc(sizeof(char) * value->size);
        // value->data = val->second;
        strcpy(value->data, node->value);
        // printf("final %s\n",root[26][27]->data);

        if (key->data)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    bool del(int N)
    {
        int index1;
        int index2;
        for (int i = 0; i < 52; i++)
        {
            bool flag = false;
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
        NodePtr node = getTreeHelper(this->root[index1][index2], N);
        printf("\n\n\n%s\n\n\n", node->data);
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

int main()
{
    Slice *a = (Slice *)malloc(sizeof(Slice));
    Slice *b = (Slice *)malloc(sizeof(Slice));

    a->data = (char *)malloc(sizeof(char)*64);
    b->data = (char *)malloc(sizeof(char)*256);

    RedBlackTree kv;
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    char op[10];

    for (;1;)
    {
        scanf("%s", op);

        if (strcmp(op, "put") == 0)
        { 
            scanf("%s", a->data);
            scanf("%s", b->data);
            bool ret = kv.put(a, b);
            printf("ret - %d\n", ret);
        }
        else if( strcmp(op, "get") == 0)
        {
            scanf("%s", a->data);
            bool ret = kv.get(a, b);
            printf("%d\n", ret);
            fflush(stdout);
            if (!ret) 
                printf("ret - %d\n", ret);
            else 
                printf("ret - %d\tval - %s\n", ret, b->data);
        }
        else if( strcmp(op, "getn") == 0)
        {
            int index;
            scanf("%d", &index); 
            bool ret = kv.get(index, a, b);
            if (!ret)
                printf("ret - %d\n", ret);
            else 
                printf("ret - %d\tkey - %s\tval - %s\n", ret, a->data, b->data);
        }
        else if( strcmp(op, "del") == 0)
        {
            scanf("%s", a->data);
            bool ret = kv.del(a);
            printf("ret - %d\n", ret);
        }
        else if( strcmp(op, "deln") == 0) 
        {
            int index;
            scanf("%d", &index);
            bool ret = kv.del(index);
            printf("ret - %d\n", ret);
        }
        else if(strcmp(op, "exit") == 0) 
        {
            break;
        }
    }
    
    return 0;
}
