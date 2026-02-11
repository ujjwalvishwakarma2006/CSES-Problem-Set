#include <iostream>
#include <vector>
using namespace std;

int N, Q;
const int maxN = 2e5 + 2;
int nums[maxN];

class SegmentTree {
    int n;
    vector<long long> segTree;

    int getMidpoint(int startPoint, int endPoint) {
        return startPoint + (endPoint-startPoint)/2;
    }

    long long buildSegTree(
        const int   arr[],
        const int   segmentStart,
        const int   segmentEnd,
        const int   segmentIndex
    ) {
        // CASE 1: Segment size becomes one
        if (segmentEnd == segmentStart) {
            return segTree[segmentIndex] = arr[segmentEnd];
        }

        // CASE 2: Segment size >= 2
        int mid = getMidpoint(segmentStart, segmentEnd);
        segTree[segmentIndex] = 0;
        segTree[segmentIndex] += buildSegTree(arr, segmentStart, mid, (segmentIndex<<1) + 1);
        segTree[segmentIndex] += buildSegTree(arr, mid+1, segmentEnd, (segmentIndex<<1) + 2);
        return segTree[segmentIndex];
    }

    long long rangeSumQuery(
        const int   segmentStart,
        const int   segmentEnd,
        const int   segmentIndex,
        const int   queryStart,
        const int   queryEnd
    ) {
        // CASE 1: Segment completely lies inside the query range
        if (queryStart <= segmentStart && segmentEnd <= queryEnd) {
            return segTree[segmentIndex];
        }
        
        // CASE 2: Segment completely lies outside the query range
        if (queryEnd < segmentStart || segmentEnd < queryStart) {
            return 0;
        }

        // CASE 3: Segment partially overlaps with the query range
        int mid = getMidpoint(segmentStart, segmentEnd);
        return rangeSumQuery(segmentStart, mid, (segmentIndex<<1) + 1, queryStart, queryEnd)
                + rangeSumQuery(mid+1, segmentEnd, (segmentIndex<<1) + 2, queryStart, queryEnd);

    }

    void pointUpdate(
        const int   segmentStart,
        const int   segmentEnd,
        const int   segmentIndex,
        const int   updateIndex,
        const int   valueDifference
    ) {
        // CASE 1: Segment does not contain the index whose value is being updated
        if (segmentStart > updateIndex || updateIndex > segmentEnd) return;

        // CASE 2: Segment contains the index whose value is being updated
        segTree[segmentIndex] += valueDifference;
        if (segmentEnd == segmentStart) return;

        int mid = getMidpoint(segmentStart, segmentEnd);
        pointUpdate(segmentStart, mid, (segmentIndex<<1) + 1, updateIndex, valueDifference);
        pointUpdate(mid+1, segmentEnd, (segmentIndex<<1) + 2, updateIndex, valueDifference);
    }

public:
    SegmentTree(const int arr[], int n) {
        this->n = n;
        segTree.resize(4*n + 5, 0);
        buildSegTree(arr, 0, n-1, 0);
    }

    long long rangeSum(const int rangeStart, const int rangeEnd) {
        return rangeSumQuery(0, n-1, 0, rangeStart, rangeEnd);
    }

    void updateValue(const int updateIndex, const int valueDifference) {
        pointUpdate(0, n-1, 0, updateIndex, valueDifference);
    }
};

void inputAndPreprocess() {
    cin >> N >> Q;
    for (int i = 0; i < N; ++i) cin >> nums[i];
}

int main () {
    
    inputAndPreprocess();
    SegmentTree tree(nums, N);

    while(Q--) {
        int qType, a, b;
        cin >> qType >> a >> b;
        if (qType == 1) {
            tree.updateValue(a-1, b-nums[a-1]);
            nums[a-1] = b;
        } else if (qType == 2) {
            cout << tree.rangeSum(a-1, b-1) << '\n';
        }
    }
    return 0;
}