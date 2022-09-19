// Contém as variáveis globais e a classe individuo, com seus respectivos métodos
// e funções auxiliares
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <random>
#include <fstream>
#include <string.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include "utils.hpp"

using namespace std;

// Hiper parâmetros do algoritmo genético
int qtd_carros    = 0;

int tam_genes     = 0; // quantidade de genes

int tam_pop      = 100; // quantidade de indivíduos da população

int tam_torneio  = 100; // tamanho do torneio

int geracoes  =  2500; // quantidade de gerações

double prob_mut = 0.8; // probabilidade de mutação

double prob_cruz= 0.6; // probabilidade de cruzamento

double prob_repair = 0.1;//como a reparação de cromosso é um processo muito custoso, a probabilidade de acontecer
//será baixa para garantir o bom desempenho do programa

int selec_gene   = 60;

int capacity      = 0;

int depot         = 0;

//Contém as coordenadas x e y de cada cliente
vector< pair <int, int> > distance_vec;

//contém a demanda associada a cada cliente
vector<int> demand_vec;

class Individuo{
	public:
		Individuo();
		~Individuo();
		void atScore();
		int returnScore();
		void printGenes();
		void mutation(void (*type_mutation)(Individuo *indi));//mutação
		void crossing(Individuo *pai1, Individuo *pai2, 
		void (*type_crossing)(Individuo*, Individuo*, Individuo*));//operador de cruzamento, recebe a função de cruzamento como parâmetro

		//first correponde ao número do cliente e second a chave aleatória
		vector< pair<int, double> > cromossomo;//vetor de genes
		vector<pair<int, int>> limits_route;//diz onde começa e termina uma rota de um K dentro do cromossomo
		double score;//score do individuo, vai ser calculado através da distância euclidiana+penalidade
		double real_score;
		int infeasibility;
		
};

void Individuo::atScore(){
	infeasibility = 0;
	int peso_penality   = 0;
	//vector<pair<int, double>> v = this->cromossomo;
	// Como a ordem de visitas de um veículo é dado pelo seu double
	// ordeno o vetor para saber qual a ordem de visitas aos clientes
	sort(this->cromossomo.begin(), this->cromossomo.end(),
     [](const pair<int, double>& lhs, const pair<int, double>& rhs) {
             return lhs.second < rhs.second; } );
	
	
	double score_fit = 0;
	int i = 0;
	int indi_1 = 0;
	int indi_2 = this->cromossomo[i].first-1;
	int actual_car = 1;//considerando que existe mais de um this.cromossomoeículo
	int peso = 0;
	
	//cout << "this.cromossomoeículo " << (int) this.cromossomo[i].second << " saindo do depósito" << endl;
	int init_route = i, final_route;
	
	while ( i < tam_genes){
		//cout << "i: "<<  i << endl;
		score_fit +=  distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);
		
		indi_1 = indi_2;
		peso += demand_vec[indi_1];
		
		i++;
		indi_2 = this->cromossomo[i].first-1;
		//cout << "indi2: " << indi_2;
		//significa que trocou de carro
		//ou seja, outra rota
		//então deve-se retornar ao depósito
		if(actual_car < (int) this->cromossomo[i].second){
			//cout << "troca carro" << endl;
			actual_car = (int) this->cromossomo[i].second;
			
			score_fit +=  distEuclidiana(distance_vec[indi_1], distance_vec[0]);//retorna ao depósito
			
			final_route = i - 1;
			//cout << "ini: " << init_route << " fi: " << final_route << endl;
			limits_route.push_back(make_pair(init_route, final_route));
			init_route = i;
			
			//após retornar o depósito, checamos se aquela rota
			// não excedeu o quanto o caminhão pode levar
			// caso sim, multiplicamos o quanto foi excedido
			// para "penalizar" a solução
			if(peso > capacity){//significa que a capacidade do caminhão foi ultrapassada
				peso_penality += (peso - capacity);
				this->infeasibility += 1;
			}

			// após isso, significa que o próximo cliente, será visitado por outro caminhão
			// então, o mesmo deve sair do depósito

			score_fit += distEuclidiana(distance_vec[0], distance_vec[indi_2]);
			
			peso = 0;
			peso += demand_vec[indi_2];
			indi_1 = indi_2;
			//cout << "i: "<<  i << endl;
			i++;
			//cout << "i: "<<  i << endl;
			indi_2 = this->cromossomo[i].first-1;
		}
		//cout << "i: "<<  i << endl;
		if(i >= tam_genes){//fim da linha
			//cout << "cabo porra" << endl;
			// quer dizer que terminaram as entregas
			// então o caminhão atual deve retornar ao depósito
			final_route = i-1;
			//cout << "ini: " << init_route << " fi: " << final_route << endl;
			limits_route.push_back(make_pair(init_route, final_route));

			score_fit += distEuclidiana(distance_vec[indi_1], distance_vec[0]);
			if(peso > capacity){//significa que a capacidade do caminhão foi ultrapassada
				peso_penality += (peso - capacity);
				this->infeasibility += 1;
			}

		}

	}	

	this->real_score = score_fit;//distância euclidiana efetivamente percorrida
	this->score = score_fit + (pow(peso_penality, 2.)) * infeasibility;
	
}
/*Penalizar inviabilidade
O método de cálculo da adequação da solução foi o primeiro procedimento testado para observar 
o efeito que isso teria nos resultados. 
O primeiro método de penalização tentado foi penalizar cada solução inviável, 
atribuindo seu custo a um grande valor.
*/

