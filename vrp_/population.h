#include "individuo.h"

class Population{
	public:
		Population(); //construtor padrão
		~Population(); //destrutor padrão
		void printPopulation();
		int indexBestScore();
        vector<Individuo> population;
        
	//private:
		

};