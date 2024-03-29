// Contem a classe population e seus respectivos métodos e funções auxiliares
#include "individuo.hpp"

class Population{
	public:
		Population(); //construtor padrão
		~Population();//destrutor padrão
		void printPopulation();
		//int indexBestScore();
		Individuo best;

	//private:
		vector<Individuo> population;

};

Population::Population(){
	this->best.score = INT32_MAX;

	for (int i = 0; i < tam_pop; i++){
		Individuo indi;
		indi.atScore(0);
		this->population.push_back(indi);

		if(indi.score <= best.score and indi.infeasibility == 0){
			best = indi;
			//cout << best.real_score << endl;
			//best.printGenes();
		}
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
