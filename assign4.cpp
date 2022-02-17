#include<iostream>
#include<algorithm>
#include<vector>
#include<time.h>
using namespace std;

#define num_queen 30
#define POPULATION_SIZE 100
#define num_island 10

int random_num(int start, int end)
{
    int range = (end - start) + 1;
    int random_int = start + (rand() % range);
    return random_int;
};
int* create_gnome()
{
    int len = num_queen;
    int* gnome = new int[num_queen];
    for (int i = 0; i < len; i++)
        gnome[i] = random_num(0, len - 1);
    return gnome;
};
class Individual
{
public:
    int chromosome[num_queen];
    int fitness;
    Individual(int* addr_chromosome);
    int cal_fitness(int chromosome[num_queen]);
    Individual crossover(Individual parent2);
};
class island 
{
public:
    island(vector<Individual> population);
    vector<Individual> population;
    void migrate(vector<Individual> neighbor_from);

};
island::island(vector<Individual> population)
{
    this->population = population;
}
void island::migrate(vector<Individual> neighbor_from)
{
    for (int i = 0; i < POPULATION_SIZE / 10; i++)
    {
        int position = random_num(0, POPULATION_SIZE-1);
        for (int i = 0; i < num_queen ; i++) {
            this->population[position].chromosome[i] 
                = neighbor_from[position].chromosome[i];
        }
    }
}
Individual::Individual(int* addr_chromosome)
{
    for (int i = 0; i < num_queen; i++)
        this->chromosome[i] = *(addr_chromosome + i);
    fitness = cal_fitness(chromosome);
};

int Individual::cal_fitness(int chromosome[num_queen])
{
    int fitness = 0;
    for (int i = 0; i < num_queen; i++)
    {
        for (int j = 0; j < num_queen; j++)
        {
            if (i != j)
            {
                if ((chromosome[i] == chromosome[j]) or
                    (abs(chromosome[i] - chromosome[j]) == abs(i - j)))
                {
                    fitness++;
                    break;
                }
            }
        }
    }
    return fitness;
};
Individual Individual::crossover(Individual parent2) {
    int child_chromosome[num_queen];
    int len = num_queen;
    for (int i = 0; i < len; i++)
    {
        int p = random_num(0, 100);
        if (p < 45)
            child_chromosome[i] = chromosome[i];
        else if (p < 90)
            child_chromosome[i] = parent2.chromosome[i];
        else
            child_chromosome[i] = random_num(0, num_queen - 1);
    }
    return Individual(child_chromosome);
}
bool comp(const Individual& a, const Individual& b)
{
    return a.fitness < b.fitness;
}

int main() {
    srand((unsigned)(time(0)));
    int generation = 0;
    double dur;
    clock_t start, end;
    start = clock();
    bool found = false;
    vector<island> group;
    // create initial population
    for (int i = 0; i < num_island; i++) 
    {
        vector<Individual> population;
        for (int j = 0; j < POPULATION_SIZE; j++)
        {
            int* gnome = create_gnome();
            population.emplace_back(Individual(gnome));
        }
        group.emplace_back(population);
    }
    // do crossover and muatation
    while (!found)
    {
        for (int i = 0; i < num_island; i++) 
        {
            sort(group[i].population.begin(), group[i].population.end(), comp);
        }

        for (int m = 0; m < num_island; m++)
        {
            vector<Individual> new_generation;
            int s = (10 * POPULATION_SIZE) / 100;
            for (int j = 0; j < s; j++)
                new_generation.emplace_back(group[m].population[j]);
            s = (90 * POPULATION_SIZE) / 100;
            for (int i = 0; i < s; i++)
            {
                int r = random_num(0, POPULATION_SIZE - 1);
                Individual parent1 = group[m].population[r];
                r = random_num(0, POPULATION_SIZE - 1);
                Individual parent2 = group[m].population[r];
                Individual offspring = parent1.crossover(parent2);
                if ((offspring.fitness <= parent1.fitness) &&
                    (offspring.fitness <= parent2.fitness))
                    new_generation.emplace_back(offspring);
                else if (parent1.fitness < parent2.fitness)
                    new_generation.emplace_back(parent1);
                else
                    new_generation.emplace_back(parent2);
            }
            group[m].population = new_generation;
        }
        if (generation % 200 == 0)
        {
            vector<Individual> temp = group[0].population;
            for (int i = 0; i < num_island - 1; i++)
            {
                group[i].migrate(group[i + 1].population);
            }
            group[num_island - 1].migrate(temp);
        }
        if (generation % 500 == 0)
        {
            cout << "Generation: " << generation << "\n";
            for (int m = 0; m < num_island; m++) 
            {
                cout << m + 1 << "island:";
                for (int i = 0; i < num_queen; i++)
                    cout << group[m].population[0].chromosome[i] << " ";
                cout << "\t";
                cout << "Fitness: " << group[m].population[0].fitness << "\n";
            }
        }
        int c = 0;
        for (int i = 0; i < num_island; i++)
        {
            if (group[i].population[0].fitness == 0)
            {
                c++;
            }
            if (c == num_island)
                found = true;
        }
        generation++;
    }

    for (int m = 0; m < num_island; m++)
    {
        if (group[m].population[0].fitness == 0) 
        {
            cout <<"\n" << m + 1 << "island" << endl;
            for (int i = 0; i < num_queen; ++i)
            {
                for (int j = 0; j < num_queen; ++j)
                {
                    if (j == group[m].population[0].chromosome[i])
                        cout << "*" << " ";
                    else
                        cout << "." << " ";
                }
                cout << endl;
            }
            cout << m + 1 << "island:";
            for (int i = 0; i < num_queen; i++)
                cout << group[m].population[0].chromosome[i] << " ";
            cout << "\t";
            cout << "Fitness: " << group[m].population[0].fitness << "\n";
        }
    }
    end = clock();
    dur = (double)(end - start);
    cout<<"Use Time:"<<(dur / CLOCKS_PER_SEC)<<"s\n";
    system("pause");
}



