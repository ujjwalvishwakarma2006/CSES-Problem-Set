/*
    SEGMENT TREE DATA STRUCTURE
    ============================
    This is a versatile implementation of Segment Tree that supports multiple operations (SUM, MIN, MAX).
    Segment Tree offers efficient point updates and range queries with O(log n) time complexity for both.

PROBLEM:
    Given an array of length N, and Q queries of two types:
    1. Type 1 ("1 k u"): Update the value at index k to u
    2. Type 2 ("2 a b"): Perform a range query on elements in range [a, b]
       - For SUM: Calculate sum of all elements
       - For MIN: Find minimum element
       - For MAX: Find maximum element

CONSTRAINTS:
    • 1 ≤ N ≤ 2×10^5 (number of array elements)
    • 1 ≤ Q ≤ 2×10^5 (number of queries)
    • Values can be positive or negative integers

KEY CONCEPTS:
    1. Segment Tree - Binary tree where each node represents a segment (range) of the array
    2. Recursion - Used for building tree, querying, and updating
    3. Identity Elements - Neutral values (0 for SUM, LLONG_MIN for MAX, LLONG_MAX for MIN)

SEGMENT TREE STRUCTURE:
    • Array-based representation where node at index i has:
      - Left child at index: 2*i + 1
      - Right child at index: 2*i + 2
    • Each node stores the result (sum/min/max) for its corresponding segment
    • Leaf nodes represent individual array elements
    • Internal nodes represent combined results of their children

ALGORITHMS:

    STEP 1: Build Segment Tree
    ---------------------------
    Recursively builds the tree in a bottom-up manner:
    
    Base Case: 
      • When segment contains only one element (leaf node), store that element's value
    
    Recursive Case:
      • Split segment into two halves at midpoint
      • Recursively build left subtree for range [start, mid]
      • Recursively build right subtree for range [mid+1, end]
      • Combine results from both children using the operation (sum/min/max)
      • Store combined result in current node
    
    Example for array [3, 1, 4, 2, 5]:
      - Root stores result for [0, 4]
      - Left child stores result for [0, 2]
      - Right child stores result for [3, 4]
    
    Time Complexity: O(n) - visits each array element once
    Space Complexity: O(4n) - segment tree array size

    STEP 2: Range Query
    -------------------
    Finds the result for a query range [queryStart, queryEnd] by traversing the tree:
    
    Three Cases:
      1. Complete Overlap: Current segment lies completely inside query range
         → Return the stored value at this node (no need to go deeper)
      
      2. No Overlap: Current segment lies completely outside query range
         → Return neutral value (0 for SUM, LLONG_MIN for MAX, LLONG_MAX for MIN)
      
      3. Partial Overlap: Current segment partially overlaps with query range
         → Recursively query both left and right children
         → Combine results from both children using the operation
    
    Example: Query range [1, 3] in segment tree for [3, 1, 4, 2, 5]
      - Starts at root [0, 4] - partial overlap, splits
      - Checks left [0, 2] - partial overlap, splits further
      - Checks right [3, 4] - partial overlap, splits further
      - Combines only the relevant segments
    
    Time Complexity: O(log n) - at most visits O(log n) nodes per level
    Space Complexity: O(log n) - recursion stack depth

    STEP 3: Point Update
    --------------------
    Updates a single element and propagates changes up the tree:
    
    Process:
      1. Traverse down to the leaf node containing the update index
      2. Update the leaf node with new value
      3. Backtrack up the tree, recalculating each ancestor node
         by combining values from its left and right children
    
    Three Cases:
      1. Out of Range: Current segment doesn't contain the update index
         → Return immediately, no changes needed
      
      2. Leaf Node: Reached the exact element to update
         → Update the value in segment tree
      
      3. Internal Node: Segment contains the update index but not a leaf
         → Recursively update appropriate child (left or right)
         → Recalculate current node by combining updated children
    
    Example: Update index 2 to value 7 in [3, 1, 4, 2, 5]
      - Traverses to leaf representing index 2
      - Updates leaf node
      - Recalculates parent covering [2, 2]
      - Recalculates ancestor covering [0, 2]
      - Recalculates root covering [0, 4]
    
    Time Complexity: O(log n) - traverses height of tree twice (down and up)
    Space Complexity: O(log n) - recursion stack depth

OPERATION TYPES:
    • SUM: Adds all elements in range (identity: 0)
    • MIN: Finds minimum element in range (identity: LLONG_MAX)
    • MAX: Finds maximum element in range (identity: LLONG_MIN)

USAGE:
    SegmentTree tree(array, size, SegmentTree::SUM);  // or MIN or MAX
    long long result = tree.query(left, right);        // Range query
    tree.updateValue(index, newValue);                 // Point update

*/

#include <iostream>
#include <vector>
#include <climits>
using namespace std;

int N, Q;
const int maxN = 2e5 + 2;
int nums[maxN];

