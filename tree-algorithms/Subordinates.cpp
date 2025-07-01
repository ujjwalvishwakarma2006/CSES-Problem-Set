#include <bits/stdc++.h>
using namespace std;

vector<int> subordinates;
vector<vector<int>> adj;

void dfs(int node) {
    if (subordinates[node] >= 0) return;
    if (adj[node].size() == 0) {
        subordinates[node] = 0;
        return;
    }
    for (int child : adj[node]) dfs(child);
    subordinates[node] = adj[node].size();
    for (int child : adj[node]) subordinates[node] += subordinates[child];
}

int main() {
    int n;
    cin >> n;
    subordinates.resize(n + 1, -1);
    adj.resize(n + 1);
    int temp;

    for (int i = 2; i <= n; i++) {
        cin >> temp;
        adj[temp].push_back(i);
    }

    dfs(1);

    for (int i = 1; i <= n; ++i) cout << subordinates[i] << ' ';

    return 0;
}