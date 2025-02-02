#include <bits/stdc++.h>
using namespace std;

class AVLNode
{
    int data;
    AVLNode *left = nullptr, *right = nullptr;
    int BF = 0; // BlanceFactor  {-1,0,1}
    AVLNode *leftRotation(AVLNode *root);
    AVLNode *rightRotation(AVLNode *root);

public:
    AVLNode(int x) : data(x) {}
    int height(AVLNode *root);
    void checkBF(AVLNode *root);
    void Sequence(AVLNode *root, vector<int> &sequence);
    void checkOrder(AVLNode *root);
    // 以上是用来检测的函数
    AVLNode *find(AVLNode *root, int x); // 递归
    AVLNode *insert(AVLNode *&root, int x);
    void remove(AVLNode *&root, int x);
    void print(AVLNode *root);
};

int AVLNode::height(AVLNode *root)
{
    return root == nullptr ? 0 : max(height(root->left), height(root->right)) + 1;
}

void AVLNode::checkBF(AVLNode *root)
{
    if (root == nullptr)
        return;
    assert(root->BF == -1 || root->BF == 0 || root->BF == 1);
    // cout << height(root->right) << " " << height(root->left) << " " << BF << endl;
    assert(height(root->right) - height(root->left) == root->BF);
    checkBF(root->left);
    checkBF(root->right);
    return;
}

void AVLNode::Sequence(AVLNode *root, vector<int> &sequence)
{
    if (root == nullptr)
        return;
    else
    {
        Sequence(root->left, sequence);
        sequence.push_back(root->data);
        Sequence(root->right, sequence);
        return;
    }
}

void AVLNode::checkOrder(AVLNode *root)
{
    if (root == nullptr)
        return;
    assert(root->left == nullptr ? 1 : root->data > root->left->data);
    assert(root->right == nullptr ? 1 : root->data < root->right->data);
    checkOrder(root->left);
    checkOrder(root->right);
    return;
}

AVLNode *AVLNode::find(AVLNode *root, int x)
{
    if (root == nullptr || root->data == x)
        return root;
    else if (x < root->data)
        return find(root->left, x);
    else
        return find(root->right, x);
}

AVLNode *AVLNode::leftRotation(AVLNode *root)
{
    AVLNode *newhead = root->right;
    root->right = newhead->left;
    newhead->left = root;
    newhead->left->BF = newhead->left->BF - 1 - (newhead->BF > 0 ? newhead->BF : 0);
    newhead->BF = newhead->BF - 1 + (newhead->left->BF < 0 ? newhead->left->BF : 0);
    return newhead;
}
AVLNode *AVLNode::rightRotation(AVLNode *root)
{
    AVLNode *newhead = root->left;
    root->left = newhead->right;
    newhead->right = root;
    newhead->right->BF = newhead->right->BF + 1 - (newhead->BF < 0 ? newhead->BF : 0);
    newhead->BF = newhead->BF + 1 + (newhead->right->BF > 0 ? newhead->right->BF : 0);
    return newhead;
}

