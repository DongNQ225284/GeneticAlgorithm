#include <iostream>
#include <vector>
#include <random>

using namespace std;

static random_device rd;
static mt19937 gen(rd());

//vấn đề
class Problem {
private:
    //input
public:
    //method
}

class Individual {
private:
    //nhiễm sắc thể
    vector<float> chromosome; 
    //giá trị thích nghi
    double fitness;
public:
    void init(Problem environment) {
        size_t length; //cần tính chiều dài nhiễm sắc thể khi biết môi trường sống
        //khởi tạo nhiễm sắc thể với chiều dài length mỗi gen là một số thực thuộc [0, 1]        
        uniform_real_distribution<float> dis(0.0, 1.0);
        for (size_t i = 0; i != length; i++) {
            float r = dis(gen);
            chromosome.push_back(r);
        }
    }
    //tính giá trị thích nghi
    void calculateFitness(Problem environment) {
        //giải mã nhiễm sắc thể
        vector<int> phenotype = decode(this->chromosome);
        //tính giá trị thích nghi
        this->fitness = calculateMetric(phenotype, environment);
    }

    //lấy chiều dài của nhiễn sắc thể
    size_t getLength() {
        return this->chromosome.size();
    }
    //lấy nhiễm sắc thể
    vector<float>& getChromosome() {
        return this->chromosome;
    }
    //lấy giá trị thích nghi
    double getFitness() {
        return this->fitness;
    }
    //hiển thị thông tin của cá thể
    void show() {
        vector<int> phenotype = decode(chromosome);
        printf("fitness = %.2lf\t\t", fitness);
        printf("chormosome: { "); for (double it : chromosome) printf("%.2lf ", it); printf("}\t\t");
        printf("phenotype: { "); for (size_t it : phenotype) printf("%d ", it); printf("}\n");
    }
};

class Population {
private:
    //danh sách cá thể trong quần thể
    vector<Individual> list;
    //môi trường sống
    Problem environment;
public:
    //khởi tạo quần thể với n cá thể trong môi trường sống
    void init(Problem environment, size_t n) {
        this->environment = environment;
        for (size_t i = 0; i != n; i++) {
            Individual member;
            member.init(environment);
            member.calculateFitness(environment);
            list.push_back(member);
        }
    }
    //lấy kích thước của quần thể
    size_t getSize() {
        return this->individual_list.size();
    }
    //lấy danh sách các cá thể
    vector<Individual>& getList() {
        return this->list;
    }
    //thêm một cá thể vào trong danh sách
    void append(Individual member) {
        this->list.push_back(member);
    }
    
    //bình chọn n đại diện làm cha mẹ
    vector<Individual> getParent(size_t n) {
        vector<Individual> listParent;
        //
        return listParent;
    }

    //chọn lọc sinh tồn
    void selection() {

    }
    //hiển thị thông tin về quần thể
    void show() {
        for (size_t i = 0; i != list.size(); i++) {
            cout << "size of populaiton: " << list.size() << endl;
            list[i].show();
        }
    }
};

//các phép toán với cá thể

//giải mã cá thể khi biết nhiễm sắc thể và môi trường sống
vector<int> decode(vector<float> chromosome, Problem environment) {
    vector<int> phenotype;
    //decode

    return phenotype;
}

//đánh giá cá thể trong môi trường sống
double calculateMetric(Problem environment) {
    //calculate
}

//toán tử lai tạo
vector<float> crossover(Individual p1, Individual p2) {
    vector<float> offspring;
    //crossover
    return offspring;
}

//toán tử đột biến
void (Individual& member) {
    //mutation
}

//Thuật toán GA trả về cá thể tốt nhất
Individual geneticalgorithm(Problem environment, //môi trường sống
                            size_t number_of_individuals, //số lượng cá thể trong môi trường
                            size_t number_of_generation, //số lượng thế hệ
                            float p_m, //tỷ lệ lai tạo
                            float p_c //tỷ lệ đột biến
                            )  
{
    uniform_real_distribution<float> dis(0.0, 1.0);
    Individual best;
    Population population;
    population.init(environment, number_of_individuals);
    while (number_of_generation) {
        vector<Individual> parent = population.getParent(2);
        size_t number_of_offspring = number_of_individuals;
        while (number_of_offspring) {
            float r_m = dis(gen);
            if (r_m <= p_m) {
                vector<Individual> offspring = crossover(parent[0], parent[1]);
                float r_c = dis(gen);
                if (r_c <= p_c) {
                    mutation(offspring[0]);
                    mutation(offspring[1]);
                }
                offspring[0].calculateFitness(environment);
                offspring[1].calculateFitness(environment);
            }
            if (offspring[0].getFitness() > 0) {
                population.append(offspring[0]);
                number_of_offspring--;
            }
            if (offspring[1].getFitness() > 0) {
                population.append(offspring[1]);
                number_of_offspring--;
            }
        }
        population.selection();
        number_of_generation--;
    }
    //chọn ra cá thể có giá trị thích nghi tốt nhất
    vector<Individual> list = population.getList();
    best = list[0];
    for (size_t i = 1; i != list.size(); i++) {
        if (list[i].getFitness() > best.getFitness()) {
            best = list[i];
        }
    } 
    return best;
}

int main() {

}