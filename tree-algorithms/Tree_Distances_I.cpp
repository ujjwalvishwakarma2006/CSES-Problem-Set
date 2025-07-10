/*
Key points to note in this problem:

 1. The farthest node from any node in the tree always lies on
    the diameter of the tree. One can prove this by considering
    two cases - either the farthest node is on the diameter or
    it is not. 

 2. Next we can claim that the farthest node from any node in 
    the tree will always be one of the two endpoints of the
    diameter of the tree. If it is not, it either lies closer
    to one of the endpoints or is the other endpoint itself.


 3. We can find the diameter of the tree by performing
    two DFS traversals. The first DFS will find the farthest node
    from an arbitrary node (This node will be one of the endpoints
    of the diameter from point number 2), and the second DFS will
    find the farthest node from that farthest node, which will 
    give us the diameter.

 4. Now, we can calculate the distance of each node from both
    endpoints of the diameter. The maximum of these two distances
    will be the distance of farthest node from that node.
    
*/

#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> adj;
vector<int> distancesFromE1;
vector<int> distancesFromE2;
   
// Let e1 and e2 be the endpoints of the diameter of the tree.
int e1 = -1, e2 = -1;
int maxDistance = 0;

// DFS to find the farthest node from a given node
void findFarthestNode(int node, int parent, int distance, int &endpoint) {
    if (distance > maxDistance) {
        maxDistance = distance;
        endpoint = node;
    }
    
    for (int neighbor : adj[node]) {
        if (neighbor == parent) continue;
        findFarthestNode(neighbor, node, distance + 1, endpoint);
    }
}

// DFS to calculate distances from the endpoints of the diameter
void calculateDistances(int node, int parent, int distance, vector<int> &distances) {
    distances[node] = distance;
    
    for (int neighbor : adj[node]) {
        if (neighbor == parent) continue;
        calculateDistances(neighbor, node, distance + 1, distances);
    }
}

int main() {
    int n;
    cin >> n;
    adj.resize(n + 1);
    distancesFromE1.resize(n + 1);
    distancesFromE2.resize(n + 1);
    int u, v;

    for (int i = 1; i <= n - 1; i++) {
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    findFarthestNode(1, -1, 0, e1);
    maxDistance = 0; // Reset for the second DFS
    findFarthestNode(e1, -1, 0, e2);

    // Calculate distances from both endpoints of the diameter
    calculateDistances(e1, -1, 0, distancesFromE1);
    calculateDistances(e2, -1, 0, distancesFromE2);

    for (int i = 1; i <= n; i++) {
        // The farthest distance from node i is the maximum of the distances
        // from the two endpoints of the diameter
        cout << max(distancesFromE1[i], distancesFromE2[i]) << " ";
    }

    return 0;
}
