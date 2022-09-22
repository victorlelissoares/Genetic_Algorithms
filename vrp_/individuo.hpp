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

int tam_torneio  = 10; // tamanho do torneio

int geracoes  =  100; // quantidade de gerações

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
		void atScore(int repair);
		void reparationCromossome(vector<thrair<int, double, int>>);//repara o cromossomo reinserindo quem é inviavel
		int returnScore();
		void printGenes();
		void mutation(void (*type_mutation)(Individuo *indi));//mutação
		void crossing(Individuo *pai1, Individuo *pai2, 
		void (*type_crossing)(Individuo*, Individuo*, Individuo*));//operador de cruzamento, recebe a função de cruzamento como parâmetro

		//first correponde ao número do cliente e second a chave aleatória
		vector< pair<int, double> > cromossomo;//vetor de genes
		vector<thrair<int, int, int>> limits_route;//diz onde começa e termina uma rota de um K 
		//e o peso que atualmente aquela rota possui
		double score;//score do individuo, vai ser calculado através da distância euclidiana+penalidade
		double real_score;
		int infeasibility;
		
};

												   //contém o cliente 
void Individuo::reparationCromossome(vector<thrair<int, double, int>> rejectsClients){
	
	//limits_rotes contém respectivamente, onde começa e termina a rota no vetor e o peso que aquela rota tem
	//rejectClients contém o cliente em si(int, double) e de onde ele foi retirado

	//k rotas, cada uma com um intMAX , e o indice do cliente rejeitado
	vector< pair<int, int> >min_insert(qtd_carros, make_pair(-1, INT_FAST32_MAX) );


	// for (auto i:limits_route){

	// 	for (auto k:rejectsClients){
	// 		//verifica se cabe naquela rota, caso caiba, vamos inserir direto
	// 		if( (capacity - i.third) >= demand_vec[k.first-1]){
	// 			cout << "cabe no veículo " << j+1 << ": " << (capacity - i.third) << " e foi colocado"
	// 			<< " " << demand_vec[k.first-1] << endl;

	// 			//vai inserir na rota e ver se está tudo certo

				
	// 		}
	// 	}
	// 	j++;
	// }

	for (int i = 0; i < (int)rejectsClients.size(); i++){

		for (int j = 0; j < (int)limits_route.size(); j++){
			cout << "rejectsClient " << rejectsClients[i].first <<
			" demand_vec " << demand_vec[rejectsClients[i].first-1] <<endl;
			
			printf("\nmin[%d].second = %d\n", i, min_insert[i].second);
			
			printf("\n((demand_vec[%d]+limits_route[j].third) - capacity) =  %d\n",
			rejectsClients[i].first-1,
			((demand_vec[rejectsClients[i].first-1]+limits_route[j].third) - capacity));

			if(min_insert[i].second >= ((demand_vec[rejectsClients[i].first-1]+limits_route[j].third) - capacity) ){
				cout << "entrei" << endl;
				min_insert[i].first = j;//indice do rejectClient
				min_insert[i].second = (demand_vec[rejectsClients[i].first-1]+limits_route[j].third) - capacity;//cálculo
			}

		}
		printf("\nmin_insert[%d].first = %d\n", i, min_insert[i].first);
		printf("rejectsClients[%d].first-1 = %d\n", min_insert[i].first, rejectsClients[min_insert[i].first].first-1);
		printf("demand_vec[%d] = %d\n", rejectsClients[min_insert[i].first].first-1, demand_vec[rejectsClients[min_insert[i].first].first-1]);
		limits_route[i].third += demand_vec[rejectsClients[min_insert[i].first].first-1];
		cout << limits_route[i].first << " " << limits_route[i].second << " " << limits_route[i].third << endl;
	}
	
	
	
}

void Individuo::atScore(int repair){
	vector<thrair<int, double, int>> rejectClients;
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
		
		if(peso + demand_vec[indi_2] > capacity and repair){
			//cliente e indice de onde foi removido do vetor
			rejectClients.push_back({this->cromossomo[i].first, this->cromossomo[i].second, i});

		}
		else{
			score_fit +=  distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);	
			peso += demand_vec[indi_2];
		}
		//score_fit +=  distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);
		
		indi_1 = indi_2;
		//peso += demand_vec[indi_2];
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
			limits_route.push_back({init_route, final_route, peso});
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
		if(i >= (int)this->cromossomo.size()){//fim da linha
			//cout << "cabo porra" << endl;
			// quer dizer que terminaram as entregas
			// então o caminhão atual deve retornar ao depósito
			final_route = i-1;
			//cout << "ini: " << init_route << " fi: " << final_route << endl;
			limits_route.push_back({init_route, final_route, peso});

			score_fit += distEuclidiana(distance_vec[indi_1], distance_vec[0]);
			if(peso > capacity){//significa que a capacidade do caminhão foi ultrapassada
				peso_penality += (peso - capacity);
				this->infeasibility += 1;
			}

		}

	}	

	this->real_score = score_fit;//distância euclidiana efetivamente percorrida
	this->score = score_fit + (pow(peso_penality, 2.)) * infeasibility;
	
	if (repair){
			for (auto k:limits_route){
			cout << k.first << " " << k.second << " " << k.third << endl;
		}

		cout << "Removidos : ";
		for (auto i:rejectClients){
			vector<pair< int, double>>::iterator it = cromossomo.begin() + i.third;//calcula posição do iterador
			this->cromossomo.erase(it);
			cout << i.first << " " << i.second << " " << i.third << endl;
		}
		cout << endl;
		this->reparationCromossome(rejectClients);
	}
	
	
	
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
	
	//if(prob_repair > prob){//então tem que reparar o cromossomo
		this->atScore(0); // atualiza o score do individuo

	// }
	// else{
	// 	this->atScore(0); // atualiza o score do individuo
	// }
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
