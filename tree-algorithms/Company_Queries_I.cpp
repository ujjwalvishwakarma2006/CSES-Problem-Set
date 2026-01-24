#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

const int MAX_LOG = 19;     // Max possible log if n = 2 * 10^5
int numEmployees, numQueries;
vector<vector<int>> boss;

void binaryLift() {
    for (int level = 1; level < MAX_LOG; ++level) {
        for (int employee = 2; employee <= numEmployees; ++employee) {
            boss[employee][level] = boss[boss[employee][level - 1]][level - 1];
        }
    }
}

int bossKLevelAbove(int employee, int k) {
    while (k > 0) {
        employee = boss[employee][int(log2(k))];
        k -= 1 << int(log2(k));
        if (employee == 0) return -1;
    }
    return employee;
}

int main() {

    cin >> numEmployees >> numQueries;
    
    boss.resize(numEmployees + 1, vector<int>(MAX_LOG, 0));
    for (int i = 2; i <= numEmployees; ++i) cin >> boss[i][0];

    binaryLift();
    
    while (numQueries--) {
        int employee, k;
        cin >> employee >> k;
        cout << bossKLevelAbove(employee, k) << endl;
    }

    return 0;
}