#include <iostream>
#include <string.h>
using namespace std;
int hashtable[64];
struct Node
{
    char data[65];
    Node *parent;
    Node *left;
    Node *right;
    char value[257];
    int color;
};
typedef Node *NodePtr;
struct Mempiece
{
    Node *root;
    int used;
} mempiece;
int NUMBER = 100;
int lexhash[52];
class RedBlackTree
{
private:
    NodePtr root[52];
    NodePtr TNULL;
    void initializeNULLNode(NodePtr node, NodePtr parent)
    {
        //node->data;
        node->parent = parent;
        node->left = nullptr;
        node->right = nullptr;
        //node->value[0] = '\0';
        node->color = 0;
    }
    // void preOrderHelper(NodePtr node)
    // {
    //     if (node != TNULL)
    //     {
    //         cout << node->data << " ";
    //         preOrderHelper(node->left);
    //         preOrderHelper(node->right);
    //     }
    // }
    void inOrderHelper(NodePtr node, int k, int *j, string *w, string *v)
    {
        if (node != TNULL)
        {
            inOrderHelper(node->left, k, j, w, v);
            (*j)++;
            //cout << node->data <<endl;
            if (*j == k)
            {
                 string temp1(node->data);
                 string temp2(node->value);
                 *w = temp1;
                 *v = temp2;
                // cout << node->data << endl;
                // cout << node->value << endl;
                // cout << k << endl;

                return;
            }
            //cout << node->data << " ";
            inOrderHelper(node->right, k, j, w, v);
        }
    }
    // void postOrderHelper(NodePtr node)
    // {
    //     if (node != TNULL)
    //     {
    //         postOrderHelper(node->left);
    //         postOrderHelper(node->right);
    //         cout << node->data << " ";
    //     }
    // }
    NodePtr searchTreeHelper(NodePtr node, char key[])
    {
        int l = strcmp(key, node->data);
        if (node == TNULL || l == 0)
        {
            return node;
        }
        if (l < 0)
        {
            return searchTreeHelper(node->left, key);
        }
        return searchTreeHelper(node->right, key);
    }
    void deleteFix(NodePtr x, int i)
    {
        NodePtr s;
        while (x != root[i] && x->color == 0)
        {
            if (x == x->parent->left)
            {
                s = x->parent->right;
                if (s->color == 1)
                {
                    s->color = 0;
                    x->parent->color = 1;
                    leftRotate(x->parent, i);
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
                        rightRotate(s, i);
                        s = x->parent->right;
                    }
                    s->color = x->parent->color;
                    x->parent->color = 0;
                    s->right->color = 0;
                    leftRotate(x->parent, i);
                    x = root[i];
                }
            }
            else
            {
                s = x->parent->left;
                if (s->color == 1)
                {
                    s->color = 0;
                    x->parent->color = 1;
                    rightRotate(x->parent, i);
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
                        leftRotate(s, i);
                        s = x->parent->left;
                    }
                    s->color = x->parent->color;
                    x->parent->color = 0;
                    s->left->color = 0;
                    rightRotate(x->parent, i);
                    x = root[i];
                }
            }
        }
        x->color = 0;
    }
    void rbTransplant(NodePtr u, NodePtr v, int i)
    {
        if (u->parent == nullptr)
        {
            root[i]= v;
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
    bool deleteNodeHelper(NodePtr node, char key[], int i)
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
            return false;
        }
        y = z;
        int y_original_color = y->color;
        if (z->left == TNULL)
        {
            x = z->right;
            rbTransplant(z, z->right, i);
        }
        else if (z->right == TNULL)
        {
            x = z->left;
            rbTransplant(z, z->left, i);
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
                rbTransplant(y, y->right, i);
                y->right = z->right;
                y->right->parent = y;
            }
            rbTransplant(z, y, i);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        //delete z;
        if (y_original_color == 0)
        {
            deleteFix(x, i);
        }
        return true;
    }
    void insertFix(NodePtr k, int i)
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
                        rightRotate(k, i);
                    }
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    leftRotate(k->parent->parent, i);
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
                        leftRotate(k, i);
                    }
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    rightRotate(k->parent->parent, i);
                }
            }
            if (k == root[i])
            {
                break;
            }
        }
        root[i]->color = 0;
    }
    // void printHelper(NodePtr root, string indent, bool last)
    // {
    //     if (root != TNULL)
    //     {
    //         cout << indent;
    //         if (last)
    //         {
    //             cout << "R----";
    //             indent += "   ";
    //         }
    //         else
    //         {
    //             cout << "L----";
    //             indent += "|  ";
    //         }
    //         string sColor = root->color ? "RED" : "BLACK";
    //         cout << root->value << "(" << sColor << ")" << endl;
    //         //cout << root->data << endl;
    //         printHelper(root->left, indent, false);
    //         printHelper(root->right, indent, true);
    //     }
    // }

