#include <bits/stdc++.h>
using namespace std;

vector<int> dp;
vector<vector<int>> adj;
int diameter = 0;

void dfs(int node, int parent) {
    int max1 = -1, max2 = -1; // max two heights from children

    for (int child : adj[node]) {
        if (child == parent) continue;
        dfs(child, node);

        if (dp[child] >= max1) {
            max2 = max1;
            max1 = dp[child];
        } else if (dp[child] > max2) {
            max2 = dp[child];
        }
    }

    dp[node] = max1 + 1; // height of current node

    // If we have at least two children, check diameter through this node
    if (max2 != -1) {
        diameter = max(diameter, max1 + max2 + 2);
    }

    // Also consider the case when this node is part of the longest single path
    diameter = max(diameter, dp[node]);
}

int main() {
    int n;
    cin >> n;
    dp.resize(n + 1, 0);
    adj.resize(n + 1);
    int u, v;

    for (int i = 1; i <= n - 1; i++) {
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    dfs(1, 0);
    cout << diameter << endl;
    return 0;
}
