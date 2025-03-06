/*
Thuật toán Genetic Algorithm - GA

begin
|   Mã hóa kiểu gen -> society;
|   Khởi tạo quần thể có N kiểu gen -> population;
|   Tính độ thích nghi (fitness) của từng cá thể;
|   while (chưa đạt điều kiện dừng) do
|   |   Chọn các cá thể từ quần thể hiện tại làm cá thể cha mẹ -> parents;
|   |   while (chưa đạt N cá thể con) do
|   |   |   Tiến hành lai ghép (crossover) các cá thể cha mẹ đã chọn với xác suất p_c để tạo thành các cá thể con -> childent;
|   |   |   Tiến hành đột biến (mutation) cá thể con với xác suất p_m;
|   |   end
|   |   Tạo quần thể mới từ N cá thể con mới và quần thể ở thế hệ trước;
|   end
end

Các kiểu mã hóa. 
- Mã hóa nhị phân
    Ví dụ: Bài toán cái túi - Knapsack Problem - KP
    Cho n đồ vật với cân nặng và w_i và giá trị sử dụng là v_i, biết không thể mang quá maximum, hãy tính giá trị lớn nhất có thể mang. 
    Mã hóa lời giải cho bài toán là một chuỗi bit, với b_i là trạng thái tương ứng với đồ vật có giá trị v_i và cân nặng w_i, với 1 thể hiện mang theo và 0 thì ngược lại.
    +----+----------------------------------+
    | MS | 1     2       3       4       5  |
    +----+----------------------------------+
    | v  | 1     4       4       3       8  |
    +----+----------------------------------+
    | w  | 2     5       3       4       10 |
    +----+----------------------------------+
    | b  | 0     1       1       1       0  |
    +----+----------------------------------+
    Với ví dụ ở bảng trên, các đồ vật sẽ được đem theo là {2, 3, 5}

- Mã hóa hoán vị: 
    Ví dụ: Bài toán người đi du lịch Traveling Salesman Problem - TSP
    Một người từ thành phố 1 muốn tham quan các thành phố 2, 3, .. n và quay trở lại thành phố ban đầu, biết chi phí giữa mỗi thành phố, tìm hành trình tốn ít chi phí nhất.
    Mã hóa cho lời giải là một dãy số là hoán vị của 1, 2, 3, ... n.
    +---+---+---+---+---+
    | 1 | 3 | 2 | 5 | 4 |
    +---+---+---+---+---+
    Với ví dụ bảng trên, hành trình sẽ là 1 -> 3 -> 2 -> 5 -> 4 -> 1

- Mã hóa kề
    Ví dụ: Vẫn là bài toán người du lịch. Ta có một cách mã hóa khác, được mô tả như sau. Nếu hành trình di chuyển từ i -> j, thì vị trí thứ i nhận giá trị j.
    Với hành trình 1 -> 3 -> 2 -> 5 -> 4 -> 1 ta mã hóa như sau:
    +---+---+---+---+---+
    | 3 | 5 | 2 | 1 | 4 |
    +---+---+---+---+---+

- Mã hóa số thực
    Ví dụ: Tìm giá trị nhỏ nhất của hàm số f(x, y, z) = xy + 3z^2 + 2 trên tập số thực
    Mã hóa lời giải là bộ ba (x, y, z), giả sử (x, y, z) = (1.2, 3, 2.1) ta mã hóa như sau:
    +---+---+---+
    |1.2|3.0|2.1|
    +---+---+---+

==> Tổng quát lại ta có thể thấy các cách mã hóa ở trên có một điểm chung là một dãy số, trong lập trình ta sẽ dùng vector<T> để biểu diễn kiểu gen.

Khởi tạo quần thể -> vector<vector<T>> populutation

Các phương pháp chọn lọc cá thể cha mẹ
- Chọn lọc ngẫu nhiên - random selection - RS
    Mô tả: Lấy ngẫu nhiên một số lượng cá thể trong tập quần thể - population để bổ sung vào tập cha mẹ - parents

- Chọn lọc theo giá trị thích nghi - fitness proportion selection - FPS
    + Bánh xe Roulete: Từ tập các giá trị thích nghi {f_1, f_2, ..., f_n} chuyển sang tập giá trị xác suất {p_1, p_2, ..., p_n} sau đó tiến hành chọn trên tập xác suất. 
    + Chọn lọc giao đấu - tournament selection: Chọn ngẫu nhiên k cá thể từ quần thể, dựa vào độ thích nghi, chọn ra cá thể tốt nhất. Lặp lại cho tới khi đủ số lượng cha mẹ.
    + Chọn lọc thứ hạng - rank selection: sắp xếp lại giá trị thích nghi, với mỗi cá thể có một hạng {1, 2, ..., n} chuyển sang tập xác suất rồi tiến hành chọn trên tập xác suất.


Độ thích nghi
- Độ thích nghi tiêu chuẩn: Từ hàm mục tiêu f, ta tính được các giá trị {f_1, f_2, ..., f_n} chuyển sang tập xác suất {F_1, F_2, ..., F_n} đây là độ thích nghi tiêu chuẩn.
- Độ thích nghi xếp hạng: Từ hàm mục tiêu f, ta tính được các giá trị {f_1, f_2, ...., f_n} xếp hạng các giá trị tăng dần và chuyển sang tập xác suất {P_1, P_2, ..., P_n} đây là độ thích nghi xếp hạng.
=> Các tập giá trị khi quy sang độ thích nghi tiêu chuẩn / xếp hạng ta sẽ nhận được giá trị thuộc [0, 1], như vậy trong quá trình chọn lọc sẽ đỡ phức tạp hơn trong quá trình chuyển đổi sang tập xác suất. 
=> Quá trình chọn lọc giống với chọn lọc Roulete


*/
#include <bits/stdc++.h>
using namespace std;

