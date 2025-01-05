#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class RoseBouquet {
public:
    RoseBouquet(int n) : n(n) {
        bouquet.resize(n, vector<int>(n, 0));
    }

    void generateBouquet() {
        int center = n / 2;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                bouquet[i][j] = static_cast<int>(sqrt(pow(i - center, 2) + pow(j - center, 2)));
            }
        }
    }

    void displayBouquet() {
        for (const auto& row : bouquet) {
            for (int val : row) {
                cout << val << " ";
            }
            cout << endl;
        }
    }

private:
    int n;
    vector<vector<int>> bouquet;
};

int main() {
    int size;
    cout << "Enter the size of the bouquet: ";
    cin >> size;

    RoseBouquet rb(size);
    rb.generateBouquet();
    rb.displayBouquet();

    return 0;
}
