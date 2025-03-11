#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

static random_device rd;
static mt19937 gen(rd()); 

enum class typeRating{
    STANDARD,
    RANKBASED,
    NICHING
};

vector<bool> decode(vector<double> chromosome) {
    vector<bool> phenotype(chromosome.size());
    for (size_t i = 0; i < chromosome.size(); i++) {
        phenotype[i] = (chromosome[i] >= 0.5);
    }
    return phenotype;
}

class Problem {
private:
    size_t num_item;
    vector<double> profit_list;
    vector<double> weight_items;
    double capacity;

public:
    Problem(){};
    Problem(vector<double> profits, vector<double> weights, double capac) {
        this->num_item = profits.size();
        this->profit_list = profits;
        this->weight_items = weights;
        this->capacity = capac;
    }

    size_t getNumberItem() {
        return this->num_item;
    }

    vector<double>& getProfitList() {
        return this->profit_list;
    }

    vector<double>& getWeightItems() {
        return this->weight_items;
    }

    double getCapacity() {
        return this->capacity;
    }

    void show() {
        printf("num_item = %d\t capacity = %.2lf\n", num_item, capacity);
        printf("profix: {\t"); for (double it : profit_list) printf("%.2lf\t", it); printf("}\n");
        printf("weight: {\t"); for (double it : weight_items) printf("%.2lf\t", it); printf("}\n");
    }
};

double cal_metric(vector<bool> phenotype, Problem problem) {
    double profit = 0;
    double total_weight = 0;
    for (size_t i = 0; i < problem.getNumberItem(); i++) {
        if (phenotype[i]) {
            profit += problem.getProfitList()[i];
            total_weight += problem.getWeightItems()[i];
        }
    }
    if (total_weight > problem.getCapacity()) return 0;
    else return profit;
}

class Rater {
public:
    vector<double> standardFitness(vector<double> fit_val) {
        size_t n = fit_val.size();
        double F = 0;
        for (size_t i = 0; i != n; i++) {
            F += fit_val[i];
        }
        
        vector<double> prob_val;
        for (size_t i = 0; i != n; i++) {
            prob_val.push_back(fit_val[i]/F);
        }
        return prob_val;
    }

    vector<double> rankbasedFitness(vector<double> fit_val, double p = 0) {
        size_t n = fit_val.size();
        vector<size_t> indices(n);
        for (size_t i = 0; i != n; i++) indices[i] = i;

        sort(indices.begin(), indices.end(), [&](size_t a, size_t b) {
            return fit_val[a] > fit_val[b];
        });

        for (int i = 0; i != indices.size(); i++) cout << indices[i] << " "; cout << endl;

        double F = (double)(n * (n + 1.0)) / 2.0;

        vector<double> prob_val(n);
        double q = 1.0;
        for (size_t i = 0; i != n; i++) {
            prob_val[i] = p * q;
            q *= 1.0 - p;
        }
        
        sort(prob_val.begin(), prob_val.end(), [&](size_t a, size_t b) {
            return indices[a] < indices[b];
        });

        return prob_val;
    }

    vector<double> rankbasedFitnessWithNiching(vector<double> fit_val) {
        vector<double> prob_val;
        size_t n = fit_val.size();
        
        for (size_t i = 0; i < n; i++) {
            double p = 0;
            for (size_t j = 0; i < n; i++) {
                if (i != j) {
                    p += 1.0 / abs(fit_val[i] - fit_val[j]);
                }
            }
            prob_val.push_back(p);
        }
        return prob_val;
    }
};

class Individual {
private:
    vector<double> chromosome;
    double fitness;
    
public:
    vector<double>& getChromosome() {
        return this->chromosome;
    }

    double getFitness() {
        return this->fitness;
    }

    void generateInit(Problem problem) {
        uniform_real_distribution<double> dis(0.0, 1.0); 
        for (size_t i = 0; i < problem.getNumberItem(); i++) {
            chromosome.push_back(dis(gen));            
        }
    }

    void calculateFitness(Problem problem) {
        vector<bool> phenotype = decode(this->chromosome);
        this->fitness = cal_metric(phenotype, problem);
    }

    bool operator!= (Individual other) {
        vector<double> chromosomeOther = other.getChromosome();
    
        if (this->chromosome.size() != chromosomeOther.size()) return true;
        for (size_t i = 0; i < chromosome.size(); i++) {
            if (this->chromosome[i] != chromosomeOther[i]) return true;
        }
        return false;
    }

    void show() {
        vector<bool> phenotype = decode(chromosome);
        printf("fitness = %.2lf\t\t", fitness);
        printf("chormosome: { "); for (double it : chromosome) printf("%.2lf ", it); printf("}\t\t");
        printf("phenotype: { "); for (size_t it : phenotype) printf("%d ", it); printf("}\n");
    }
};

class Population {
private:
    size_t pop_size;
    Problem problem;
    vector<Individual> indi_list;

public:
    Population(size_t num, Problem problem) {
        pop_size = num;
        this->problem = problem;
    }

    size_t getSize() {
        return pop_size;
    }

    vector<Individual> getList() {
        return indi_list;
    }

