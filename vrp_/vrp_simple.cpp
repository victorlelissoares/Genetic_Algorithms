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

int tam_pop = 5; // quantidade de indivíduos da população

int tam_torneio = 20; // tamanho do torneio

int geracoes = 100; // quantidade de gerações

double prob_mut = 0.2; // probabilidade de mutação

double prob_cruz = 0.7; // probabilidade de cruzamento

int selec_gene = 45;

int capacity = 0;

int depot = 0;

vector< pair <int, int> > distance_vec;

vector<int> demand_vec;

//função para gerar números(double) aleatórios 
double fRand(double fMin, double fMax){
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

inline double distEuclidiana(pair<int, int> cl1, pair<int, int>cl2){	
	//( (c1x - c2x)^2 + (c1y - c2y)^2 )^(1/2)
 	return sqrt( pow( (cl1.first - cl2.first), 2) + pow( (cl1.second - cl2.second), 2) );
}

class Individuo{
	public:
		Individuo();
		~Individuo();
		void atScore();
		int returnScore();
		void printGenes();
		

	//private:
		vector< pair<int, double> > cromossomo;//vetor de genes
		int score;//score do individuo, vai ser calculado através da distância euclidiana
		void mutation(void (*type_mutation)(Individuo *indi));//mutação
		void crossing(Individuo *pai1, Individuo *pai2, 
		void (*type_crossing)(Individuo*, Individuo*, Individuo*));//operador de cruzamento, recebe a função de cruzamento como parâmetro
};

void Individuo::atScore(){
	cout << endl << "Função Atualiza Score" << endl;
	vector<pair<int, double>> v = this->cromossomo;
	// Como a ordem de visitas de um veículo é dado pelo seu double
	// ordeno o vetor para saber qual a ordem de visitas aos clientes
	sort(v.begin(), v.end(),
     [](const pair<int, double>& lhs, const pair<int, double>& rhs) {
             return lhs.second < rhs.second; } );
	
	for(auto i: v)
		cout << "Cliente: " << i.first << " " << i.second << " ";

	cout << endl;

	// cout << "cord_x: " << distance_vec[depot].first << " cord_y: " << distance_vec[depot].second
	// 	<< endl;

	// for(auto i: v)
	// cout << "cord_x: " << distance_vec[i.first - 2].first << " cord_y: " << distance_vec[i.first-2].second
	// 	<< endl;
	// cout << "cord_x: " << distance_vec[v[0].first-1].first << " cord_y: " << distance_vec[v[0].first-1].second;
	// cout << endl << "distancia: " << distEuclidiana(distance_vec[depot], distance_vec[v[0].first-1]) << endl;
	// cout << "Fim Função Atualiza Score" << endl << endl;

	double score_fit = 0;
	int i = 0;
	int indi_1 = 0;
	int indi_2 = v[i].first-1;
	int actual_car = 1;
	int peso = 0;
	
	cout << "veículo " << (int) v[i].second << " saindo do depósito" << endl;

	while ( i < tam_genes){
		cout << indi_1+1 << " e " << indi_2+1 << "; ";
		score_fit +=  distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);
		cout << distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]) << endl;;
		
		indi_1 = indi_2;
		peso+= demand_vec[indi_1];
		cout << "Peso a ser adicionado: " << demand_vec[indi_1] <<
		"; Peso até então: " << peso << endl;
		i++;
		indi_2 = v[i].first-1;
		// cout << "atual: " << indi_1+1 << endl;
		//cout << "prox: " << indi_2+1 << endl;

		//significa que trocou de carro
		//ou seja, outra rota
		//então deve-se retornar ao depósito
		if(actual_car < (int) v[i].second){
			//peso-=demand_vec[indi_1];
			actual_car = (int) v[i].second;
			cout << "retornando ao depósito" << endl;
			cout << indi_1+1 << " e " << "1" << "; ";
			cout << distEuclidiana(distance_vec[indi_1], distance_vec[0]) << endl;
			score_fit +=  distEuclidiana(distance_vec[indi_1], distance_vec[0]);//retorna ao depósito
			cout << "Peso total usado: " << peso << endl;
			cout << "Total Percorrido: " << score_fit << endl;
			
			//após retornar o depósito, checamos se aquela rota
			// não excedeu o quanto o caminhão pode levar
			// caso sim, multiplicamos o quanto foi excedido
			// para "penalizar" a solução
			if(peso - capacity > 0)//significa que a capacidade do caminhão foi ultrapassada
				score_fit *= (peso - capacity);
			
			// cout << score_fit << endl;
			cout << "Total Percorrido com penalização: " << score_fit << endl;
			// após isso, significa que o próximo cliente, será visitado por outro caminhão
			// então, o mesmo deve sair do depósito
			cout << "Veículo " <<(int)v[i].second << " saindo do depósito" << endl;
			cout << "1" << " e " << indi_2+1 << "; ";
			cout << distEuclidiana(distance_vec[0], distance_vec[indi_2]) << endl;
			score_fit += distEuclidiana(distance_vec[0], distance_vec[indi_2]);//adiciona a distância de fato
			
			peso = 0;
			peso += demand_vec[indi_2];
			indi_1 = indi_2;
			i++;
			indi_2 = v[i].first-1;
		}
		if(indi_2+1 == 0){//fim da linha
			cout << "Voltando ao depósito" << endl;
			cout << "Peso : " << peso << endl;
			// quer dizer que terminaram as entregas
			// então o caminhão atual deve retornar ao depósito
			cout << indi_1+1 << " e " << "1" << "; " ;
			cout << distEuclidiana(distance_vec[indi_1], distance_vec[0]) << endl;
			score_fit+=distEuclidiana(distance_vec[indi_1], distance_vec[0]);
			if(peso - capacity > 0)//significa que a capacidade do caminhão foi ultrapassada
				score_fit *= (peso - capacity);
			// cout << score_fit << endl;
			cout << "Total Percorrido com penalização: " << score_fit << endl;

		}

	}
	
	this->score = score_fit;
	cout << "Total Percorrido: " << score_fit << endl;
	cout << "Finaliza Função Score" << endl << endl;
}