AVLNode *AVLNode::insert(AVLNode *&root, int x)
{
    if (root == nullptr)
    {
        root = new AVLNode(x);
        return root;
    }
    AVLNode *cur = root, **parent = &root;
    AVLNode *destNode = nullptr, **parentOfDest = &root;
    while (1) // 找到destNode，并检查x是否已在树中
    {
        if (cur->BF != 0)
        {
            destNode = cur;
            parentOfDest = parent;
        }
        if (x < cur->data)
        {
            if (cur->left == nullptr)
            {
                cur->left = new AVLNode(x);
                break;
            }
            else
            {
                parent = &cur->left;
                cur = cur->left;
            }
        }
        else if (x > cur->data)
        {
            if (cur->right == nullptr)
            {
                cur->right = new AVLNode(x);
                break;
            }
            else
            {
                parent = &cur->right;
                cur = cur->right;
            }
        }
        else
            return cur;
    }

    if (destNode == nullptr)
    {
        AVLNode *temp = root;
        while (temp->data != x)
        {
            temp->BF += (x < temp->data) ? -1 : 1;
            if (x < temp->data)
                temp = temp->left;
            else
                temp = temp->right;
        }
        return temp;
    }
    else
    {
        AVLNode *temp = destNode;

        while (temp->data != x)
        {
            temp->BF += (x < temp->data) ? -1 : 1;
            if (x < temp->data)
                temp = temp->left;
            else
                temp = temp->right;
        }
        if (destNode->BF == 2) // 进行相应的旋转 
        {
            assert(destNode->right->BF == 1 || destNode->right->BF == -1);
            if (destNode->right->BF == 1)
                *parentOfDest = leftRotation(*parentOfDest);
            else if (destNode->right->BF == -1)
            {
                destNode->right = rightRotation(destNode->right);
                *parentOfDest = leftRotation(*parentOfDest);
            }
        }
        else if (destNode->BF == -2) // 进行相应的旋转 
        {
            assert(destNode->left->BF == 1 || destNode->left->BF == -1);
            if (destNode->left->BF == -1)
                *parentOfDest = rightRotation(*parentOfDest);
            else if (destNode->left->BF == 1)
            {
                destNode->left = leftRotation(destNode->left);
                *parentOfDest = rightRotation(*parentOfDest);
            }
        }
        return temp;
    }
}
void AVLNode::remove(AVLNode *&root, int x)
{
    AVLNode *cur = root, **parent = &root;
    AVLNode *NonShorter = nullptr, **parentOfNS = &root, *destNode, *delNode;

    // 找到各节点
    while (cur != nullptr && cur->data != x)
    {
        if (cur->data > x)
        {
            if (cur->BF != -1 && (cur->BF != 1 || cur->right->BF == 0))
            {
                NonShorter = cur;
                parentOfNS = parent;
            }
            parent = &cur->left;
            cur = cur->left;
        }
        else // cur->data < x
        {
            if (cur->BF != 1 && (cur->BF != -1 || cur->left->BF == 0))
            {
                NonShorter = cur;
                parentOfNS = parent;
            }
            parent = &cur->right;
            cur = cur->right;
        }
    }
    if (cur == nullptr)
        return;
    else
        destNode = cur;
    int delData;
    if (destNode->right == nullptr || destNode->left == nullptr) // 已经为叶子节点或叶子节点的父亲，不需要再找中序前驱
    {
        delNode = destNode;
        delData = delNode->data;
    }
    else // 找到中序前驱
    {
        if (cur->BF != 1 && (cur->BF != -1 || cur->left->BF == 0)) // 检查是否要更新NonShorter
        {
            NonShorter = cur;
            parentOfNS = parent;
        }
        parent = &cur->left;
        cur = cur->left;
        while (cur->right != nullptr)
        {
            if (cur->BF != 1 && (cur->BF != -1 || cur->left->BF == 0)) // 检查是否要更新NonShorter
            {
                NonShorter = cur;
                parentOfNS = parent;
            }
            parent = &cur->right;
            cur = cur->right;
        }
        delNode = cur;
        delData = delNode->data;
    }

    // 调整
    AVLNode *temp = (NonShorter == nullptr) ? root : NonShorter;
    AVLNode **parentOfTemp = parentOfNS;
    while (1)
    {
        if (temp->data > delData)
        {
            temp->BF++;
            if (temp->BF == 2)
            {
                if (temp->right->BF == 1 || temp->right->BF == 0)
                    *parentOfTemp = leftRotation(*parentOfTemp);
                else if (temp->right->BF == -1)
                {
                    temp->right = rightRotation(temp->right);
                    *parentOfTemp = leftRotation(*parentOfTemp);
                }
            }
            parentOfTemp = &temp->left;
            temp = temp->left;
            continue;
        }
        else if (temp->data < delData)
        {
            temp->BF--;
            if (temp->BF == -2)
            {
                if (temp->left->BF == -1 || temp->left->BF == 0)
                    *parentOfTemp = rightRotation(*parentOfTemp);
                else if (temp->left->BF == 1)
                {
                    temp->left = leftRotation(temp->left);
                    *parentOfTemp = rightRotation(*parentOfTemp);
                }
            }
            parentOfTemp = &temp->right;
            temp = temp->right;
            continue;
        }
        else if (temp->data == delData)
        {
            destNode->data = delData;
            *parentOfTemp = (temp->right == nullptr) ? temp->left : temp->right;
            delete temp;
            temp = nullptr;
            return;
        }
    }
}

void AVLNode::print(AVLNode *root)
{
    if (root == nullptr)
        return;
    cout << '(';
    print(root->left);
    cout << root->data << "," << root->BF;
    print(root->right);
    cout << ")";
    return;
}

class AVLtree
{
    AVLNode *head = nullptr;

public:
    AVLNode *insert(int x);
    void remove(int x);
    void print();
    void check();
};

AVLNode *AVLtree::insert(int x)
{
    return head->insert(head, x);
}
void AVLtree::remove(int x)
{
    head->remove(head, x);
}
void AVLtree::print()
{
    head->print(head);
}
void AVLtree::check()
{
    head->checkBF(head);
    head->checkOrder(head);
    return;
}

int main()
{
    AVLtree myTree;
    int n;
    cin >> n;
    while (n--)
    {
        int op, x;
        cin >> op >> x;
        switch (op)
        {
        case 1:
            myTree.insert(x);
            break;
        case 2:
            myTree.remove(x);
            break;
        default:
            cout << "Please Enter correct operation" << endl;
        }
        myTree.print();
        myTree.check();
        cout << endl;
    }
    return 0;
}
