#include "population.h"

Population::Population(){
	for (int i = 0; i < tam_pop; i++){
		Individuo indi;
		this->population.push_back(indi);
	}
	
}

Population::~Population(){
	;
}

void Population::printPopulation(){
	cout << "População" << endl;
	for (int i = 0; i < tam_pop; i++){
		cout << i+1 << ". ";
		this->population[i].printGenes();
	}
	cout << "Fim População" << endl;
}