#include <iostream>
#include <conio.h>
#include <random>
#include <time.h>
#include <windows.h>
#include <vector>

using namespace std;

#define min(a, b) a < b ? a : b
#define MUTATION_START_RATE 2

string PHRASE = "very hard message to find";

char random_char() {
    int r = rand() % 27 + 97;
    if(r == 123) return ' '; 
    return char(r);
}

float fitness(string str) {
    float correct_letters = 0;
    for(int i = 0; i < PHRASE.length(); i++)
        if(PHRASE[i] == str[i]) correct_letters++;
    return correct_letters / PHRASE.length();
}

/*
NATURAL SELECTION: the fittest survive, genes get passed down, a small chance of mutation
*/

string breed(string a, string b) {
    string res;
    for(int i = 0; i < a.length(); i++) {
        res += (rand() % 2) == 1 ? a[i] : b[i];
    }
    return res;
}

vector<string> matingPool(vector<string> pop) {
    vector<string> fp;
    for(string i: pop) 
        for(int j = 0; j < 100*fitness(i); j++)
            fp.push_back(i);
    return fp;
}

string best_of_gen(vector<string> gen) {
    string best = gen[0];
    for(int i = 0; i < gen.size(); i++)
        if(fitness(gen[i]) > fitness(best))
            best = gen[i];
    return best;
}

vector<string> new_generation(vector<string> pop, string &tags) {
    vector<string> ret;
    vector<string> mating_pool = matingPool(pop);

    ret.push_back(best_of_gen(pop));

    for(int i = 0; i < (pop.size()-1); i++) {
        string parent_a = mating_pool[rand() % mating_pool.size()];
        string parent_b = mating_pool[rand() % mating_pool.size()];
        ret.push_back(breed(parent_a, parent_b));
    }

    return ret;
}

void mutate(vector<string> &pop, int rate) {
    for(int i = 0; i < pop.size(); i++)
        if(rand() % 100 < rate) {
            pop[i][rand() % PHRASE.length()] = random_char();
        }
}


int main() {
    srand(time(NULL));

    unsigned int population = 40;

    printf("Guessing the phrase with generaitons and just percentage of solved!\n--------------");
    cout<<"\nWrite a phrase, and the machine will find it! > ";
    getline(cin, PHRASE);
    if(population % 2 != 0) population += 1;

    printf("First population:\n");

    vector<string> pop(population);
    for(int i = 0; i < population; i++) {
        for(int j = 0; j < PHRASE.length(); j++)
            pop[i] += random_char();
        cout<<"\""<<pop[i]<<"\"\n";
    }

    printf("\nPress a key to continue...");
    getch();

    printf("\nNow... breeding untill achieving final word!\nStarting in: ");
    for(int i = 3; i >= 1; i--) {
        printf("%d... ", i);
        Sleep(1000);
    }

    float g = 0.f;
    int it = 1, mutation_rate = MUTATION_START_RATE;
    string best = best_of_gen(pop);
    string lbest = pop[0];
    printf("\n");
    printf("\n%d: %s (best guess for the generation)", it, best.c_str());
    string tags = "";

    while(g < 1) {
        it++;
        pop = new_generation(pop, tags);
        if(lbest >= best_of_gen(pop)) mutation_rate = min(mutation_rate + int(MUTATION_START_RATE)/2, 30);
        else mutation_rate = MUTATION_START_RATE;
        
        mutate(pop, mutation_rate);
        best = best_of_gen(pop), g = fitness(best);
        printf("\ngen %d: %s (%f), %s", it, best.c_str(), g, to_string(mutation_rate).c_str());
        lbest = best;
    }

    printf("\nGUESSED THE CODE WITH JUST %d GENERATIONS!!", it);


    getch();
    return 0;
}