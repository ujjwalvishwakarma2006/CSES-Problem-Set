/*
    This code is nearly identical to Company Queries II (Method 2), with only a few additional lines.

PROBLEM:
    Find the distance between any two arbitrary nodes in a tree.

KEY CONCEPTS USED:
    1. Binary Lifting
    2. Bit Shifting
    3. Depth First Search (DFS)

ALGORITHM:

    STEP 1: DFS - Build Data Structures
    ------------------------------------
    Populate the following using DFS traversal:

      • depth[i]:
        The depth (distance from root) of node i

      • up[i][j]:
        The 2^j-th ancestor of node i

    STEP 2: Distance Calculation
    ----------------------------
    1. Calculate the initial depth difference between the two nodes.
    2. Bring the deeper node to the same level as the shallower node.
    3. If both nodes are now the same, return the depth difference.
    4. Otherwise, simultaneously lift both nodes upward. When their ancestors differ at a given level,
       move both nodes to that ancestor and accumulate the distance.
    5. Return the total distance plus 2 (the final 2^0 = 1 node jump for each node).

*/

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int MAX_LOG = 19;     // 2^19 > 2*10^5
int n, q, root;
vector<vector<int>> adj, up;
vector<int> depth;

inline void dfs(int currNode, int parent) {

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

inline int distance(int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);
    
    // Reduce the depthGap between the two nodes
    int depthGap = depth[u] - depth[v];
    for (int i = MAX_LOG - 1; i >= 0; --i) {
        if ((1 << i) & depthGap) u = up[u][i];
    }

    if (u == v) return depthGap;
    
    int totalDistance = depthGap;
    
    for (int i = MAX_LOG - 1; i >= 0; --i) {
        if (up[u][i] == up[v][i]) continue;
        u = up[u][i];
        v = up[v][i];
        totalDistance += (1 << i) << 1;
    }

    return totalDistance + 2;
}

inline void inputAndPreprocess() {
    adj.resize(n + 1);
    up.resize(n + 1, vector<int>(MAX_LOG, 0));
    depth.resize(n + 1, 0);
    
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    depth[0] = -1;
    dfs(root, 0);
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
        cout << distance(a, b) << '\n';
    }

    return 0;
}