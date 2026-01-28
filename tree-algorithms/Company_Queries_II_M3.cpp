/*
================================================================================
        METHOD 3: LCA USING EULER TOUR AND SEGMENT TREE (RANGE MIN QUERY)
================================================================================

PROBLEM:
    Find the Lowest Common Ancestor (LCA) of two nodes in a tree.

KEY CONCEPTS USED:
    1. Euler Tour (Eulerian Path):
       - A traversal of the tree that visits each node and records it when entering
         and exiting during DFS, creating a sequence of nodes
    2. Segment Tree with Range Minimum Query (RMQ):
       - Efficiently finds the node with minimum height in a range
    3. Height Information:
       - Each node's distance from the root, used to identify the shallowest node

ALTERNATIVE APPROACH:
    Square-root decomposition can also be used instead of segment trees for RMQ.

ALGORITHM BREAKDOWN:
    
    STEP 1: DFS Traversal - Build Data Structures
    ─────────────────────────────────────────────────
    Populate the following vectors during DFS traversal:
        
        • height[i]:
          Stores the depth/height of node i measured from the root node
        
        • euler[]:
          Stores the Euler tour sequence - list of nodes visited during DFS
          Nodes appear multiple times (when entering a subtree and leaving it)
        
        • first[i]:
          Stores the index of the first occurrence of node i in the euler[] array
        
        • visited[i]:
          Boolean array to track which nodes have been visited during DFS
        
        • segtree[]:
          Stores the segment tree structure
          For node at index i: left child is at 2*i, right child is at 2*i+1

    STEP 2: Build Segment Tree (Bottom-Up Construction)
    ──────────────────────────────────────────────────────
    Process the euler[] array recursively to create the segment tree:
        
        • Base Case:
          If only one element remains (b == e), store that node in segtree[node]
        
        • Recursive Case:
          Divide the range into two halves: [b, mid] and [mid+1, e]
          Build left subtree at index (node << 1)
          Build right subtree at index (node << 1 | 1)
          Store the node with smaller height at current index:
          Compare heights of left and right child results
        
        • Time Complexity: O(n)

    STEP 3: Query Segment Tree for LCA
    ────────────────────────────────────
    Find the node with minimum height in the range [first[u], first[v]]:
        
        • Query Base Cases:
          - No overlap: return -1 if query range doesn't intersect current range
          - Complete overlap: return current node if entirely within query range
        
        • Query Recursive Cases:
          Recursively query left and right subtrees
          Combine results by selecting the node with smaller height
        
        • Time Complexity: O(log n)

    STEP 4: Process Queries
    ────────────────────────
    For each query (u, v):
        Get first[u] and first[v] from the first occurrence array
        Ensure left ≤ right by swapping if needed
        Query segment tree for minimum height node in range [left, right]
        That node is the LCA of u and v
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

    // Build the segment tree recursively
    // Parameters:
    //   node:  Current node in the segment tree
    //   b:     Begin index - start of the range in euler[] array
    //   e:     End index - end of the range in euler[] array (inclusive)
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

    // Query the segment tree for range minimum query
    // Parameters:
    //   node:  Current node in the segment tree
    //   b:     Begin index - start of current segment's range in euler[] array
    //   e:     End index - end of current segment's range in euler[] array (inclusive)
    //   L, R:  Query range - we want min height node in euler[L..R]
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