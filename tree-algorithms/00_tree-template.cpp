#include <bits/stdc++.h>
using namespace std;

class Tree {
    vector<vector<int>> adj;
    int numNodes; // Number of nodes in the tree

public:
    Tree(int n) {
        this->numNodes = n;
        adj.resize(n + 1);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void inputEdge() {
        int u, v;
        cin >> u >> v;
        addEdge(u, v);
    }

    void inputTree() {
        for (int i = 1; i < numNodes; ++i) {
            inputEdge();
        }
    }

};

int main() {
    int n;
    cin >> n;
    Tree tree(n);
    tree.inputTree();


    return 0;
}