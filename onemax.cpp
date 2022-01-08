#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
using namespace std;

// parâmetros do algoritmo genético
int tam_genes = 10; // quantidade de genes
int tam_pop = 5; // quantidade de indivíduos da população
int tam_torneio = 20; // tamanho do torneio
int geracoes = 100; // quantidade de gerações
double prob_mut = 0.2; // probabilidade de mutação
double prob_cruz = 0.7; // probabilidade de cruzamento


class Individuo{
	friend class Population;

	public:
		Individuo();
		~Individuo();
		int return_score();
		void print_genes();

	//private:
		vector<int> genes;//vetor de genes
		int score;//score do individuo
		void mutation();//mutação
		void crossing(Individuo pai1, Individuo pai2);//operador de cruzamento
};

//construtor
Individuo::Individuo(){
	this->score = 0;

	for (int i = 0; i < tam_genes; ++i){
		
		int num = rand() % 2; // gera um numero entre [0,1]
		
		this->genes.push_back(num);//insere o gene 
		
		if(num == 1)
			this->score += 1;
	}
}

//destrutor
Individuo::~Individuo(){
	;
}

void Individuo::crossing(Individuo pai1, Individuo pai2){
	int ponto = rand() % tam_genes;


	for (int i = 0; i < ponto + 1; ++i){
		this->genes[i] = pai1.genes[i];
	}

	for (int i = ponto + 1; i < tam_genes; ++i){
		this->genes[i] = pai2.genes[i];	
	}
}


//modifica um gene, no intervalo de [0, tam_genes - 1]
void Individuo::mutation(){
	int gene = rand() % tam_genes;

	if(this->genes[gene] == 0)
		this->genes[gene] = 1;
	else
		this->genes[gene] = 0;
}

int Individuo::return_score(){
	return score;
}

void Individuo::print_genes(){

	for (int i = 0; i < tam_genes; ++i){
		cout << this->genes[i] << " ";	
	}
	cout << "Score: " << this->score;
	cout << endl;
}

class Population{
	public:
		Population();
		~Population();
		void printPopulation();

	//private:
		vector<Individuo> population;

};

Population::Population(){
	for (int i = 0; i < tam_pop; ++i){
		Individuo pop;//gerou individuo
		this->population.push_back(pop);//insere individuo na população
	}
}
Population::~Population(){
	;
}

void Population::printPopulation(){
	for (int i = 0; i < tam_pop; ++i){
		cout << "Individuo " << i + 1 << " :\n";
		this->population[i].print_genes();//mostra genes daquele individuo
		cout << endl;
	}
}




int main(int argc, char const *argv[]){
	srand(time(NULL));
	Population especimes;
	especimes.printPopulation();
	return 0;
}