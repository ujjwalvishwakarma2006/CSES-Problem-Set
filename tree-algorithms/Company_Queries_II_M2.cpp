/*
This question can be mapped to finding the lowest common ancestor of two nodes in a tree.
Another method that similar to M1 again uses the binary lifting technique. 

In this method, when finding LCA of two given nodes, we first lift the deeper node up
so that it comes to the same level as the shallower node. 
After this, we check if the two nodes are equal.
If yes, then we immediately return that node.
Else, we simultaneously keep jumping up towards root node till we see their ancestors
are different.
*/

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

const int MAX_LOG = 19;     // 2^19 > 2*10^5
int n, q, root, timer;
vector<vector<int>> adj, up;
vector<int> depth;

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

int lca(int u, int v) {
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

void inputAndPreprocess() {
    adj.resize(n + 1);
    up.resize(n + 1, vector<int>(MAX_LOG, 0));
    depth.resize(n + 1, 0);
    
    for (int u = 2; u <= n; ++u) {
        int v;
        cin >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    dfs(root, 0);
}

int main() {

    root = 1;
    cin >> n >> q;

    inputAndPreprocess();

    while (q--) {
        int a, b;
        cin >> a >> b;
        cout << lca(a, b) << endl;
    }

    return 0;
}