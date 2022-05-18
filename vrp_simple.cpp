#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <cmath>
using namespace std;

// parâmetros do algoritmo genético
int qtd_carros = 2;

int tam_genes = 10; // quantidade de genes

int tam_pop = 5; // quantidade de indivíduos da população

int tam_torneio = 20; // tamanho do torneio

int geracoes = 100; // quantidade de gerações

double prob_mut = 0.2; // probabilidade de mutação

double prob_cruz = 0.7; // probabilidade de cruzamento

double selec_gene = 0.0;




class Individuo{
	friend class Population;

	public:
		Individuo();
		~Individuo();
		void at_score();
		int return_score();
		void print_genes();
		

	//private:
		vector<double> cromossomo;//vetor de genes
		int cord_x;
		int cord_y;
		int score;//score do individuo, vai ser calculado através da distância euclidiana
		void mutation();//mutação
		void crossing(Individuo pai1, Individuo pai2);//operador de cruzamento
};

Individuo::Individuo(){
	for (int i = 0; i < tam_genes; ++i){
		double gene =  rand() % qtd_carros;//gera o alelo(valor do gene)

		this->cromossomo.push_back(gene);//insere o gene no cromossomo
	}

}
void Individuo::print_genes(){

	for(auto i: this->cromossomo)
		cout << i << " ";

	cout << endl;
}

class Population{
	public:
		Population(); //construtor padrão
		~Population(); //destrutor padrão
		void printPopulation();
		int index_best_score();
		inline double dist_euclidiana(Individuo *c1, Individuo *c2);


	//private:
		vector<Individuo> population;

};


inline double Population::dist_euclidiana(Individuo *c1, Individuo *c2){
	// ( (c1x - c2x)^2 + (c1y - c2y)^2 )^(1/2)
	return sqrt( pow( (c1->cord_x - c2->cord_x), 2) + pow( (c1->cord_y - c2->cord_y), 2) );
}


int main(int argc, char const *argv[]){
	
	return 0;
}
