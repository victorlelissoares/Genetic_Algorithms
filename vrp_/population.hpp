#include "individuo.hpp"

class Population{
	public:
		Population(); //construtor padrão
		~Population(); //destrutor padrão
		void printPopulation();
		int indexBestScore();

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
