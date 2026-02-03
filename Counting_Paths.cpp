/*
    This solution uses binary lifting techniques similar to the LCA (Lowest Common Ancestor) problem.

PROBLEM:
    Given a tree and a set of paths, determine for each node how many of the given paths pass through it.

KEY CONCEPTS:
    1. Binary Lifting
    2. Depth First Search
    3. Dynamic Programming

ALGORITHM:
    Step 1: Build Binary Lifting Structure
    ----------------------------------------
    Root the tree at node 1 and use DFS to compute:
    
      • up[i][j]:
        The 2^j-th ancestor of node i
        
      • depth[i]:
        The depth of node i from the root
    
    This allows us to find the LCA (Lowest Common Ancestor) in O(log n) time.
    
    Step 2: Process Each Path
    ---------------------------
    For each path (u, v), find its LCA and mark the path using two arrays:
    
      • increments[i]:
        Count of paths that start at node i or pass through it going upward
        
      • decrements[i]:
        Count of paths that end at node i (to stop counting at LCA)
    
    Action: Increment increments[u] and increments[v] by 1, and increment decrements[LCA(u, v)] by 1.
    
    Step 3: Count Paths via DFS
    ----------------------------
    Use DFS to compute the final path count for each node:
    
      • numPaths[i]:
        The number of paths that pass through node i
    
    Calculation: For each node, start with (increments[i] - decrements[i]) to account for 
    paths starting/ending at this node. Then add the contribution from each child's subtree 
    that wasn't blocked: (numPaths[child] - decrements[child]).
        being carried up (i.e. numPaths[child] - decrements[i])
*/

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

const int MAX_LOG = 19;     // 2^19 > 2*10^5
int n, q, root;
vector<vector<int>> adj, up;
vector<int> depth, numPaths;
vector<int> increments, decrements;

void dfs(int currNode, int parent) {

    depth[currNode] = depth[parent] + 1;
    up[currNode][0] = parent;
    
    // We can be sure that we have already discovered all nodes which are above node currNode,
    // since we are doing DFS. So the following node will not result in bad bahaviour
    for (int i = 1; i < MAX_LOG; ++i) {
        up[currNode][i] = up[up[currNode][i - 1]][i - 1];
        // minor performance gain, stopping as soon as no ancestors present
        if (up[currNode][i] == 0) break;
    }
    
    for (int child : adj[currNode]) {
        if (child == parent) continue;
        dfs(child, currNode);
    }
}

inline int lca(int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);
    
    // Reduce the depthGap between the two nodes
    int depthGap = depth[u] - depth[v];
    for (int i = MAX_LOG - 1; i >= 0; --i) {
        if ((1 << i) & depthGap) u = up[u][i];
    }

    if (u == v) return u;

    for (int i = MAX_LOG - 1; i >= 0; --i) {
        if (up[u][i] == up[v][i]) continue;
        u = up[u][i];
        v = up[v][i];
    }

    return up[u][0];
}

inline void addPath(int u, int v) {
    int pathLCA = lca(u, v);
    increments[u] += 1;
    increments[v] += 1;
    decrements[pathLCA] += 1;
}

inline void inputAndPreprocess() {
    adj.resize(n + 1);
    up.resize(n + 1, vector<int>(MAX_LOG, 0));
    depth.resize(n + 1, 0);
    increments.resize(n + 1, 0);
    decrements.resize(n + 1, 0);
    numPaths.resize(n + 1, 0);
    
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    depth[0] = -1;
    dfs(root, 0);
}

void dfsAndCountPaths(int node, int parent) {
    numPaths[node] = increments[node] - decrements[node];
    for (int child : adj[node]) {
        if (child == parent) continue;
        dfsAndCountPaths(child, node);
        numPaths[node] += (numPaths[child] - decrements[child]);
    }
}

int main() {

    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    root = 1;
    cin >> n >> q;

    inputAndPreprocess();

    while (q--) {
        int a, b;
        cin >> a >> b;
        addPath(a, b);
    }

    dfsAndCountPaths(1, 0);
    for (int i = 1; i <= n; ++i) cout << numPaths[i] << ' ';

    return 0;
}