#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> dp;
vector<vector<int>> adj;

void dfs(int node, int parent) {
    dp[node][0] = 0;
    dp[node][1] = 0;
    int other = -1;;
    int maxDiff = INT_MIN;

    for (int child : adj[node]) {
        if (child == parent) continue;
        dfs(child, node);
    }

    // Compute dp[node][0]
    for (int child : adj[node]) {
        if (child == parent) continue;
        dp[node][0] += max(dp[child][0], dp[child][1]);
        int diff = dp[child][0] - dp[child][1];
        if (diff > maxDiff) {
            other = child;
            maxDiff = diff;
        }
    }

    // Try matching node with each child
    for (int child : adj[node]) {
        if (child == parent) continue;
        if (child == other) dp[node][1] += 1 + dp[child][0];
        else dp[node][1] += max(dp[child][0], dp[child][1]);
    }
}

int main() {
    int n;
    cin >> n;
    dp.resize(n + 1, vector<int>(2, 0));
    adj.resize(n + 1);
    int u, v;

    for (int i = 1; i <= n - 1; i++) {
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    dfs(1, 0);
    cout << max(dp[1][0], dp[1][1]) << endl;

    return 0;
}
