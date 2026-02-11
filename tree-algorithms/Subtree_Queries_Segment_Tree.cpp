/*
    This solution uses a Segment Tree to efficiently answer range sum queries on tree subtrees.
    In an alternative approach we can use a Fenwick Tree (Binary Indexed Tree) for similar O(log n) complexity.
    Some users may find it a bit easier to implement BIT than segment tree.

PROBLEM:
    Given a rooted tree (root = node 1) where each node has a value, answer q queries of two types:
    1. Type 1: Update the value of node s to x
    2. Type 2: Calculate and print the sum of all values in the subtree rooted at node s

CONSTRAINTS:
    • 1 ≤ n ≤ 2×10^5 (number of nodes)
    • 1 ≤ q ≤ 2×10^5 (number of queries)

KEY CONCEPTS:
    1. Euler Tour Technique - Flatten tree into array
    2. Segment Tree - Efficient range sum queries and point updates
    3. DFS - Tree traversal to compute entry/exit times

ALGORITHM:
    
    Step 1: Euler Tour (DFS Flattening)
    ------------------------------------
    Perform DFS to convert the tree structure into a linear array where each subtree 
    corresponds to a contiguous range. This allows us to use array-based data structures.

    For each node i, we track:
      • entryTime[i]: 
        The timestamp when DFS first visits node i (entering its subtree)
      
      • exitTime[i]: 
        The timestamp when DFS finishes processing node i's subtree (exiting)

      • eulerTourValues[t]: 
        The value of the node that was visited at timestamp t during DFS.
        This creates an "Euler Tour of Values" - a flattened representation of the tree.

    Step 2: Segment Tree Construction
    ----------------------------------
    Build a segment tree over the eulerTourValues array to support:
      
      • rangeSum(queryStart, queryEnd): 
        Returns sum of values in range [queryStart, queryEnd] in O(log n)
        Handles three cases:
        - Segment fully inside query range → return stored sum
        - Segment fully outside query range → return 0
        - Partial overlap → recursively query left and right children
      
      • pointUpdate(updateIndex, valueDiff): 
        Updates value at index and propagates changes up the tree in O(log n)
        Only updates nodes whose range contains the updateIndex

    Step 3: Query Processing
    ------------------------
      • Type 1 (Update): Convert node to its entry time, then perform point update
      • Type 2 (Sum): Query range [entryTime[node], exitTime[node]] for subtree sum

TIME COMPLEXITY:
    • Preprocessing: O(n) for DFS + O(n) for segment tree construction
    • Each query: O(log n)
    • Total: O(n + q log n)

SPACE COMPLEXITY: O(n) for arrays and segment tree
*/

#include <iostream>
#include <vector>
using namespace std;

const int maxN = 2e5 + 5;
int n, q, root = 1;
long long value[maxN];
vector<int> adjList[maxN];

int currentTime = 0, entryTime[maxN], exitTime[maxN];
vector<int> eulerTourValues;

void dfs(const int node, const int parentNode) {
    entryTime[node] = currentTime++;
    for (auto child : adjList[node]) {
        if (child == parentNode) continue;
        dfs(child, node);
    }
    // Notice that, unlike last time, we aren't adding the node to the euler tour
    // each time after we finish one of its children.
    exitTime[node] = currentTime - 1;
}

class SegmentTree {
    int n;
    vector<long long> segTree;
    vector<int> arr;

    int getMid(const int l, const int r) const {
        return l + (r-l)/2;
    }

    long long buildSegTree(
        const vector<int>&  arr, 
        const int           segmentStart, 
        const int           segmentEnd, 
        const int           segmentIndex
    ) {
        // CASE 1: Only one element left in the segment
        if (segmentStart == segmentEnd) 
            return segTree[segmentIndex] = arr[segmentStart];
        
        // CASE 2: More than one element in the segment
        int mid = getMid(segmentStart, segmentEnd);
        segTree[segmentIndex] = 
            buildSegTree(arr, segmentStart, mid, (segmentIndex*2) + 1) +
            buildSegTree(arr, mid + 1, segmentEnd, (segmentIndex*2) + 2);

        return segTree[segmentIndex];
    }

    long long rangeSum(
        const int   segmentStart,
        const int   segmentEnd,
        const int   segmentIndex,
        const int   queryStart,
        const int   queryEnd
    ) const {
        // CASE 1: Segment completely lies inside the query range
        if (queryStart <= segmentStart && segmentEnd <= queryEnd)
        return segTree[segmentIndex];
        
        // CASE 2: Segment completely lies outside the query range
        if (segmentEnd < queryStart || queryEnd < segmentStart)
        return 0;
        
        // CASE 3: A part of the Segment overlaps with the query range
        int mid = getMid(segmentStart, segmentEnd);
        return rangeSum(segmentStart, mid, (segmentIndex*2) + 1, queryStart, queryEnd) +
            rangeSum(mid + 1, segmentEnd, (segmentIndex*2) + 2, queryStart, queryEnd);
    }

    void pointUpdate(
        const int   segmentStart,
        const int   segmentEnd,
        const int   segmentIndex,
        const int   updateIndex,
        const int   valueDiff
    ) {         
        // CASE 1: updateIndex lies outside the Segment Range   
        if (updateIndex < segmentStart || segmentEnd < updateIndex) return;
        
        // CASE 2: updateIndex lies inside the Segment Range   
        segTree[segmentIndex] += valueDiff;
        if (segmentStart == segmentEnd) return;
        
        int mid = getMid(segmentStart, segmentEnd);
        pointUpdate(segmentStart, mid, (segmentIndex*2) + 1, updateIndex, valueDiff);
        pointUpdate(mid+1, segmentEnd, (segmentIndex*2) + 2, updateIndex, valueDiff);
    }

public:
    SegmentTree(const vector<int>& arr) {
        this->n = arr.size();
        this->arr = arr;
        segTree.resize(4*n);    // Think about why 4*n.
        buildSegTree(arr, 0, n-1, 0);
    }

    long long sumQuery(const int queryStart, const int queryEnd) const {
        if (queryStart < 0 || queryEnd > n-1 || queryEnd < queryStart){
            cout << "Invalid Query\n";
            return -1;
        }
        return rangeSum(0, n-1, 0, queryStart, queryEnd);
    }

    void updateValue(int index, int newValue) {
        pointUpdate(0, n-1, 0, index, newValue - arr[index]);
        arr[index] = newValue;
    }
};

void inputAndPreprocess() {
    cin >> n >> q;
    for (int i = 1; i <= n; ++i) cin >> value[i];
    for (int i = 0; i < n-1; ++i) {
        int u, v;
        cin >> u >> v;
        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }
    dfs(root, 0);
    eulerTourValues.resize(currentTime);

    for (int node = 1; node <= n; ++node) {
        eulerTourValues[entryTime[node]] = value[node];
    }
}

void updateNodeValue(const int node, const int newValue, SegmentTree& tree) {
    int time = entryTime[node];
    tree.updateValue(time, newValue);
}

long long subtreeSum(const int node, const SegmentTree& tree) {
    int segmentStart = entryTime[node];
    int segmentEnd = exitTime[node];
    return tree.sumQuery(segmentStart, segmentEnd);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    inputAndPreprocess();
    SegmentTree tree(eulerTourValues);

    while(q--) {
        int queryType, s, x;
        cin >> queryType;
        if (queryType == 1) {
            cin >> s >> x;
            updateNodeValue(s, x, tree);
        } else {
            cin >> s;
            cout << subtreeSum(s, tree) << '\n';
        }
    }
    return 0;
}