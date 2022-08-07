#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct ValSet
{
    int newVal;
    int oldVal;
};


template <typename Item>
class IndexPriorityQueue
{
public:
    IndexPriorityQueue(int maxN);
                                         
    int size() const; 
    bool empty() const;

    bool contains(int key) const;

    //k is index of nums, item is val diff
    void insert(int k, Item item);
    
    void update(int k, Item item);
    
    Item visit(int k);
    
    int diffSum(int left, int right);

    void deleteNode(int k);

    int delMin();

    Item min() const;

    void print() const;
    
private:
    //priorities on key
    std::vector<Item> m_data; //m_data[1] gives the item for key 1
    std::vector<int> m_im; //inverse map, the binary heap, m_im[1] gives the key for node 1
    std::vector<int> m_pm; //position map, m_pm[1] gives the node number for key 1
    
    int m_size;

    /**
     * @brief Compare the priorities two items in binary heap
     * 
     * @param i the index for node
     * @param j the index for node
     * @return true if the first is smaller than or equal to the second
     */
    bool less(int i, int j) const;

    /**
     * @brief swap two nodes, update both m_im and m_pm
     * 
     * @param i 
     * @param j 
     */
    void swap(int i, int j);

    /**
     * @brief swap m_im[ k ] with its parent until the heap order is restored
     * 
     * @param k 
     */
    void swim(int k);

    /**
     * @brief swap m_im[ k ] with its children until the heap order is restored
     * 
     * @param k 
     */
    void sink(int k);
};


template <typename Item>
IndexPriorityQueue<Item>::IndexPriorityQueue(int maxN):
    m_size(0)
{
    m_data.resize(maxN);
    m_im.resize(maxN);
    m_pm.resize(maxN);

    std::fill(m_pm.begin(), m_pm.end(), -1);
}

template <typename Item>
int IndexPriorityQueue<Item>::size() const
{
    return m_size;
}

template <typename Item>
bool IndexPriorityQueue<Item>::empty() const
{
    return m_size == 0;
}

template <typename Item>
bool IndexPriorityQueue<Item>::contains(int k) const
{
    return m_pm[k] != -1;
}

template <typename Item>
void IndexPriorityQueue<Item>::insert(int k, Item item)
{
    m_im[m_size] = k;
    m_pm[k] = m_size;
    m_data[k] = item;
    swim(m_size++);
}

template <typename Item>
void IndexPriorityQueue<Item>::update(int k, Item item)
{
    m_data[k] = item;
    //this tree compare on key, and key will not change in this question
}

template <typename Item>
Item IndexPriorityQueue<Item>::visit(int k)
{
    return m_data[k];
}

// use bfs, O(n), exceed Time limit
template <typename Item>
int IndexPriorityQueue<Item>::diffSum(int left, int right)
{  
    if (m_size == 0) return 0;

    int diffSum = 0;
    queue<int> Q;
    Q.push(0);
    while (!Q.empty()){
        int index = Q.front();
        int key = m_im[index]; //visit the node
        if (key >= left && key <= right)
        {
            diffSum += m_data[key].newVal - m_data[key].oldVal;
        }
        if (2 * index + 1 < m_size) Q.push(2 * index + 1);
        if (2 * index + 2 < m_size) Q.push(2 * index + 2);
        Q.pop();
    }
    
    return diffSum;
}

template <typename Item>
void IndexPriorityQueue<Item>::deleteNode(int k)
{
    //exchange with the last item, since last item has no children
    //less link to break
    swap(k, m_size - 1);
    m_size--;
    swap(k, 0);
    sink(0);
    
    //mark the key as invalid
    m_pm[m_im[m_size + 1]] = -1;
}

template <typename Item>
int IndexPriorityQueue<Item>::delMin()
{
    int min = m_im[0];
    
    deleteNode(0);

    return min;
}

template <typename Item>
Item IndexPriorityQueue<Item>::min() const
{
    return m_data[m_im[0]];
}

template <typename Item>
void IndexPriorityQueue<Item>::print() const
{
    for (int i = 0; i < m_size; i++)
    {
        std::cout << "(" << m_im[i] << ", " << m_data[m_im[i]] << ")" << " ";
    }
    std::cout << "\n";
}


//check whether key of i has lower priorities than j's
template <typename Item>
bool IndexPriorityQueue<Item>::less(int i, int j) const
{
    int key1 = m_im[i];
    int key2 = m_im[j];

    // updating elements with smaller index has larger influence,
    // so smaller key has higher priorities
    return key1 >= key2;
}

template <typename Item>
void IndexPriorityQueue<Item>::swap(int i, int j)
{
    int key1 = m_im[i];
    int key2 = m_im[j];

    std::swap(m_im[i], m_im[j]);
    std::swap(m_pm[key1], m_pm[key2]);
}

template <typename Item>
void IndexPriorityQueue<Item>::swim(int k)
{
    while (k > 0 && less(k / 2, k))
    {
        swap(k / 2, k);
        k = k / 2;
    }
}

template <typename Item>
void IndexPriorityQueue<Item>::sink(int k)
{
    while (2 * k + 1 < m_size)
    {
        int j = 2 * k + 1;

        //choose the larger children to compare with the parent
        if (j + 1 < m_size && less(j, j + 1))
        {
            ++j;
        }
        //If it is larger than the largest children, do not sink
        if (!less(k, j))
        {
            break;
        }
        swap(k, j);
        k = j;
    }
}

class NumArray {
public:
    vector<int> prefixSum;
    vector<int>& ref;
    IndexPriorityQueue<ValSet> tree;
    NumArray(vector<int>& nums):
        ref(nums), tree(nums.size() + 1)
    {
        int sum = 0;
        for (auto& el: nums){
            sum += el;
            prefixSum.push_back(sum);
        }
    }
    
    void update(int index, int val) {
        if (!tree.contains(index))
        {
            tree.insert(index, ValSet{val, ref[index]});
        }
        else
        {
            int oldVal = tree.visit(index).oldVal;
            tree.update(index, ValSet{val, oldVal});
        }
        ref[index] = val;
        
        /*int oldVal = ref[index];
        ref[index] = val;
        for (int i = index; i < prefixSum.size(); i++){
            prefixSum[i] += val - oldVal;
        }*/
    }
    
    int sumRange(int left, int right) {
        int diffSum = tree.diffSum(left, right);
        
        if (left - 1 >= 0){
            return prefixSum[right] - prefixSum[left - 1] + diffSum;
        }
        
        return prefixSum[right] + diffSum;
    }
};