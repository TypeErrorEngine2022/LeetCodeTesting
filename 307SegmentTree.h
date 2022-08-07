using namespace std;

struct SegmentTreeNode
{
    // start and end indicate the range 
    int start;
    int end;
    int sum; //for leaf node, sum is val; for others, sum is from [start, end]
    SegmentTreeNode* left;
    SegmentTreeNode* right;

    SegmentTreeNode(int start, int end, int sum, SegmentTreeNode* left, SegmentTreeNode* right):
        start(start), end(end), sum(sum), left(left), right(right)
    {

    }

    ~SegmentTreeNode()
    {
        delete left;
        delete right;
    }
};


class NumArray {
public:
    vector<int>& ref;
    SegmentTreeNode* root;

    NumArray(vector<int>& nums):
        ref(nums), root(nullptr)
    {
        root = buildTree(0, nums.size() - 1, ref);
    }

    ~NumArray()
    {
        // left and right will be deleted, and thus the whole subtree
        delete root;
    }

    SegmentTreeNode* buildTree(int start, int end, vector<int>& ref)
    {
        if (start == end)
        {
            SegmentTreeNode* res = new SegmentTreeNode(start, end, ref[start], nullptr, nullptr);
            return res;
        }

        int mid = start + (end - start) / 2;
        SegmentTreeNode* left = buildTree(start, mid, ref);
        SegmentTreeNode* right = buildTree(mid + 1, end, ref);

        SegmentTreeNode* res = new SegmentTreeNode(start, end, left->sum + right->sum, left, right);
        return res;
    }
    
    void updateTree(SegmentTreeNode* root, int index, int val)
    {
        // reach leaf node, base case
        if (root->start == root->end && root->start == index)
        {
            root->sum = val;
            return;
        }

        int mid = root->start + (root->end - root->start) / 2;
        if (index <= mid)
        {
            updateTree(root->left, index, val);
        }
        else
        {
            updateTree(root->right, index, val);
        }

        root->sum = root->left->sum + root->right->sum;
        return;
    }

    void update(int index, int val) {
        ref[index] = val;
        updateTree(root, index, val);
    }
    
    int querySum(SegmentTreeNode* root, int left, int right)
    {
        // base case: match the range
        if (root->start == left && root->end == right)
        {
            return root->sum;
        }

        int mid = root->start + (root->end - root->start) / 2;
        // whole range falls on left tree
        if (right <= mid)
        {
            return querySum(root->left, left, right);
        }
        else if (left > mid) //falls on right tree
        {
            return querySum(root->right, left, right);
        }
        else
        {
            return querySum(root->left, left, mid) + querySum(root->right, mid + 1, right);
        }
    }

    int sumRange(int left, int right) {
        return querySum(root, left, right);
    }
};