Individuo::Individuo(){
	int j = 2;
	for (int i = 0; i < tam_genes; ++i){
		double gene =  fRand(1, qtd_carros+1);//gera o alelo(valor do gene)

		this->cromossomo.push_back(make_pair(j++, gene));//insere o gene no cromossomo
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
		//auto pos = indi->cromossomo.begin()+i;
		//indi->cromossomo.emplace(pos, pair<int, double>(indi->cromossomo[i].first, gene));//altera o valor do gene
		indi->cromossomo.at(i) = pair<int, double>(indi->cromossomo[i].first, gene);
	}

}

void Individuo::mutation(void(*type_mutation)(Individuo *indi)){
	type_mutation(this);
}

void Individuo::printGenes(){
	for(auto i: this->cromossomo)
		cout << "Cliente: " << i.first << " " << i.second << " ";

	cout << endl;
}

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

void read_file(){
	ifstream myfile;
	string myline;
	char *line;
	int i = 0;

	myfile.open("toy.vrp");

	//lê a linha que especifica o número de clientes
	getline(myfile, myline, '\n');//read dimension line
	line = new char[myline.length()+1];
	strcpy(line, myline.c_str());//transforma a linha lida em um char *
	char *tok = strtok(line, " \n");
	//cout << tok << endl;
	tok = strtok(NULL, " \n");
	//cout << tok << endl;
	tok = strtok(NULL, " \n");
	//cout << tok << endl;
	string str;
	str.assign(tok);
	tam_genes = stoi(tok)-1;//considerando um único depósito
	//cout << tam_genes << endl;
	// int x = stoi(str);//coordenada x
	
	//lê a linha que especifica a capacidade dos carros
	getline(myfile, myline, '\n');//read dimension line
	line = new char[myline.length()+1];
	strcpy(line, myline.c_str());//transforma a linha lida em um char *
	tok = strtok(line, " \n");
	//cout << tok << endl;
	tok = strtok(NULL, " \n");
	//cout << tok << endl;
	tok = strtok(NULL, " \n");
	//cout << tok << endl;
	str.assign(tok);
	capacity = stoi(tok);
	//cout << capacity << endl;

	//lê seção de coordenadas
	getline(myfile, myline, '\n');//ignore node_coord section
	while(1){
		getline(myfile, myline, '\n');//pega a linha contendo as coordenadas
		line = new char[myline.length()+1];
		strcpy(line, myline.c_str());
		if(myline.find("DEMAND_SECTION", 0) != string::npos){	
			break;
		}
		
		char *tok = strtok(line, " \n");
		//cout << tok << endl;
		tok = strtok(NULL, " \n");
		string str;
		str.assign(tok);
		//cout << str << endl;
		int x = stoi(str);//coordenada x
		//cout << x << endl;

		tok = strtok(NULL, " \n");
		str.assign(tok);//coordenada y

		distance_vec.push_back(make_pair(x, stoi(str)));//insere as cord. no vetor de distância

		cout << "cord_x: " << distance_vec[i].first << " cord_y: " << distance_vec[i].second
		<< endl;
		
		i++;
	}
	i = 0;
	while (1){
		getline(myfile, myline, '\n');//lê a 1 linha de demmand section
		strcpy(line, myline.c_str());
		// cout << "antes";
		if(myline.find("DEPOT_SECTION", 0) != string::npos){	
			// cout << "dentro" << endl;
			break;
		}
		// cout << "depois";
		char *tok = strtok(line, " \n");
		int pos = stoi(tok);
		cout << "client: " << pos << " ";
		tok = strtok(NULL, " \n");
		cout << " demand: " << stoi(tok) << endl;
		demand_vec.push_back(stoi(tok));
	}

	//depot section
	getline(myfile, myline, '\n');//lê deposito
	depot = stoi(myline) - 1;
	getline(myfile, myline, '\n');//lê fim dos depósitos
	getline(myfile, myline, '\n');//lê o fim de um arquivo
}


int main(int argc, char const *argv[]){
	read_file();
	srand(time(0));
	Population pop;
	Individuo teste;
	
	
	// teste de cruzamento
	// teste = pop.population[2];
	// teste.printGenes();
	// teste.mutation(&imigracaoMutation);
	teste.printGenes();
	teste.atScore();

	//teste de reprodução
	// Individuo filho;
	// filho.crossing(&pop.population[0], &pop.population[1], &uniformCrossing);
	// filho.printGenes();

	return 0;
}
