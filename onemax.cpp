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
		void at_score();
		int return_score();
		void print_genes();
		int operator<(Individuo &other);
		int operator>(Individuo &other);

	//private:
		vector<int> genes;//vetor de genes
		double score;//score do individuo
		void mutation();//mutação
		void crossing(Individuo pai1, Individuo pai2);//operador de cruzamento
};

void Individuo::at_score(){
	this->score = 0;
	for (int i = 0; i < tam_genes; ++i){
		
		if(this->genes[i] == 1)
			score += 1;
	}
}

int Individuo::operator<(Individuo &other){
	return this->score < other.score;
}

int Individuo::operator>(Individuo &other){
	return this->score > other.score;
}

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
		Population(); //construtor padrão
		~Population(); //destrutor padrão
		void printPopulation();
		int index_best_score();


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

int Population::index_best_score(){
	int best_score = 0;

	for (int i = 0; i < tam_pop; ++i){
		//if( (this->population[best_score]).score < (this->population[i]).score )
		if(this->population[best_score] < this->population[i])
			best_score = i;
	}

	return best_score;
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
	Population especimes;//constroi população
	especimes.printPopulation();

	//cout << especimes.index_best_score() + 1;

	for (int i = 0; i < geracoes; ++i){

		for(int j = 0; j < tam_torneio; j++){
			
			double prob = ((double) rand() / ((double)RAND_MAX + 1) );
			
			if(prob < prob_cruz){
				int indice_pai1 = rand() % tam_pop;
				
				int indice_pai2 = rand() % tam_pop;
				
				//garante que os indices são diferentes
				while(indice_pai2 == indice_pai1){
					indice_pai2 = rand() % tam_pop;
				}
				Individuo filho;

				//filho.print_genes();
				filho.crossing(especimes.population[indice_pai1], especimes.population[indice_pai2]);
				filho.at_score();//atualiza o score
				//filho.print_genes();

				double prob = ((double) rand() / ((double)RAND_MAX + 1) );

				if(prob < prob_mut){
					filho.mutation();
					filho.at_score();//atualiza o score
				}

				if( especimes.population[indice_pai1] < filho){
					//copia genes do filho para o pai
					for (int k = 0; k < tam_genes; ++k){
						especimes.population[indice_pai1].genes[k] = filho.genes[k];
					}
					especimes.population[indice_pai1].at_score();//atualiza o score

				}
			}
		}
		cout << "Geracao " << i+1 << endl;

		int index = especimes.index_best_score();
		cout << "Genes: ";
		especimes.population[index].print_genes();
		cout << endl;

		if(especimes.population[index].score == tam_genes)
			break;	

	}


	return 0;
}
