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
	//auto cpRoutes = limits_route;
	//limits_rotes contém respectivamente, onde começa e termina a rota no vetor e o peso que aquela rota tem
	//rejectClients contém o cliente em si(int, double) e de onde ele foi retirado
	//apenas registra qual cliente já foi alocado
	//vector<int> allocatedClients(tam_genes, 0);
	
	// for (auto i:rejectsClients){
	// 	cout << i.first << " " << i.second << " " << i.third << endl;
	// }

	//testando somente com a primeira rota
	//cada posição de min corresponde a uma rota/veículo
	//então min[0] == k_0
	vector< pair<int, float> > min_(rejectsClients.size(), make_pair(0, MAXFLOAT));
	
	// for (int j = 0; j < (int)limits_route.size(); j++){
	
	// 	for (int i = 0; i < (int)rejectsClients.size(); i++){
	// 		auto indi = rejectsClients[i].first-1;//indice do cliente no vetor de demanda
			
	// 		//contem o peso total da rota, supondo a adição do atual cliente rejeitado
	// 		auto pesoTotal = demand_vec[indi] + limits_route[j].third;
			
	// 		//tem que ser o menor e não pode ter sido adicionado a um veículo
	// 		if(pesoTotal - capacity < min_[j].second and allocatedClients[i] == 0){
	// 			min_[j] = make_pair(i,pesoTotal - capacity);
	// 		}

	// 	}
	// 						//imenor
	// 	allocatedClients[min_[j].first] = 1;
	// 	//o rejectedClient[min_[j].first] já foi alocado para a melhor rota

	// }

	for (int i = 0; i < (int)rejectsClients.size(); i++){
		
		for (int j = 0; j < (int)limits_route.size(); j++){
			auto indi = rejectsClients[i].first-1;

			auto pesoTotal = demand_vec[indi] + limits_route[j].third;

			if(pesoTotal-capacity < min_[i].second){
				//o cliente i tem que ser alocado na rota j
				min_[i] = make_pair(j, pesoTotal-capacity);
			}
		}
						//j									//indi
		limits_route[min_[i].first].third += demand_vec[rejectsClients[i].first-1];
	}
	

	cout << "Rota em que deve ser inserido e diferença de peso" << endl;
	for (auto i:min_){
		cout << i.first << " " << i.second << endl;
	}
	cout << "Rotas, seus limites e pesos" << endl;
	for (auto i:limits_route){
		cout << i.first << " " << i.second << " " << i.third << endl;
	}
	
	int i = 0;
	double score_fit = this->real_score;
	//melhorar essa parte para que deixe de ser o(n)
	if(!rejectsClients.empty()){
		for (i = 0; i < cromossomo.size(); i++){
			if (min_[0].first+1 == (int) cromossomo[i].second){
				
				//No começo do cromossomo(que é um começo de rota)
				//ou a parte inteira do double anterior
				//seja diferente do atual
				if(i == 0 || min_[0].first+1 != (int) cromossomo[i-1].second){
					cout << "Começo teste" << endl;
					int indi_1 = 0;
					int indi_2 = this->cromossomo[i].first-1;

					//subtrai o valor da rota
					//de 0 ao primeiro cliente
					score_fit -= distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);	

					//calcula a distância do 0 ao novo cliente que vai ser inserido
											//i
					indi_2 = rejectsClients[0].first - 1;
					score_fit += distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);

					//calcula a distância do novo cliente que seŕa inserido ao primeiro
					indi_1 = indi_2;
					indi_2 = this->cromossomo[i].first-1;
					

				}
				//fim do cromossomo é um fim de rota
				//ou quando a parte inteira do double atual, é diferente do próximo
				else if( (min_[0].first+1 != (int) cromossomo[i+1].second) 
						||i == cromossomo.size()-1){
					cout << "Fim teste" << endl;
					int indi_1 = this->cromossomo[i].first-1;
					int indi_2 = 0;

					//subtrai do ultimo cliente da rota
					//de 0 ao primeiro cliente
					score_fit -= distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);	

					//calcula a distância do 0 ao novo cliente que vai ser inserido
											//i
					indi_2 = rejectsClients[0].first - 1;
					score_fit += distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);

					//calcula a distância do novo cliente que seŕa inserido ao primeiro
					indi_1 = indi_2;
					indi_2 = 0;
					score_fit += distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);
				}//demais casos                                                         
				else{
					cout << "Meio teste" << endl;
					int indi_1 = this->cromossomo[i].first-1;
					int indi_2 = this->cromossomo[i+1].first-1;

					//subtrai do ultimo cliente da rota
					//de 0 ao primeiro cliente
					score_fit -= distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);	

					//calcula a distância do 0 ao novo cliente que vai ser inserido
											//i
					indi_2 = rejectsClients[0].first - 1;
					score_fit += distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);

					//calcula a distância do novo cliente que seŕa inserido ao primeiro
					indi_1 = indi_2;
					indi_2 = this->cromossomo[i+1].first-1;
					score_fit += distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);
				}
			}
		}
	}


	
	
	
}

void Individuo::atScore(int repair){
	vector<thrair<int, double, int>> rejectClients;
	infeasibility = 0;
	int peso_penality   = 0;

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
		
		if( (peso + demand_vec[indi_2] > capacity) and repair == 1){
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
		this->atScore(1); // atualiza o score do individuo

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
		else{//recebe pai2
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
