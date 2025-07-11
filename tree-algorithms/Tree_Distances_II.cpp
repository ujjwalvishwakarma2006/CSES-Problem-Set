/*

🔍 Understanding the Tree Distance Rerooting Trick:

    Suppose we have a tree with 'n' nodes. We arbitrarily choose a root node,
    say node 1, and perform a depth-first traversal to compute:

    1. The size of each subtree (number of nodes rooted at a node).
    2. The total sum of distances from the root to every other node.

    Now, assume we know the total distance D(u) for some node u.
    Let v be a child of u in the rooted tree.

    We can compute D(v) using the formula:
        D(v) = D(u) + (n - 2 * subtreeSize[v])

    🧠 Why does this work?
    When we move the root from u to v:
        - All nodes in the subtree rooted at v become **1 unit closer** to the root.
        - All other nodes become **1 unit farther** from the new root.
    So, we adjust the distance sum accordingly:
        total += (-subtreeSize[v] * 1) + ((n - subtreeSize[v]) * 1)
               = n - 2 * subtreeSize[v]

*/

#include <bits/stdc++.h>
using namespace std;

class Tree {
    int numNodes; // Number of nodes in the tree
    int root = 1;
    vector<vector<int>> adj;
    vector<long long> treeDistances; // Stores sum of distances to all other other nodes
    vector<long long> subtreeSize; // Stores size of each subtree

public:
    Tree(int n) {
        this->numNodes = n;
        adj.resize(n + 1);
        treeDistances.resize(n + 1, 0);
        subtreeSize.resize(n + 1, 0);
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

    void populateSubtreeSize(int node, int parent) {
        subtreeSize[node] = 1; // Count the node itself
        for (int child : adj[node]) {
            if (child == parent) continue; // Skip the parent node
            populateSubtreeSize(child, node);
            subtreeSize[node] += subtreeSize[child];
        }
    }

    void sumDistancesFromRoot(int node, int parent, int depth) {
        treeDistances[root] += depth;
        for (int child : adj[node]) {
            if (child == parent) continue;
            sumDistancesFromRoot(child, node, depth + 1);
        }
    }

    void findTreeDistances(int node, int parent) {
        for (int child : adj[node]) {
            if (child == parent) continue;
            treeDistances[child] = treeDistances[node] + (numNodes - 2 * subtreeSize[child]);
            findTreeDistances(child, node);
        }
    }

    void findTreeDistances() {
        for (int child : adj[root]) {
            treeDistances[child] = treeDistances[root] + (numNodes - 2 * subtreeSize[child]);
            findTreeDistances(child, root);
        }
    }

    void printTreeDistances() {
        for (int i = 1; i <= numNodes; ++i) {
            cout << treeDistances[i] << " ";
        }
    }

    void printSubtreeSizes() {
        for (int i = 1; i <= numNodes; ++i) {
            cout << subtreeSize[i] << " ";
        }
        cout << endl;
    }

};

int main() {
    int n;
    cin >> n;
    Tree tree(n);
    tree.inputTree();
    tree.populateSubtreeSize(1, 0);
    tree.sumDistancesFromRoot(1, 0, 0);
    tree.findTreeDistances();
    // tree.printSubtreeSizes();
    tree.printTreeDistances();

    return 0;
}