public:
    RedBlackTree()
    {
        TNULL = new Node;
        TNULL->color = 0;
        TNULL->left = nullptr;
        TNULL->right = nullptr;
        for (int i = 0; i < 52; i++)
                root[i] = TNULL;
    }
    // void preorder()
    // {
    //     preOrderHelper(this->root);
    // }
    // // void inorder()
    // // {
    // //     inOrderHelper(this->root);
    // // }
    // void postorder()
    // {
    //     postOrderHelper(this->root);
    // }
    bool get(char k[])
    {
        int index;
        if (k[0] <= 'Z')
            index = k[0] - 'A';
        else
            index = k[0] - 'a' + 26;
        if (searchTreeHelper(this->root[index], k) == TNULL)
            return false;
        else
            return true;
    }
    NodePtr minimum(NodePtr node)
    {
        while (node->left != TNULL)
        {
            node = node->left;
        }
        return node;
    }
    // NodePtr maximum(NodePtr node)
    // {
    //     while (node->right != TNULL)
    //     {
    //         node = node->right;
    //     }
    //     return node;
    // }
    // NodePtr successor(NodePtr x)
    // {
    //     if (x->right != TNULL)
    //     {
    //         return minimum(x->right);
    //     }
    //     NodePtr y = x->parent;
    //     while (y != TNULL && x == y->right)
    //     {
    //         x = y;
    //         y = y->parent;
    //     }
    //     return y;
    // }
    // NodePtr predecessor(NodePtr x)
    // {
    //     if (x->left != TNULL)
    //     {
    //         return maximum(x->left);
    //     }
    //     NodePtr y = x->parent;
    //     while (y != TNULL && x == y->left)
    //     {
    //         x = y;
    //         y = y->parent;
    //     }
    //     return y;
    // }
  
    void leftRotate(NodePtr x, int i)
    {
        NodePtr y = x->right;
        x->right = y->left;
        if (y->left != TNULL)
        {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr)
        {
            this->root[i] = y;
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
        x->parent = y;
    }
    void rightRotate(NodePtr x, int i)
    {
        NodePtr y = x->left;
        x->left = y->right;
        if (y->right != TNULL)
        {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr)
        {
            this->root[i] = y;
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
        x->parent = y;
    }
    bool put(char key[], char val[])
    {
        // if(strcmp(key,"oCxfxpemTGMazERgUtmalkuMMmOTEpGTtGAtVgFqMrqNxJTTdHUQtQDg")==0)
        // 	cout<<val;
        int index;
        if (key[0] <= 'Z')
            index = key[0] - 'A';
        else
            index = key[0] - 'a' + 26;
        lexhash[index]++;
        if (mempiece.root == NULL || mempiece.used == NUMBER)
        {
            mempiece.root = new Node[NUMBER];
            mempiece.used = 0;
        }
        NodePtr node = &mempiece.root[mempiece.used++];
        node->parent = nullptr;
        strcpy(node->data, key);
        //cout<<key<<endl;
        // if (strcmp(node->data, key) != 0)
        // {
        //     cout << "inrbtree"<<endl;
        //     cout << strlen(node->data)<<endl;
        //     cout << node->data << " " << key << endl;
        //     cout << "bughere" << endl;
        // }
        strcpy(node->value, val);
        // if (strcmp(node->value, val) != 0)
        // {
        //     cout << strlen(node->value)<<endl;
        //     cout << node->value << " " << key << endl;
        //     cout << "bughere" << endl;
        // }
        node->left = TNULL;
        node->right = TNULL;
        node->color = 1;
        NodePtr y = nullptr;
        NodePtr x = this->root[index];
        while (x != TNULL)
        {
            y = x;
            int l = strcmp(node->data, x->data);
            if (l == 0)
            {
                //cout<<node->data<<endl;

                strcpy(x->value, val);
                lexhash[index]--;
                // if (strcmp(node->data, key) != 0)
                // {
                //     cout << node->data << " " << key << endl;
                //     cout << "bughere" << endl;
                // }
                return true;
            }
            if (l < 0)
            {
                x = x->left;
            }
            else
            {
                x = x->right;
            }
        }
        node->parent = y;
        if (y == nullptr)
        {
            root[index] = node;
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
            node->color = 0;
            return false;
        }
        if (node->parent->parent == nullptr)
        {
            return false;
        }
        insertFix(node, index);
        return false;
    }
    pair<string, string> get(int n)
    {
        int i = 0;
        int temp = 0;
        for (; i < 52; i++)
        {
            //cout<<lexhash[i]<<endl;
            if (n <= temp + lexhash[i])
                break;
            temp += lexhash[i];
            //cout<<temp<<endl;
        }
        int *k = new int;
        pair<string, string> p;
        cout << "i " << i << endl;
        cout << "n " << n << endl;
        cout << "temp " << temp << endl;
        inOrderHelper(this->root[i], n-temp, k, &(p.first), &(p.second));
        return p;
    }
    // NodePtr getRoot()
    // {
    //     return this->root;
    // }
    bool del(char data[])
    {
        int index;
        if (data[0] <= 'Z')
            index = data[0] - 'A';
        else
            index = data[0] - 'a' + 26;
        //cout << "In delete";
        if (deleteNodeHelper(this->root[index], data, index) == true)
        {
          //  cout << "In here";
            lexhash[index]--;
            return true;
        }
        else
            return false;
    }
    // void printTree()
    // {
    //     if (root)
    //     {
    //         printHelper(this->root, "", true);
    //     }
    // }
};
// int main()
// {
//     RedBlackTree bst;
//     int n;
//     cin >> n;
//     char key[64];
//     char value[256];
//     for (int i = 0; i < n; i++)
//     {
//         cin >> key >> value;
//         bst.put(key, value);
//     }
//     //bst.printTree();
//     // cout << endl
//     //      << "After deleting" << endl;
//     // //bst.deleteNode(13);
//     // bst.printTree();
// }