class getPopulation {
public:
    vector<vector<bool>> getBinaryPopulation(size_t length, size_t quantity) {
        vector<vector<bool>> population;
        static random_device random;
        static mt19937 generate(random());
        uniform_int_distribution<int> distribution(0, 1);

        while (quantity > 0) {
            vector<bool> element(length);
            for (size_t i = 0; i != length; i++) {
                element[i] = distribution(generate);
            }
            population.push_back(element);
            quantity--;
        }
        return population;
    }

    vector<vector<int>> getPermutationPopulation(size_t length, size_t quantity) {
        vector<vector<int>> population;
        static random_device random;
        static mt19937 generate(random());
        uniform_int_distribution<int> distribution(1, length);

        while (quantity > 0) {
            vector<bool> mark(length + 1, false);
            vector<int> element(length);
        
            for (size_t i = 0; i != length; i++) {
                int r = -1;
                do {
                    r = distribution(generate);
                } while (mark[r]);
                mark[r] = true;
                element[i] = r;
            }

            population.push_back(element);
            quantity--;
        }
        return population;
    }

    vector<vector<int>> getAdjacencyPopulation(size_t length, size_t quantity) {
        vector<vector<int>> population;
        static random_device random;
        static mt19937 generate(random());
        uniform_int_distribution<int> distribution(0, length - 1);

        while (quantity > 0) {
            vector<bool> mark(length + 1, false);
            vector<int> element(length);

            size_t firstPosition = -1;
            do {
                firstPosition = distribution(generate);
            } while (firstPosition == 0);

            element[firstPosition] = 1;
            element[length - 1] = firstPosition + 1;
            mark[1] = mark[firstPosition + 1] = true;
            
            for (size_t i = 0; i != length - 1; i++) {
                if (i == firstPosition) continue;
                size_t r = -1;
                do {
                    r = 1 + distribution(generate);
                } while (mark[r]);
                mark[r] = true;
                element[i] = r;
            }
            population.push_back(element);
            quantity--;
        }
        return population;
    }

    vector<vector<double>> getRealvaluedPopulation(size_t length, pair<double, double> range, size_t quantity) {
        vector<vector<double>> population;
        static random_device random;
        static mt19937 generate(random());
        uniform_real_distribution<double> distribution(range.first, range.second);

        while (quantity > 0) {
            vector<double> element(length);
            for (int i = 0; i != length; i++) {
                double r = distribution(generate);
                element[i] = r;
            }

            population.push_back(element);
            quantity--;
        }
        return population;
    }
};
    