class SegmentTree {
public:
    enum OperationType {
        MAX,
        MIN,
        SUM
    };

private:
    int n;
    OperationType operationType;
    vector<long long> segTree;

    int getMidpoint(int startPoint, int endPoint) const {
        return startPoint + (endPoint - startPoint) / 2;
    }

    // Returns the neutral value for the operation (identity element)
    long long getNeutralValue() const {
        switch (operationType) {
            case MAX: return LLONG_MIN;
            case MIN: return LLONG_MAX;
            case SUM: return 0;
        }
        return 0;
    }

    // Combines two values according to the operation type
    long long combineValues(long long leftValue, long long rightValue) const {
        switch (operationType) {
            case MAX: return max(leftValue, rightValue);
            case MIN: return min(leftValue, rightValue);
            case SUM: return leftValue + rightValue;
        }
        return 0;
    }

    long long buildSegTree(
        const int arr[],
        const int segmentStart,
        const int segmentEnd,
        const int segmentIndex
    ) {
        // CASE 1: Segment size becomes one (leaf node)
        if (segmentEnd == segmentStart) {
            return segTree[segmentIndex] = arr[segmentEnd];
        }

        // CASE 2: Segment size >= 2 (internal node)
        int mid = getMidpoint(segmentStart, segmentEnd);

        long long leftValue  = buildSegTree(arr, segmentStart, mid, (segmentIndex << 1) + 1);
        long long rightValue = buildSegTree(arr, mid+1, segmentEnd, (segmentIndex << 1) + 2);

        return segTree[segmentIndex] = combineValues(leftValue, rightValue);
    }

    long long rangeQuery(
        const int segmentStart,
        const int segmentEnd,
        const int segmentIndex,
        const int queryStart,
        const int queryEnd
    ) const {
        // CASE 1: Segment completely lies inside the query range
        if (queryStart <= segmentStart && segmentEnd <= queryEnd) {
            return segTree[segmentIndex];
        }

        // CASE 2: Segment completely lies outside the query range
        if (queryEnd < segmentStart || segmentEnd < queryStart) {
            return getNeutralValue();
        }

        // CASE 3: Segment partially overlaps with the query range
        int mid = getMidpoint(segmentStart, segmentEnd);
        long long leftValue  = rangeQuery(segmentStart, mid, (segmentIndex << 1) + 1, queryStart, queryEnd);
        long long rightValue = rangeQuery(mid+1, segmentEnd, (segmentIndex << 1) + 2, queryStart, queryEnd);

        return combineValues(leftValue, rightValue);
    }

    void pointUpdate(
        const int segmentStart,
        const int segmentEnd,
        const int segmentIndex,
        const int updateIndex,
        const int newValue
    ) {
        // CASE 1: Segment does not contain the index whose value is being updated
        if (segmentStart > updateIndex || updateIndex > segmentEnd) {
            return;
        }

        // CASE 2: Reached the leaf node containing the update index
        if (segmentEnd == segmentStart) {
            segTree[segmentIndex] = newValue;
            return;
        }

        // CASE 3: Internal node - recursively update children and recalculate
        int mid = getMidpoint(segmentStart, segmentEnd);
        pointUpdate(segmentStart, mid, (segmentIndex << 1) + 1, updateIndex, newValue);
        pointUpdate(mid+1, segmentEnd, (segmentIndex << 1) + 2, updateIndex, newValue);

        // Recalculate current node's value based on updated children
        long long leftValue   = segTree[(segmentIndex << 1) + 1];
        long long rightValue  = segTree[(segmentIndex << 1) + 2];
        segTree[segmentIndex] = combineValues(leftValue, rightValue);
    }

public:
    SegmentTree(const int arr[], int n, OperationType type = SUM) {
        this->n = n;
        this->operationType = type;
        segTree.resize(4 * n + 5, getNeutralValue());
        buildSegTree(arr, 0, n - 1, 0);
    }

    long long query(const int rangeStart, const int rangeEnd) const {
        return rangeQuery(0, n - 1, 0, rangeStart, rangeEnd);
    }

    void updateValue(const int updateIndex, const int newValue) {
        pointUpdate(0, n - 1, 0, updateIndex, newValue);
    }
};

void inputAndPreprocess() {
    cin >> N >> Q;
    for (int i = 0; i < N; ++i) cin >> nums[i];
}

int main() {
    inputAndPreprocess();
    
    // Create segment tree with SUM operation (default)
    SegmentTree tree(nums, N, SegmentTree::MIN);
    
    while (Q--) {
        int qType, a, b;
        cin >> qType >> a >> b;
        
        if (qType == 1) {
            // Update query: set value at position a to b
            tree.updateValue(a - 1, b);
            nums[a - 1] = b;
        } else if (qType == 2) {
            // Range query: get result for range [a, b]
            cout << tree.query(a - 1, b - 1) << '\n';
        }
    }
    
    return 0;
}