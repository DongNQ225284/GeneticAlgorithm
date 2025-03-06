#include <bits/stdc++.h>
using namespace std;


class Encoder {
public:
    
};

enum class ENCODING_TYPE {
    BINARY = 0,
    PERMUTATION = 1,
    ADJACENCY = 2,
    REALVALUED = 3
};


template <typename T>
void print(vector<T> arr) {
    for (size_t i = 0; i != arr.size(); i++) {
        cout << arr[i] << "\t";
    }
    cout << endl;
}

template <typename T>
void print(vector<vector<T>> arr) {
    for (size_t i = 0; i != arr.size(); i++) {
        for (size_t j = 0; j != arr[i].size(); j++) {
            cout << arr[i][j] << "\t";
        }
        cout << endl;
    }
}

vector<double> rankbasedFitnessWithNiching(vector<double> fitnessValues) {
    vector<double> probabilityValues;
    size_t n = fitnessValues.size();

    for (size_t i = 0; i != n; i++) {
        double p = 0;
        for (size_t j = 0; j != n; j++) {
            if (i != j) p += 1.0 / abs(fitnessValues[i] - fitnessValues[j]);
        }
        probabilityValues.push_back(p);
    }
    
    return probabilityValues;
}


int main() {
    vector<double> a = {5.2, 4.8, 4.6, 4.7, 0.5};
    print(rankbasedFitnessWithNiching(a));
}