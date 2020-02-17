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
typedef Node *NodePtr;
class RedBlackTree
{
private:
    NodePtr root;
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
    void deleteFix(NodePtr x)
    {
        NodePtr s;
        while (x != root && x->color == 0)
        {
            if (x == x->parent->left)
            {
                s = x->parent->right;
                if (s->color == 1)
                {
                    s->color = 0;
                    x->parent->color = 1;
                    leftRotate(x->parent);
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
                        rightRotate(s);
                        s = x->parent->right;
                    }
                    s->color = x->parent->color;
                    x->parent->color = 0;
                    s->right->color = 0;
                    leftRotate(x->parent);
                    x = root;
                }
            }
            else
            {
                s = x->parent->left;
                if (s->color == 1)
                {
                    s->color = 0;
                    x->parent->color = 1;
                    rightRotate(x->parent);
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
                        leftRotate(s);
                        s = x->parent->left;
                    }
                    s->color = x->parent->color;
                    x->parent->color = 0;
                    s->left->color = 0;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = 0;
    }
    void rbTransplant(NodePtr u, NodePtr v)
    {
        if (u->parent == nullptr)
        {
            root = v;
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
            cout << "Key not found in the tree" << endl;
            return NULL;
        }
        y = z;
        return y->value;
    }
    void deleteNodeHelper(NodePtr node, char *key)
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
            cout << "Key not found in the tree" << endl;
            return;
        }
        y = z;
        int y_original_color = y->color;
        if (z->left == TNULL)
        {
            x = z->right;
            rbTransplant(z, z->right);
        }
        else if (z->right == TNULL)
        {
            x = z->left;
            rbTransplant(z, z->left);
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
                rbTransplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            rbTransplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;
        if (y_original_color == 0)
        {
            deleteFix(x);
        }
    }
    void insertFix(NodePtr k)
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
                        rightRotate(k);
                    }
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    leftRotate(k->parent->parent);
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
                        leftRotate(k);
                    }
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    rightRotate(k->parent->parent);
                }
            }
            if (k == root)
            {
                break;
            }
        }
        root->color = 0;
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
    RedBlackTree()
    {
        TNULL = new Node;
        TNULL->color = 0;
        TNULL->left = nullptr;
        TNULL->right = nullptr;
        root = TNULL;
    }
    void preorder()
    {
        preOrderHelper(this->root);
    }
    void inorder()
    {
        inOrderHelper(this->root);
    }
    void postorder()
    {
        postOrderHelper(this->root);
    }
    NodePtr searchTree(char *k)
    {
        return searchTreeHelper(this->root, k);
    }
    NodePtr minimum(NodePtr node)
    {
        while (node->left != TNULL)
        {
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
    void leftRotate(NodePtr x)
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
            this->root = y;
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
    void rightRotate(NodePtr x)
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
            this->root = y;
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
    void insert(char *key, char *value)
    {
        NodePtr node = new Node;
        node->parent = nullptr;
        strcpy(node->data, key);
        strcpy(node->value, value);
        node->left = TNULL;
        node->right = TNULL;
        node->color = 1;
        node->leftNo = 0;
        node->rightNo = 0;
        NodePtr y = nullptr;
        NodePtr x = this->root;
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
            root = node;
        }
        else if (strcmp(node->data, y->data) < 0)
        {
            y->left = node;
        }
        else if (strcmp(node->data, y->data) == 0)
        {
            strcpy(y->value, value);
        }
        else
        {
            y->right = node;
        }
        if (node->parent == nullptr)
        {
            node->color = 0;
            return;
        }
        if (node->parent->parent == nullptr)
        {
            return;
        }
        insertFix(node);
    }
    NodePtr getRoot()
    {
        return this->root;
    }
    void deleteNode(char *data)
    {
        deleteNodeHelper(this->root, data);
    }
    char *search(char *key)
    {
        return searchHelper(this->root, key);
    }
    void printTree()
    {
        if (root)
        {
            printHelper(this->root, "", true);
        }
    }
};
int code(char c)
{
    int val = (int)(c);
    if (val > 64 && val < 91)
    {
        return (val - 65) * 2 + 1;
    }
    else
    {
        return (val - 97) * 2;
    }
}
int main()
{
    char *a, *b;
    int n;
    scanf("%d", &n);
    RedBlackTree bst[52][64];
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < n; i++)
    {
        a = (char *)malloc(64 * sizeof(char));
        scanf("%s", a);
        b = (char *)malloc(256 * sizeof(char));
        scanf("%s", b);
        bst[code(a[0])][strlen(a) - 1].insert(a, b);
        // printf("Value is %s\n", bst[code(a[0])][strlen(a) - 1].search(a));
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    double tdiff = (end.tv_sec - start.tv_sec) + 1e-9 * (end.tv_nsec - start.tv_nsec);
    printf("time %f\n", tdiff);
    for (int i = 0; i < 52; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            bst[i][j].printTree();
        }
    }
    return 0;
}