class Evaluation {
public:
    vector<double> standardFitness(vector<double> fitnessValues) {
        size_t n = fitnessValues.size();
        double F = 0;
        for (size_t i = 0; i != n; i++) {
            F += fitnessValues[i];
        }
        
        vector<double> probabilityValues;
        for (size_t i = 0; i != n; i++) {
            probabilityValues.push_back(fitnessValues[i]/F);
        }
        return probabilityValues;
    }

    vector<double> rankbasedFitness(vector<double> fitnessValues, double p = 0) {
        size_t n = fitnessValues.size();
        vector<size_t> indices(n);
        for (size_t i = 0; i != n; i++) indices[i] = i;

        sort(indices.begin(), indices.end(), [&](size_t a, size_t b) {
            return fitnessValues[a] > fitnessValues[b];
        });

        for (int i = 0; i != indices.size(); i++) cout << indices[i] << " "; cout << endl;

        double F = (double)(n * (n + 1.0)) / 2.0;


        vector<double> probabilityValues(n);
        double q = 1.0;
        for (size_t i = 0; i != n; i++) {
            probabilityValues[i] = p * q;
            q *= 1.0 - p;
        }
        
        sort(probabilityValues.begin(), probabilityValues.end(), [&](size_t a, size_t b) {
            return indices[a] < indices[b];
        });

        return probabilityValues;
    }

    vector<double> rankbasedFitnessWithNiching(vector<double> fitnessValues) {
        vector<double> probabilityValues;
        size_t n = fitnessValues.size();
        vector<double> f = fitnessValues;
        vector<size_t> indices(n);
        for (size_t i = 0; i != n; i++) indices[i] = i;

        sort(indices.begin(), indices.end(), [&](size_t a, size_t b) {
            return f[a] < f[b];
        });
        sort(f.begin(), f.end());

        vector<double> delta(n, 0);
        for (size_t i = 0; i != n; i++) {
            delta[0] += f[i] - f[0]; 
        }

        for (size_t i = 1; i != n; i++) {
            delta[i] = delta[i - 1] + (f[i] - f[i - 1]) * (i - n);
        }
        for (size_t i = 0; i != n; i++) {
            probabilityValues.push_back(1.0/delta[i]);
        }
        sort(probabilityValues.begin(), probabilityValues.end(), [&](size_t a, size_t b) {
            return indices[a] < indices[b];
        });
        return probabilityValues;
    }
};

template <typename T>
class randomSelection {
public:
    vector<vector<T>> getParents(vector<vector<T>> population, size_t quantity) {
        vector<vector<T>> parents;
        if (population.size() == 0) {
            cout << "Error! population is empty! => Return empty vector." << endl;
            return parents;
        }
        if (quantity > population.size()) {
            cout << "Error! The quantity must be less than or equal to the number of individuals in the population! => Return empty vector." << endl;
            return parents;
        }
        size_t n = population.size();

        static random_device random;
        static mt19937 generate(random());
        uniform_int_distribution<int> distribution(0, n - 1);

        vector<bool> markSelection(n, false);
        while (quantity > 0) {    
            size_t r = distribution(generate);
            if (!markSelection[r]) {
                parents.push_back(population[r]);
                markSelection[r] = true;
                quantity--;
            }
        }
        return parents;
    }    
};

