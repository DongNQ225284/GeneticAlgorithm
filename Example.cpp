/*
Thuật toán di truyền cơ bản
begin
|    Khởi tạo một quần thể gồm N kiểu gen;
|    Tính độ thích nghi của từng cá thể;
|    while (chưa đạt điều kiện dừng) do
|    |    Chọn các cá thể cha mẹ từ quẩn thể hiện tại để tiến hành lai tạo;
|    |    while (chưa tạo đủ N cá thể con) do
|    |    |    Tiến hành lai ghép các cá thể cha mẹ với xác suất pc để tạo cá thể con;
|    |    |    Tiến hành đột biến cá thể con với xác xuất pm;
|    |    end
|    |    Chọn lọc cá thể trong quần thể mới để làm thế hệ tiếp theo;
|    end
end

Ví dụ để giải bài toán tìm giá trị nhỏ nhất của hàm số f(x) = (x - 5)^2 trên [0, 31]

B1. Mã hóa từ kiểu hình sang kiểu gen.
+-----------+----------+
| kiểu hình | kiểu gen |
+-----------+----------+
|     0     |   00000  |
|     2     |   00001  |
|    ...    |   .....  |   
|     31    |   11111  |
+-----------+----------+

B2. Khởi tạo quần thể (ngẫu nhiên) gồm 4 kiểu gen: 
+-----------+----------+
| kiểu hình | kiểu gen |
+-----------+----------+
|     21    |   10101  |
|     11    |   01011  |
|     25    |   11001  |   
|     2     |   00010  |
+-----------+----------+

B3. Đánh giá với fitness function: f(x) = (x - 5)^2
+-----------+----------+---------+
| kiểu hình | kiểu gen | fitness |
+-----------+----------+---------+
|     21    |   10101  |   256   |
|     11    |   01011  |   36    |
|     25    |   11001  |   400   |
|     2     |   00010  |    9    |
+-----------+----------+---------+

B4. Chọn ra 2 cá thể tốt nhất tiến hành crossover và mutation
+-----------+----------+---------+------------+----------+
| kiểu hình | kiểu gen | fitness |  crossover | mutation |
+-----------+----------+---------+------------+----------+
|     11    |  010|11  |   36    |   01010    |  01010   |
|     2     |  000|10  |    9    |   00011    |  00111   | 
+-----------+----------+---------+------------|----------|

B5. Xếp hạng và chọn lọc quần thể
+-----------+----------+---------+------+------+
| kiểu hình | kiểu gen | fitness | rank | chọn |
+-----------+----------+---------+------+------+
|     7     |   00111  |    4    |   1  |   x  |
|     2     |   00010  |    9    |   2  |   x  |
|     10    |   01010  |   25    |   3  |   x  |
|     11    |   01011  |   36    |   4  |   x  |
|     21    |   10101  |   256   |   5  |      |
|     25    |   11001  |   400   |   6  |      |
+-----------+----------+---------+------+------+

B6. Nếu tồn tại cá thể có fitness < 1 thì dừng, không thì quay lại B4.

*/
#include <bits/stdc++.h>
using namespace std;

class Solution {
private:
    int count = 0;
    int minimum = INT_MAX;
    string state;
    vector<int> All;
    vector<int> Population;
    
public:
    int fitness(int x) {
        return (x * x * x) / 1000 - 3 * x / 10 + 10;
    }

    void encode() {
        for (int i = 0; i < 32; i++) All.push_back(i);
        state = "done encode";
    }
    void selectPopulation() {
        if (state != "done encode") {
            cout << "Error! need to call encode function!" << endl;
            return;
        }
        for (int i = 0; i != 4; i++) {
            int pos = rand() % All.size();
            Population.push_back(All[pos]);
            All.erase(All.begin() + pos);
        }
        state = "done select population";
    }

    void evaluate() {
        if (state != "done select population" && state != "done transformation") {
            cout << "Error! need to call selectPopulation or transformation!" << endl;
            return;
        }
        sort(Population.begin(), Population.end(), [this](int a, int b) {
            return this->fitness(a) < this->fitness(b);
        });

        if (state == "done transformation") {
            Population.pop_back();
            Population.pop_back();
        }

        state = "done evaluate";
    }

    bool execute() {
        if (state != "done evaluate") {
            cout << "Error! need to call evaluate function => END" << endl;
            return false;
        }
        state = "done execute";
        
        minimum = min(fitness(Population[0]), minimum);
        if (fitness(Population[0]) - minimum < 5 && count > 2000) return false;
        if (count > 4000) return false;
        return true;
    }

    pair<int, int> selectParent() {
        if (state != "done execute") {
            cout << "Error! need to call execute function" << endl;
            return make_pair(0, 0);
        }
        state = "done select parent";
        return make_pair(Population[0], Population[1]);
    }

    void transformation(pair<int, int> parent) {
        if (state != "done select parent") {
            cout << "Error! need to call selectParent function" << endl;
            return;
        } 
        //crossover
        int x = parent.first;
        int y = parent.second;

        int a = x ^ (x >> 2);
        int b = y ^ (y >> 2);

        x = x >> 2; x = x << 2;
        y = y >> 2; y = y << 2;

        x |= b; y |= a;

        //mutation
        if (rand() % 100 < 50) x ^= (1 << (rand() % 5));
        if (rand() % 100 < 50) y ^= (1 << (rand() % 5));
        
        //union
        Population.push_back(x);
        Population.push_back(y);

        state = "done transformation";
    }

    void print(vector<int> arr) {
        for (int i = 0; i != arr.size(); i++) {
            cout << arr[i] << " ";
        }
        cout << endl;
    }

    int GA() {
        // char chr;
        encode();
        // print(All);
        // cin >> chr;
        selectPopulation();
        // print(Population);
        // cin >> chr;
        evaluate();
        // print(Population);
        // cin >> chr;
        while(execute()) {
            pair<int, int> parent = selectParent();
            transformation(parent);
            evaluate();
            // print(Population);
            // cout << state << endl;
            // cin >> chr;
            count++;
        }
        return Population[0];
    }
};

int main() {
    srand(time(0));
    Solution obj;
    cout << obj.GA();
    return 0;
}