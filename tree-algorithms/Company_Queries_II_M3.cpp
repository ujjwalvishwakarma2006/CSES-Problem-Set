/*

In this another method of finding LCA, we make use of some extra concepts:
    1. Euler Tour
    2. Segment Trees for Range Minimum Queries

One can also use the Square-root decomposition instead of segment tree as well. 

    Step 1: Populate the following vectors according to the description via the dfs function:

        -   height[i] stores the height of node i from the root node
        -   euler[i] stores the i'th node in the euler tour of the tree
        -   first[i] stores the first occurance of i'th node in the euler array
        -   segtree stores the segment tree, where left and right children of node i are 2 * i and 2 * i + 1
        -   visited[i] keeps track whether we have visited a node or not. 
    
    Step 2: Recursively build the Segment Tree for efficient query Processing

        
*/

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int n, q, root;
vector<vector<int>> adjList;

struct LCA {
    vector<int> height, euler, first, segtree;
    vector<bool> visited;
    int n;

    LCA(vector<vector<int>>& adjList) {
        n = adjList.size();
        height.resize(n);
        first.resize(n);
        euler.reserve(n * 2);
        visited.assign(n, false);
        dfs(adjList, root);
        int m = euler.size();
        segtree.resize(m * 4);
        buildSegTree(1, 0, m - 1);
    }

    void dfs(vector<vector<int>>& adjList, int node, int h = 0) {
        visited[node] = true;
        height[node] = h;
        first[node] = euler.size();
        euler.push_back(node);
        for(int child : adjList[node]) {
            if (visited[child]) continue;
            dfs(adjList, child, h + 1);
            euler.push_back(node);
        }
    }

    void buildSegTree(int node, int b, int e) {
        if (b == e) segtree[node] = euler[b];
        else {
            int mid = b + (e - b) / 2;
            buildSegTree(node << 1, b, mid);
            buildSegTree(node << 1 | 1, mid + 1, e);
            int l = segtree[node << 1];
            int r = segtree[node << 1 | 1];
            segtree[node] = (height[l] < height[r]) ? l : r;
        }
    }

    int query(int node, int b, int e, int L, int R) {
        if (b > R || e < L) return -1;
        if (b >= L && e <= R) return segtree[node];
        int mid = (b + e) >> 1;
        int left = query(node << 1, b, mid, L, R);
        int right = query(node << 1 | 1, mid + 1, e, L, R);
        if (left == -1) return right; 
        if (right == -1) return left; 
        return height[left] < height[right] ? left : right;
    }

    int lca(int u, int v) {
        int left = first[u];
        int right = first[v];
        if (left > right) swap(left, right);
        return query(1, 0, euler.size() - 1, left, right);
    }
};

int main() {
    
    root = 1;
    cin >> n >> q;
    
    adjList.resize(n + 1);
    
    for (int u = 2; u <= n; ++u) {
        int v;
        cin >> v;
        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }

    LCA queryProcessor(adjList);
    
    while (q--) {
        int a, b;
        cin >> a >> b;
        cout << queryProcessor.lca(a, b) << endl;
    }

    return 0;
}