template <typename T>
class fitnessProportionSelection {
public:
    vector<vector<T>> getParentsByRouleteSelection(vector<vector<T>> population, vector<double> fitnessValues, size_t quantity) {
        vector<vector<T>> parents;
        if (population.size() == 0) {
            cout << "Error! population is empty! => Return empty vector." << endl;
            return parents;
        }
        if (quantity > population.size()) {
            cout << "Error! The quantity must be less than or equal to the number of individuals in the population! => Return empty vector." << endl;
            return parents;
        }

        size_t n = fitnessValues.size();
        double F = 0;
        for (size_t i = 0; i != n; i++) {
            F += fitnessValues[i];
        }
        
        vector<double> probabilityValues;
        for (size_t i = 0; i != n; i++) {
            probabilityValues.push_back(fitnessValues[i]/F);
        }

        vector<double> prefixSum(n);
        prefixSum[0] = probabilityValues[0];
        for (size_t i = 1; i != n; i++) {
            prefixSum[i] = probabilityValues[i] + prefixSum[i - 1];
        }
        
        //initialize for random number in [0, 1] 
        static random_device random;
        static mt19937 generate(random());
        uniform_real_distribution<double> distribution(0.0, 1.0);

        vector<bool> markSelection(n, false);
        while (quantity > 0) {    
            double r = distribution(generate); //random number in [0, 1]
            size_t index = lower_bound(prefixSum.begin(), prefixSum.end(), r) - prefixSum.begin();
            if (!markSelection[index]) {
                parents.push_back(population[index]);
                markSelection[index] = true;
                quantity--;
            }
        }
        return parents;
    }

    vector<vector<T>> getParentsByTournamentSelection(vector<vector<T>> population, vector<double> fitnessValues, size_t quantity, size_t k) {
        vector<vector<T>> parents;
        if (population.size() == 0) {
            cout << "Error! population is empty! => Return empty vector." << endl;
            return parents;
        }
        if (quantity > population.size()) {
            cout << "Error! The quantity must be less than or equal to the number of individuals in the population! => Return empty vector." << endl;
            return parents;
        }
        if (k > population.size()) {
            cout << "Error! k must be less than or equal to the population size! => Return empty vector." << endl;
            return parents;
        }

        size_t n = population.size();
        static random_device random;
        static mt19937 generate(random());
        uniform_int_distribution<int> distribution(0, n - 1);
        
        vector<bool> markSelection(n, false);
        while (quantity > 0) {
            unordered_set<size_t> indices;
            while (indices.size() < k) {
                indices.insert(distribution(generate));
            }

            double fitnessMaximun = fitnessValues[*indices.begin()];
            size_t bestIndex = *indices.begin();

            for (size_t i : indices) {
                if (fitnessValues[i] > fitnessMaximun) {
                    fitnessMaximun = fitnessValues[i];
                    bestIndex = i;
                }
            }

            if (!markSelection[bestIndex]) {
                parents.push_back(population[bestIndex]);
                markSelection[bestIndex] = true;
                quantity--;
            }
        }
        return parents;
    }

    vector<vector<T>> getParentsByRankSelection(vector<vector<T>> population, vector<double> fitnessValues, size_t quantity) {
        vector<vector<T>> parents;
        if (population.size() == 0) {
            cout << "Error! population is empty! => Return empty vector." << endl;
            return parents;
        }
        if (quantity > population.size()) {
            cout << "Error! The quantity must be less than or equal to the number of individuals in the population! => Return empty vector." << endl;
            return parents;
        }

        size_t n = fitnessValues.size();
        vector<size_t> indices(n);
        for (size_t i = 0; i != n; i++) indices[i] = i;

        sort(indices.begin(), indices.end(), [&](size_t a, size_t b) {
            return fitnessValues[a] > fitnessValues[b];
        });

        double S = n * (n + 1) / 2.0;

        vector<double> prefixSum(n);
        prefixSum[0] = 1.0 / S;
        for (size_t i = 1; i != n; i++) {
            prefixSum[i] = prefixSum[i - 1] + (double)(i + 1) / (double)S;
        }

        static random_device random;
        static mt19937 generate(random());
        uniform_real_distribution<double> distribution(0.0, 1.0);

        vector<bool> markSelection(n, false);

        while (quantity > 0) {
            double r = distribution(generate);
            size_t index = lower_bound(prefixSum.begin(), prefixSum.end(), r) - prefixSum.begin();

            size_t selectedIndex = indices[index];
            if (!markSelection[selectedIndex]) {
                parents.push_back(population[selectedIndex]);
                markSelection[selectedIndex] = true;
                quantity--;
            }
        }
        return parents;
    }
};

int main() {
    cout << "Hello";
}