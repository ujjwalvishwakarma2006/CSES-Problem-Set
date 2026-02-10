#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
using namespace std;

int n, q, root = 1;
const int maxN = 2e5 + 5;
int parent[maxN];
long long value[maxN];
vector<int> adjList[maxN];
unordered_map<int, long long> pendingUpdates;

void dfs(int node, int parentNode) {
    parent[node] = parentNode;
    for (int child : adjList[node]) {
        if (child == parentNode) continue;
        dfs(child, node);
        value[node] += value[child];
    }
}

void inputAndPreprocess() {
    cin >> n >> q;
    for (int i = 1; i <= n; ++i) cin >> value[i];
    for (int i = 0; i < n-1; ++i) {
        int u, v;
        cin >> u >> v;
        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }
    dfs(root, 0);
}

void updateValues() {
    if (pendingUpdates.size() == 0) return;
    unordered_set<int> updated;
    for (auto [node, newValue] : pendingUpdates) {
        if (updated.find(node) != updated.end()) continue;
        long long valueDifference = newValue - value[node];
        int parentNode = parent[node];
        value[node] = newValue;
        while (parentNode != 0) {
            // if (updated.find(parentNode) != updated.end()) continue;
            if (pendingUpdates.find(parentNode) != pendingUpdates.end()) {
                valueDifference += pendingUpdates[parentNode] - value[parentNode];
            }
            value[parentNode] += valueDifference;
            parentNode = parent[parentNode];
        }
    }
    pendingUpdates.clear();
    updated.clear();
}

int getSubtreeValue(int s) {
    updateValues();
    return value[s];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    inputAndPreprocess();

    while(q--) {
        int queryType, s, x;
        cin >> queryType;
        if (queryType == 1) {
            cin >> s >> x;
            pendingUpdates[s] = x;
        } else {
            cin >> s;
            cout << getSubtreeValue(s) << '\n';
        }
    }

    return 0;
}