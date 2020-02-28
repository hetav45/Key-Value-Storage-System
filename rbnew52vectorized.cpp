// Implementing Red-Black Tree in C++
#include <bits/stdc++.h>
#include <emmintrin.h>
#include <immintrin.h>
using namespace std;
struct Node
{
    char *data;
    char *value;
    Node *parent;
    Node *left;
    Node *right;
    int leftNo;
    int rightNo;
    int color;
};
void keycopy(char *b, char *a, int n)
{
    for (int i = 0; i < n; i++)
    {
        _mm256_storeu_si256((__m256i *)&b[i * 32], _mm256_loadu_si256((const __m256i *)&a[i * 32]));
    }
    b[n*32] = '\0';
}

void valuecopy(char *b, char *a, int n)
{
    for (int i = 0; i < n; i++)
    {
        _mm256_storeu_si256((__m256i *)&b[i * 32], _mm256_loadu_si256((const __m256i *)&a[i * 32]));
    }
    b[n*32] = '\0';
}
struct Slice
{
    uint8_t size;
    char *data;
};
int sse42_strcmp(const char *s1, const char *s2)
{

    if (s1 == s2)
    {
        return 0;
    }

    __m128i *ptr1 = reinterpret_cast<__m128i *>(const_cast<char *>(s1));
    __m128i *ptr2 = reinterpret_cast<__m128i *>(const_cast<char *>(s2));

    for (/**/; /**/; ptr1++, ptr2++)
    {

        const __m128i a = _mm_loadu_si128(ptr1);
        const __m128i b = _mm_loadu_si128(ptr2);

        const uint8_t mode =
            _SIDD_UBYTE_OPS |
            _SIDD_CMP_EQUAL_EACH |
            _SIDD_NEGATIVE_POLARITY |
            _SIDD_LEAST_SIGNIFICANT;

        if (_mm_cmpistrc(a, b, mode))
        {
            // a & b are different (not counting past-zero bytes)
            const auto idx = _mm_cmpistri(a, b, mode);

            const uint8_t b1 = (reinterpret_cast<char *>(ptr1))[idx];
            const uint8_t b2 = (reinterpret_cast<char *>(ptr2))[idx];

            if (b1 < b2)
            {
                return -1;
            }
            else if (b1 > b2)
            {
                return +1;
            }
            else
            {
                return 0;
            }
        }
        else if (_mm_cmpistrz(a, b, mode))
        {
            // a & b are same, but b contains a zero byte
            break;
        }
    }

    return 0;
}
typedef Node *NodePtr;
class kvStore
{
private:
    NodePtr root[52][53];
    NodePtr TNULL;
    int no_of_element[55];
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
    bool hasredchild(NodePtr node)
    {
        return (node->left != TNULL and node->left->color == 1) or (node->right != TNULL and node->right->color == 1);
    }
    bool isonleft(NodePtr node)
    {
        return node == node->parent->left;
    }
    NodePtr sibling(NodePtr node)
    {
        if (node->parent == TNULL)
        {
            return TNULL;
        }
        if (isonleft(node))
        {
            return node->parent->right;
        }
        return node->parent->right;
    }
    // void inOrderHelper(NodePtr node, int *j, int k, char *key, char *value)
    // {
    //     if (node != TNULL)
    //     {
    //         inOrderHelper(node->left, j, k, key, value);
    //         (*j)++;
    //         if (*j == k)
    //         {
    //             strcpy(key, node->data);
    //             strcpy(value, node->value);
    //             return;
    //         }
    //         inOrderHelper(node->right, j, k, key, value);
    //     }
    // }
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
        if (node == TNULL || sse42_strcmp(key, node->data) == 0)
        {
            return node;
        }
        if (sse42_strcmp(key, node->data) < 0)
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
    void deleteFix(NodePtr x, int index1, int index2)
    {
        NodePtr s;
        while (x != root[index1][index2] && x->color == 0)
        {
            if (x == x->parent->left)
            {
                s = x->parent->right;
                if (s == TNULL)
                    break;
                if (s->color == 1)
                {
                    s->color = 0;
                    x->parent->color = 1;
                    leftRotate(x->parent, index1, index2);
                    s = x->parent->right;
                }
                else if (s->left->color == 0 && s->right->color == 0)
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
                if (s == TNULL)
                    break;
                if (s->color == 1)
                {
                    s->color = 0;
                    x->parent->color = 1;
                    rightRotate(x->parent, index1, index2);
                    s = x->parent->left;
                }
                else if (s->right->color == 0 && s->right->color == 0)
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
            if (sse42_strcmp(node->data, key) == 0)
            {
                z = node;
            }
            if (sse42_strcmp(node->data, key) <= 0)
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
    NodePtr rbreplace(NodePtr x)
    {
        if (x->left != TNULL && x->right != TNULL)
        {
            return succ(x->right);
        }
        if (x->left == TNULL && x->right == TNULL)
        {
            return TNULL;
        }
        if (x->left != TNULL)
        {
            return x->left;
        }
        else
        {
            return x->right;
        }
    }

    void fixdoubleblack(NodePtr x, int index1, int index2)
    {
        if (x == root[index1][index2])
        {
            return;
        }
        NodePtr sib = sibling(x);
        NodePtr parent = x->parent;
        if (sib == TNULL)
        {
            fixdoubleblack(parent, index1, index2);
        }
        else
        {
            if (sib->color == 1)
            {
                parent->color = 1;
                sib->color = 0;
                if (isonleft(sib))
                {
                    rightRotate(parent, index1, index2);
                }
                else
                {
                    leftRotate(parent, index1, index2);
                }
                fixdoubleblack(x, index1, index2);
            }
            else
            {
                if (hasredchild(sib))
                {
                    if (sib->left != TNULL and sib->left->color == 1)
                    {
                        if (isonleft(sib))
                        {
                            sib->left->color = sib->color;
                            sib->color = parent->color;
                            rightRotate(parent, index1, index2);
                        }
                        else
                        {
                            sib->left->color = sib->color;
                            rightRotate(sib, index1, index2);
                            leftRotate(parent, index1, index2);
                        }
                    }
                    else
                    {
                        if (isonleft(sib))
                        {
                            sib->right->color = parent->color;
                            leftRotate(sib, index1, index2);
                            rightRotate(parent, index1, index2);
                        }
                        else
                        {
                            sib->right->color = sib->color;
                            sib->color = parent->color;
                            leftRotate(parent, index1, index2);
                        }
                    }
                    parent->color = 0;
                }
                else
                {
                    sib->color = 1;
                    if (parent->color == 0)
                    {
                        fixdoubleblack(parent, index1, index2);
                    }
                    else
                    {
                        parent->color = 0;
                    }
                }
            }
        }
    }
    bool deleteNodeHelper(NodePtr node, char *key, int index1, int index2)
    {
        NodePtr z = TNULL;
        NodePtr x, y;
        NodePtr temp = searchTreeHelper(node, key);
        if (temp != TNULL)
        {
            no_of_element[index1]--;
            while (node != TNULL)
            {
                if (sse42_strcmp(node->data, key) == 0)
                {
                    z = node;
                    break;
                }
                if (sse42_strcmp(node->data, key) < 0)
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
                if (sse42_strcmp(node->data, key) == 0)
                {
                    z = node;
                }
                if (sse42_strcmp(node->data, key) <= 0)
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

public:
    kvStore()
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
        for (int i = 0; i < 53; i++)
        {
            no_of_element[i] = 0;
        }
    }
    NodePtr current[1000];
    int len = 0;
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
    bool put(Slice &key, Slice &value)
    {

        int index1, index2;
        if (key.data[1] != '\0')
        {
            index1 = code(key.data[0]);
            index2 = code(key.data[1]) + 1;
        }
        else
        {
            index1 = code(key.data[0]);
            index2 = 0;
        }
        NodePtr temp = searchTreeHelper(this->root[index1][index2], key.data);
        if (temp == TNULL)
        {
            NodePtr node = alloc();
            no_of_element[index1]++;
            int k = ((key.size - 1) / 32 + 1);
            int v = ((value.size - 1) / 32 + 1);
            node->parent = nullptr;
            node->data = new char[k * 32 + 1];
            node->value = new char[v * 32 + 1];
            keycopy(node->data, key.data,k);
            valuecopy(node->value, value.data,v);
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
                if (sse42_strcmp(node->data, x->data) < 0)
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
                root[index1][index2] = node;
            }
            else if (sse42_strcmp(node->data, y->data) < 0)
            {
                y->left = node;
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
        else
        {
            NodePtr node = alloc();
            int k = ((key.size - 1) / 32 + 1);
            int v = ((value.size - 1) / 32 + 1);
            node->parent = nullptr;
            node->data = new char[k * 32 + 1];
            node->value = new char[v * 32 + 1];
            keycopy(node->data, key.data,k);
            valuecopy(node->value, value.data,v);
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
                if (sse42_strcmp(node->data, x->data) < 0)
                {
                    x = x->left;
                }
                else if (sse42_strcmp(node->data, x->data) == 0)
                {
                    valuecopy(node->value, value.data,((value.size - 1) / 32 + 1));
                    return true;
                }
                else
                {
                    x = x->right;
                }
            }
            return false;
        }
    }
    NodePtr alloc()
    {
        if (len != 1000 || len != 0)
        {
            return current[len++];
        }
        else
        {
            for (int i = 0; i < 1000; i++)
            {
                current[i] = new Node;
            }
            len = 0;
            return current[len++];
        }
    }
    bool del(Slice &data)
    {
        int index1, index2;
        if (data.data[1] != '\0')
        {
            index1 = code(data.data[0]);
            index2 = code(data.data[1]) + 1;
        }
        else
        {
            index1 = code(data.data[0]);
            index2 = 0;
        }
        return deleteNodeHelper(this->root[index1][index2], data.data, index1, index2);
    }
    bool get(Slice &key, Slice &value)
    {
        int index1, index2;
        if (key.data[1] != '\0')
        {
            index1 = code(key.data[0]);
            index2 = code(key.data[1]) + 1;
        }
        else
        {
            index1 = code(key.data[0]);
            index2 = 0;
        }
        NodePtr temp = searchTreeHelper(this->root[index1][index2], key.data);
        if (temp == TNULL)
            return false;
        value.data = temp->value;
        return true;
    }
    void printTree(int index1, int index2)
    {
        if (this->root[index1][index2])
        {
            printHelper(this->root[index1][index2], "", true);
        }
    }
    bool get(int N1, Slice &key, Slice &value)
    {
        int N = N1;
        int index1 = 0;
        int index2 = 0;
        bool flag = false;
        for (int i = 0; i < 52; i++)
        {
            int number = no_of_element[i];
            if (N - number <= 0)
            {
                index1 = i;
                break;
            }
            N -= number;
        }
        for (int j = 0; j < 53; j++)
        {
            int number = 0;
            if (this->root[index1][j] != TNULL && this->root[index1][j]->data[0] != '\0')
            {
                number = this->root[index1][j]->leftNo + this->root[index1][j]->rightNo + 1;
            }
            index2 = j;
            if (N - number <= 0)
            {
                flag = true;
                break;
            }
            N -= number;
        }
        if (flag == false)
        {
            return false;
        }
        NodePtr node = getTreeHelper(this->root[index1][index2], N);
        if (node)
        {
            strcpy(key.data, node->data);
            strcpy(value.data, node->value);
            return true;
        }

        return false;
    }
    bool del(int N)
    {
        Slice *key = new Slice;
        Slice *value = new Slice;
        key->data = new char[68];
        value->data = new char[260];
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
        else
        {
            return (val - 97) + 26;
        }
    }
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
