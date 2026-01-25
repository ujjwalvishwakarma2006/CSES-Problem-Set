/*
This question can be mapped to finding the lowest common ancestor of two nodes in a tree.
There are various methods that can be be utilized to find the LCA. 
This code implements the simplest method, which is fastest to code using binary lifting.

- up[i][j]: stores 2^j'th ancestor of node i
- tin[i]: time when DFS first discovers node i
- tout[i]: time when DFS leaves node i

tin and tout are used to check if one node is ancestor of another in constant time. [Genius]

*/

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

const int MAX_LOG = 19;     // 2^19 > 2*10^5
int n, q, root, timer;
vector<vector<int>> adj, up;
vector<int> tin, tout;

void dfs(int currNode, int parent) {
    tin[currNode] = ++timer;

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

    tout[currNode] = ++timer;
}

// Checks if node u is ancestor of node v in O(1)
bool isAncestor(int u, int v) {
    // return true if discovered u before v and left u after v
    return (tin[u] <= tin[v]) && (tout[u] >= tout[v]);
}

int lca(int u, int v) {
    // if one node is ancestor of another, return the ancestor node immediately
    if (isAncestor(u, v)) return u;
    if (isAncestor(v, u)) return v;
    
    // jump up when up[u][i] is not an ancestor if v
    for (int i = MAX_LOG - 1; i >= 0; --i) {
        if (isAncestor(up[u][i], v)) continue;
        u = up[u][i];
    }

    return up[u][0];
}

void inputAndPreprocess() {
    adj.resize(n + 1);
    up.resize(n + 1, vector<int>(MAX_LOG, 0));
    tin.resize(n + 1);
    tout.resize(n + 1);
    
    for (int u = 2; u <= n; ++u) {
        int v;
        cin >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    dfs(root, 0);

    // This last line is very important
    // Think about what happens when up[u][i] in the lca function reaches 0.
    // isAncestor function will return false always, because tout[0] = tin[0] = 0
    // So, u will keep jumping and reach 0.
    // Therefore, the code, most of the times, will return 0 as LCA.
    // But if we set 0 to be virtual parent of the root node, by exiting it after the root node
    // the function isAncestor(0, v) will return true for all nodes v
    // and will not update u to up[u][i] unnecessarily
    tout[0] = tout[root] + 1;
}

int main() {

    timer = 0;
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