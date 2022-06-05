#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <random>
#include <fstream>
#include <string.h>
#include<cstdlib>
#include<ctime>
#include<algorithm>

using namespace std;

// parâmetros do algoritmo genético
int qtd_carros = 2;

int tam_genes = 0; // quantidade de genes

int tam_pop = 10; // quantidade de indivíduos da população

int tam_torneio = 20; // tamanho do torneio

int geracoes = 1000; // quantidade de gerações

double prob_mut = 0.2; // probabilidade de mutação

double prob_cruz = 0.7; // probabilidade de cruzamento

int selec_gene = 45;

int capacity = 0;

int depot = 0;

vector< pair <int, int> > distance_vec;

vector<int> demand_vec;

class Individuo{
	public:
		Individuo();
		~Individuo();
		void atScore();
		int returnScore();
		void printGenes();
        //first correponde ao número do cliente e second a chave aleatória
		vector< pair<int, double> > cromossomo;//vetor de genes
		int score;//score do individuo, vai ser calculado através da distância euclidiana
		int infeasibility;
		void mutation(void (*type_mutation)(Individuo *));//mutação
		void crossing(Individuo *, Individuo *, 
		void (*type_crossing)(Individuo*, Individuo*, Individuo*));//operador de cruzamento, recebe a função de cruzamento como parâmetro
};