    void gen_pop() {
        for (size_t i = 0; i < this->pop_size; i++) {
            Individual indi;
            indi.generateInit(this->problem);
            indi.calculateFitness(this->problem);
            indi_list.push_back(indi);
        }
    }

    void add(Individual indi) {
        this->indi_list.push_back(indi);
        this->pop_size++;
    }

    void select(typeRating rate) {
        vector<double> fit_val;
        for (size_t i = 0; i < pop_size; i++) {
            fit_val.push_back(indi_list[i].getFitness());
        }
        Rater rater;
        vector<double> prob_val;

        if (rate == typeRating::STANDARD) {
            prob_val = rater.standardFitness(fit_val);
        } else if (rate == typeRating::RANKBASED) {
            prob_val = rater.rankbasedFitness(fit_val);
        } else {
            prob_val = rater.rankbasedFitnessWithNiching(fit_val);
        }

        vector<double> pre_sum(pop_size);
        pre_sum[0] = prob_val[0];
        for (size_t i = 1; i < pop_size; i++) {
            pre_sum[i] = prob_val[i] + pre_sum[i - 1];
        }
        
        static random_device rd;
        static mt19937 gen(rd());
        uniform_real_distribution<double> dis(0.0, 1.0);

        vector<bool> mark(pop_size, false);
        vector<Individual> new_gen;

        size_t num = pop_size / 2;
        while (num > 0) {    
            double r = dis(gen);
            size_t idx = lower_bound(pre_sum.begin(), pre_sum.end(), r) - pre_sum.begin();
            if (!mark[idx]) {
                new_gen.push_back(indi_list[idx]);
                mark[idx] = true;
                num--;
            }
        }
        indi_list.clear();
        indi_list = new_gen;
        pop_size = indi_list.size();
    }

    void show() {
        printf("pop_size = %d\n", pop_size);
        for (size_t i = 0; i < pop_size; i++) {
            indi_list[i].show();
        }
    }
};

vector<Individual> crossover(Individual indi1, Individual indi2) {
    vector<Individual> child;
    vector<double> chro1 = indi1.getChromosome();
    vector<double> chro2 = indi2.getChromosome();

    size_t n = indi1.getChromosome().size();
    uniform_int_distribution<int> dis(0, n - 1);

    size_t num = dis(gen);
    while(num--) {
        size_t pos = dis(gen);
        swap(indi1.getChromosome()[pos], indi2.getChromosome()[pos]);
    }
    child.push_back(indi1);
    child.push_back(indi2);
    return child;
}

void mutation(Individual& indi, double p = 0.5) {
    uniform_real_distribution<double> dis(0.0, 1.0);
    for (size_t i = 0; i < indi.getChromosome().size(); i++) {
        if (dis(gen) < p) (indi.getChromosome())[i] = dis(gen);
    }
};

vector<Individual> randomIndividual(vector<Individual> list, size_t num) {
    vector<Individual> listParent;
    if (num > list.size()) {
        cout << "Error!" << endl;
        return listParent;
    }

    uniform_int_distribution<int> dis(0, list.size() - 1);
    vector<bool> mark(list.size());
    
    while(num > 0) {
        size_t pos = dis(gen);
        if (!mark[pos]) {
            mark[pos] = true;
            listParent.push_back(list[pos]);
            num--;
        }
    }
    return listParent;
}

Individual GeneticAlgorithm(Problem problem, size_t pop_size, size_t num_gen, double pm, double pc) {
    uniform_real_distribution<double> dis(0.0, 1.0);
    Population pop(pop_size, problem);
    pop.gen_pop();
    int num_child;
    while (num_gen > 0) {
        vector<Individual> listParent = randomIndividual(pop.getList(), 5);
        num_child = pop.getSize();
        while (num_child > 0) {
            if (dis(gen) <= pc) {
                vector<Individual> parent = randomIndividual(listParent, 2);
                vector<Individual> child = crossover(parent[0], parent[1]);
                if (dis(gen) <= pm) {
                    mutation(child[0]);
                    mutation(child[1]);
                }
                child[0].calculateFitness(problem);
                child[1].calculateFitness(problem);

                if (child[0].getFitness() > 0) {
                    pop.add(child[0]);
                    num_child--;
                }
                if (child[1].getFitness() > 0) {
                    pop.add(child[1]);
                    num_child--;
                }
            }
        }
        pop.select(typeRating::STANDARD);
        num_gen--;
    }
    vector<Individual> list = pop.getList();
    Individual best = list[0];
    for (size_t i = 0; i < list.size(); i++) {
        if (best.getFitness() < list[i].getFitness()) best = list[i];
    }
    return best;
}

int main() {
    vector<double> profit = {20, 12, 23, 21, 6, 12, 25};
    vector<double> weight = { 7,  5, 10,  8, 4, 10, 23};
    double capacity = 25;

    Problem knapsack_problem(profit, weight, capacity);
    
    knapsack_problem.show();

    size_t pop_size = 15;
    size_t num_gen = 1500;
    double pm = 0.5;
    double pc = 0.2;

    Individual solution = GeneticAlgorithm(knapsack_problem, pop_size, num_gen, pm, pc);
    
    cout << endl;
    cout << "best solution:" << endl;
    solution.show();
    return 0;
}