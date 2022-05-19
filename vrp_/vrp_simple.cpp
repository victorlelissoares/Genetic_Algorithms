#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <random>

using namespace std;

// parâmetros do algoritmo genético
int qtd_carros = 2;

int tam_genes = 10; // quantidade de genes

int tam_pop = 5; // quantidade de indivíduos da população

int tam_torneio = 20; // tamanho do torneio

int geracoes = 100; // quantidade de gerações

double prob_mut = 0.2; // probabilidade de mutação

double prob_cruz = 0.7; // probabilidade de cruzamento

int selec_gene = 45;

//função para gerar números(double) aleatórios 
double fRand(double fMin, double fMax){
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}



class Individuo{
	
	public:
		Individuo();
		~Individuo();
		void atScore();
		int returnScore();
		void printGenes();
		

	//private:
		vector<double> cromossomo;//vetor de genes
		int cord_x;
		int cord_y;
		int score;//score do individuo, vai ser calculado através da distância euclidiana
		void mutation(void (*type_mutation)(Individuo *indi));//mutação
		void crossing(Individuo *pai1, Individuo *pai2, 
		void (*type_crossing)(Individuo*, Individuo*, Individuo*));//operador de cruzamento, recebe a função de cruzamento como parâmetro
};

Individuo::Individuo(){
	for (int i = 0; i < tam_genes; ++i){
		double gene =  fRand(1, qtd_carros+1);//gera o alelo(valor do gene)

		this->cromossomo.push_back(gene);//insere o gene no cromossomo
	}

}

Individuo::~Individuo(){

}


// boas chances de combinar boas características, independente de onde estejam no cromossomo
void uniformCrossing(Individuo *pai1, Individuo *pai2, Individuo *filho){
	int param;
	srand(time(0));
	for (int i = 0; i < tam_genes; i++){	
		param = (int) fRand(1, 100);//gerado a cada iteração
		//cout << "Prob: " << param << endl;
		if(param > selec_gene){//recebe pai1
			//cout << "pai 1" << endl;
			filho->cromossomo[i] = pai1->cromossomo[i];
		}
		else{//recebe pai 2
			//cout << "pai 2" << endl;
			filho->cromossomo[i] = pai2->cromossomo[i];
		}

	}
		
}

void Individuo::crossing(Individuo *pai1, Individuo *pai2, 
						void (*type_crossing)(Individuo *, Individuo *, Individuo*)){
	type_crossing(pai1, pai2, this);
	// função que correponde a um tipo de cruzamento
	// um ponto, dois pontos, uniforme, etc...

}

// Muta todo o cromossomo, proporcionando maior diversidade na população
void imigracaoMutation(Individuo *indi){
	for (int i = 0; i < tam_genes; ++i){
		double gene =  fRand(1, qtd_carros+1);//gera o alelo(valor do gene)

		indi->cromossomo[i] = gene;//altera o valor do gene
	}

}

void Individuo::mutation(void(*type_mutation)(Individuo *indi)){
	type_mutation(this);
}

void Individuo::printGenes(){
	for(auto i: this->cromossomo)
		cout << i << " ";

	cout << endl;
}

class Population{
	public:
		Population(); //construtor padrão
		~Population(); //destrutor padrão
		void printPopulation();
		int indexBestScore();
		inline double distEuclidiana(Individuo *c1, Individuo *c2);

	//private:
		vector<Individuo> population;


};

Population::Population(){
	for (int i = 0; i < tam_pop; i++){
		Individuo indi;

		this->population.push_back(indi);
	}
	
}

void Population::printPopulation(){
	cout << "População" << endl;
	for (int i = 0; i < tam_pop; i++){
		cout << i+1 << ". ";
		this->population[i].printGenes();
	}
	cout << "Fim População" << endl;
}

Population::~Population(){
	;
}


inline double Population::distEuclidiana(Individuo *c1, Individuo *c2){
	// ( (c1x - c2x)^2 + (c1y - c2y)^2 )^(1/2)
	return sqrt( pow( (c1->cord_x - c2->cord_x), 2) + pow( (c1->cord_y - c2->cord_y), 2) );
}


int main(int argc, char const *argv[]){
	srand(time(0));
	Population pop;
	Individuo teste;

	pop.printPopulation();
	
	//teste de cruzamento
	// teste = pop.population[2];
	// teste.printGenes();
	// teste.mutation(&imigracaoMutation);
	// teste.printGenes();

	//teste de reprodução
	// Individuo filho;
	// filho.crossing(&pop.population[0], &pop.population[1], &uniformCrossing);
	// filho.printGenes();



	return 0;
}