Individuo::Individuo(): score(0), real_score(0), infeasibility(0){
	int j = 2;
	for (int i = 0; i < tam_genes; ++i){
		double gene =  fRand(1, qtd_carros+1);//gera o alelo(valor do gene)

		this->cromossomo.push_back(make_pair(j++, gene));//insere o gene no cromossomo
	}
	
	double prob = fRand(0, 1);
	
	// if(prob_repair > prob){//então tem que reparar o cromossomo
	// 	this->atScore(1); // atualiza o score do individuo
	// }
	// else{
	// 	this->atScore(0); // atualiza o score do individuo
	// }
	atScore();
}

Individuo::~Individuo(){

}

// boas chances de combinar boas características, independente de onde estejam no cromossomo
void uniformCrossing(Individuo *pai1, Individuo *pai2, Individuo *filho){
	int param;
	srand(time(0));
	for (int i = 0; i < tam_genes; i++){	
		param = (int) fRand(1, 100);
		if(param > selec_gene){//recebe pai1
			filho->cromossomo[i] = pai1->cromossomo[i];
		}
		else{//recebe pai 2
			filho->cromossomo[i] = pai2->cromossomo[i];
		}

	}		
}


// função que correponde a um tipo de cruzamento um ponto, dois pontos, uniforme, etc...
void Individuo::crossing(Individuo *pai1, Individuo *pai2, 
						void (*type_crossing)(Individuo *, Individuo *, Individuo*)){
	type_crossing(pai1, pai2, this);
}

// Muta todo o cromossomo, proporcionando maior diversidade na população
void imigracaoMutation(Individuo *indi){
	for (int i = 0; i < tam_genes; ++i){
		double gene =  fRand(1, qtd_carros+1);//gera o alelo(valor do gene)
		indi->cromossomo.at(i) = pair<int, double>(indi->cromossomo[i].first, gene);
	}

}

void Individuo::mutation(void(*type_mutation)(Individuo *indi)){
	type_mutation(this);
}

void Individuo::printGenes(){
	for(auto i: this->cromossomo){
		cout << "Cliente: " << i.first << " " << i.second << " ";
		//cout << i.second << " ";
	}
	cout << "; Distance: " << this->real_score;
	cout << "; Fit: " << this->score;
	cout << " ; Inviavel? " << this->infeasibility;
	cout << endl;
}
