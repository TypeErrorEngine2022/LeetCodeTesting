#include <iostream>
#include <vector>

#include "307SegmentTree.h" //using namespace std


int main()
{
    using namespace std;

    vector<int> vec{1, 1, 1, 1, 1, 1};

    NumArray* obj = new NumArray(vec);
    obj->update(1, -3); // 1, -3, 1, 1, 1, 1
    cout << obj->sumRange(0, 5) << " ";
    cout << obj->sumRange(2, 5) << " ";
    obj->update(5, 7); // 1, -3, 1, 1, 1, 7
    //obj->update(1,5);
    cout << obj->sumRange(4, 5) << " ";
    obj->update(1, 7); // 1, 7, 1, 1, 1, 7
    cout << obj->sumRange(0, 1);
    delete obj;

    return 0;
}


/**
 * Your NumArray object will be instantiated and called as such:
 * NumArray* obj = new NumArray(nums);
 * obj->update(index,val);
 * int param_2 = obj->sumRange(left,right);
 */