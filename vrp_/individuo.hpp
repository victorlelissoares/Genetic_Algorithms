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

//gap = 100 * (valorE - ValorO)/ValorE

// Hiper parâmetros do algoritmo genético
int qtd_carros    = 0;

int tam_genes     = 0; // quantidade de genes

int tam_pop      = 200; // quantidade de indivíduos da população

int tam_torneio  = 100; // tamanho do torneio

int geracoes     = 100; // quantidade de gerações

double prob_mut  = 0.8; // probabilidade de mutação

double prob_cruz = 0.8; // probabilidade de cruzamento

double prob_repair = 0;//como a reparação de cromosso é um processo muito custoso, a probabilidade de acontecer
//será baixa para garantir o bom desempenho do programa

int selec_gene    = 50; //parametro para seleção uniforme

int capacity       = 0;

int depot          = 0;

//Contém as coordenadas x e y de cada cliente
vector< pair <int, int> > distance_vec;

//contém a demanda associada a cada cliente
vector<int> demand_vec;

class Individuo{
	public:
		Individuo();
		Individuo(double v[]);
		~Individuo();
		void atScore(int repair);
		void reparationCromossome(vector<thrair<int, double, int>>);//repara o cromossomo reinserindo quem é inviavel
		int returnScore();
		void printGenes();
		void mutation(void (*type_mutation)(Individuo *indi));//mutação
		void crossing(Individuo *pai1, Individuo *pai2, 
		void (*type_crossing)(Individuo*, Individuo*, Individuo*));//operador de cruzamento, recebe a função de cruzamento como parâmetro
		Individuo& operator=(const Individuo& indi);

		//first correponde ao número do cliente e second a chave aleatória
		vector< pair<int, double> > cromossomo;//vetor de genes
		vector< thrair<int, int, int> > limits_route;//diz onde começa e termina uma rota de um veículo
		//e o peso que atualmente aquela rota possui
		double score;//score do individuo, vai ser calculado através da distância euclidiana+penalidade
		double real_score;
		int infeasibility;
		
};

Individuo& Individuo::operator=(const Individuo& indi){
	for (int k = 0; k < tam_genes; k++){
		this->cromossomo[k] = indi.cromossomo[k];
	}
	
	this->score = indi.score;
	this->real_score = indi.real_score;
	this->infeasibility = indi.infeasibility;
	
	return *this;
}
												   //contém o cliente 
void Individuo::reparationCromossome(vector<thrair<int, double, int>> rejectsClients){
	vector< pair<int, float> > min_(rejectsClients.size(), make_pair(0, MAXFLOAT));
	//this->printGenes();
	// Complexidade: O(nXm), onde n é o numero de clientes excluidos
	// e m o número de rotas
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
	
	/*
	cout << "Rota em que deve ser inserido e diferença de peso" << endl;
	for (auto i:min_){
		cout << i.first << " " << i.second << endl;
	}
	cout << "Rotas, seus limites e pesos" << endl;
	for (auto i:limits_route){
		cout << i.first << " " << i.second << " " << i.third << endl;
	}
	*/
	
	int i = 0;
	
	int actual_car = 0;
	auto aux_count = -1;
	//vai guardar respectivamente, o indice em que deve ser inserido e o
	//fit daquele cromossomo após aquela rota e o valor da chave aleatória
	thrair<int, double, double> min_insert = {-1, MAXFLOAT, 0.0};
	//melhorar essa parte para que deixe de ser o(n)
	
	for (int j = 0; j < (int)rejectsClients.size(); j++){
		double score_fit = this->real_score;
		aux_count = -1;	
		actual_car = 0;
		//cout << "reject " << j << endl;
		i = 0;
		for (i = 0; i <(int) this->cromossomo.size(); i++){
			//auto rejectClient_fit = score_fit;
			
			//conta o número da rota
			//rota 0, rota 1, e assim sucessivamente
			//cout << cromossomo[i].second <<endl;
			//cout <<"cr: " << (int) cromossomo[i].second << " actual: " << (int) cromossomo[i].second << endl;
			if( (int) cromossomo[i].second != actual_car){
				actual_car = (int) cromossomo[i].second;
				aux_count++;//troca de rota
			}
			
				// cout << "rota: " << min_[j].first << " count: " << aux_count << endl;
				//rota           cont_rota
			if (min_[j].first == aux_count){
				auto rejectClient_fit = score_fit;
				//começo de uma rota
				//pode ser o inicio de um cromossomo
				if(i == 0 || actual_car != (int) cromossomo[i-1].second){
					//cout << "Começo\n";
					//agr divide-se em dois casos
					//posso inserir antes do começo da rota
					//ou seja, depot-rejectClient-1cl-2cl...e assim sucessivamente
					//ou depot-1cl-rejectClient-2cl...e assim sucessivamente

					//supondo a inserção antes do primeiro cliente
					int indi_1 = 0;//depósito
					int indi_2 = this->cromossomo[i].first-1;//primeiro cliente
					
					//cout << "Desconsiderando " << indi_1 << "-" << indi_2+1 << endl;

					//desfaz a rota do repósito ao primeiro cliente
					rejectClient_fit -= distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);

					//novo cliente que será inserido na rota
					indi_2 = rejectsClients[j].first - 1;
					//calculo da distância do depósito ao novo cliente que será inserido
					rejectClient_fit += distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);
					//cout << "Inserindo " << indi_1 << "-" << indi_2+1 << endl;

					//cliente rejeitado
					indi_1 = indi_2;
					//primeiro cliente da rota
					indi_2 = this->cromossomo[i].first-1;
					//cout << "Inserindo " << indi_1+1 << "-" << indi_2+1 << endl;

					//calcula distâcia do cliente rejeitado inserido na rota, até o primeiro cliente
					rejectClient_fit += distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);

					//guarda a posição que deverá ser inserido
					auto insert_pos = i;
					//guarda o fit após a inserção do novo cliente
					auto fit_afterInsert = rejectClient_fit;
					//nova chave aleatória temporária
					auto new_aleatoryKey = fRand((int)this->cromossomo[i].second, this->cromossomo[i].second);
					
					// agora, precisamos verificar o caso de inserir após o primeiro cliente
					// caso o próximo cliente pertença a mesma rota, o mesmo pode ser
					// ou o ultimo cliente da rota, ou estar no meio de dois clientes da rota
					// mas, pode ser que esse primeiro cliente TAMBÉM, seja o último da rota
					// então é necessário fazer essa verificação
					
					//caso seja o primeiro E unico da rota
					//vamos tentar inserir após ele
					if(((int) cromossomo[i+1].second != actual_car) 
						||i == (int)cromossomo.size()-1){
						//cout << "Começo E fim\n";
						auto fit_endRoute = score_fit;
						//primeiro e ultimo cliente da rota
						int indi_1 = this->cromossomo[i].first-1;
						//depot
						int indi_2 = 0;

						//desconsidera distância do primeiro cliente ao depósito
						fit_endRoute -= distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);
						//cout << "Desconsiderando " << indi_1+1 << "-" << indi_2 << endl;

						//insere o novo cliente entre o ultimo cliente e o depósito
						//cliente que vai ser inserido
						indi_2 = rejectsClients[j].first - 1;
						//acrescenta distância do ultimo cliente da rota ao novo
						fit_endRoute += distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);
						//cout << "Inserindo " << indi_1+1 << "-" << indi_2+1 << endl;

						indi_1 = indi_2;
						indi_2 = 0;
						//acrescenta distância do cliente novo ao depósito
						fit_endRoute += distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);
						//cout << "Inserindo " << indi_1+1 << "-" << indi_2 << endl;

						//decide se vai inserir antes ou depois do primeiro cliente
						if(fit_endRoute < rejectClient_fit){
							//cout << "Inserir depois é melhor\n";
							//guarda a posição que deverá ser inserido
							insert_pos = i+1;
							//guarda o fit após a inserção do novo cliente
							fit_afterInsert = fit_endRoute;
							//nova chave aleatória temporária
							new_aleatoryKey = fRand(this->cromossomo[i].second, (int)this->cromossomo[i+1].second);
						}
						
					}

					if(fit_afterInsert < min_insert.second){
						min_insert = {insert_pos, fit_afterInsert, new_aleatoryKey};
					}
					//cout << min_insert.first << " " << min_insert.second << endl;
				}
				//fim de rota
				//pode ser o fim do cromossomo
				//ou quando o próximo é um carro diferente do atual
				else if( ((int) cromossomo[i+1].second != actual_car) 
						||i == (int)cromossomo.size()-1){
					//cout << "Fim teste" << endl;
					//nesse caso podemos inserir antes do cliente atual
					//ou depois, vamos tratar os dois casos

					//inserindo antes
					int indi_1 = this->cromossomo[i-1].first-1;
					int indi_2 = this->cromossomo[i].first-1;

					//desfaz a rota entre o individuo anterior e o atual
					rejectClient_fit -= distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);

					//cliente rejeitado que será inserido na rota
					indi_2 = rejectsClients[j].first -1;

					//distancia entre o cliente anterior na rota e o novo cliente
					rejectClient_fit += distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);

					//novo cliente inserido na rota
					indi_1 = indi_2;
					//ultimo cliente da rota
					indi_2 = this->cromossomo[i].first-1;

					//distância entre os dois
					rejectClient_fit += distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);

					//guarda a posição que deverá ser inserido
					auto insert_pos = i;
					//guarda o fit após a inserção do novo cliente
					auto fit_afterInsert = rejectClient_fit;
					//nova chave aleatória temporária
					auto new_aleatoryKey = fRand(this->cromossomo[i-1].second, this->cromossomo[i].second);

					//falta considerar o caso de inserir após o último cara da rota
					//inserindo após o último cliente da rota
					auto aux_anotherRoute = score_fit;
					
					//ultimo cliente atendido pela rota
					indi_1 = this->cromossomo[i].first-1;
					//depot
					indi_2 = 0;

					//desconsidera distância entre o ultimo cliente e o depósito
					aux_anotherRoute -= distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);

					//cliente que vai ser inserido entre o ultimo cliente e o depósito
					indi_2 = rejectsClients[j].first -1;

					//considera distância entre o ultimo cliente e o que vai ser inserido
					aux_anotherRoute += distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);

					//individuo que foi inserido
					indi_1 = indi_2;
					//depot
					indi_2 = 0;
					
					//considera a distância entre o cliente que foi inserido até o depot
					aux_anotherRoute += distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);
					
					//decide se vai inserir antes ou depois do ultimo cliente
					if(aux_anotherRoute < rejectClient_fit){
						//guarda a posição que deverá ser inserido
						insert_pos = i+1;
						//guarda o fit após a inserção do novo cliente
						fit_afterInsert = aux_anotherRoute;
						new_aleatoryKey = fRand(this->cromossomo[i].second, ((int)this->cromossomo[i].second) + 1);
					}
					if(fit_afterInsert < min_insert.second){
						min_insert = {insert_pos, fit_afterInsert, new_aleatoryKey};
					}
					//cout << min_insert.first << " " << min_insert.second << endl;
				}
				//demais casos, no meio de uma rota
				else{
					//cout << "Meio Teste\n";
					// nesse caso, estamos considerando a seguinte situação
					// cl1-cl2-cl3, o i está em cl2,
					// então precisamos decidir se inserimos antes ou depois do cl2

					//vamos ver como fica a distância percorrida, tentando inserir antes do cl2
					//nesse caso podemos inserir antes do cliente atual
					//ou depois, vamos tratar os dois casos

					//inserindo antes
					int indi_1 = this->cromossomo[i-1].first-1;
					int indi_2 = this->cromossomo[i].first-1;

					//desfaz a rota entre o individuo anterior e o atual
					rejectClient_fit -= distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);

					//cliente rejeitado que será inserido na rota
					indi_2 = rejectsClients[j].first -1;

					//distancia entre o cliente anterior na rota e o novo cliente
					rejectClient_fit += distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);

					//novo cliente inserido na rota
					indi_1 = indi_2;
					//o outro cliente da rota
					indi_2 = this->cromossomo[i].first-1;

					//distância entre os dois
					rejectClient_fit += distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);


					//guarda a posição que deverá ser inserido
					auto insert_pos = i;
					//guarda o fit após a inserção do novo cliente
					auto fit_afterInsert = rejectClient_fit;
					auto new_aleatoryKey = fRand(this->cromossomo[i-1].second, this->cromossomo[i].second);

					//falta considerar o caso de inserir após cl2
					//inserindo após cl2
					auto aux_anotherRoute = score_fit;
					
					//ultimo cliente atendido pela rota
					indi_1 = this->cromossomo[i].first-1;
					//depot
					indi_2 = this->cromossomo[i+1].first-1;

					//desconsidera distância entre o ultimo cliente e o depósito
					aux_anotherRoute -= distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);

					//cliente que vai ser inserido entre o ultimo cliente e o depósito
					indi_2 = rejectsClients[j].first -1;

					//considera distância entre o ultimo cliente e o que vai ser inserido
					aux_anotherRoute += distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);

					//individuo que foi inserido
					indi_1 = indi_2;
					//depot
					indi_2 = this->cromossomo[i+1].first-1;
					
					//considera a distância entre o cliente que foi inserido até o depot
					aux_anotherRoute += distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);
					
					//decide se vai inserir antes ou depois do ultimo cliente
					if(aux_anotherRoute < rejectClient_fit){
						//guarda a posição que deverá ser inserido
						insert_pos = i+1;
						//guarda o fit após a inserção do novo cliente
						fit_afterInsert = aux_anotherRoute;
						new_aleatoryKey = fRand(this->cromossomo[i].second, this->cromossomo[i+1].second);
					}

					if(fit_afterInsert < min_insert.second){
						min_insert = {insert_pos, fit_afterInsert, new_aleatoryKey};
					}
					//cout << min_insert.first << " " << min_insert.second << endl;
				}
			}
		}
		// cout << "Entrei aqui\n";
		// cout << min_insert.first << " " << min_insert.second << endl;
		vector<pair<int, double>>::iterator it;
									//posição em que deve ser inserido
		it = this->cromossomo.begin() + min_insert.first;
		this->cromossomo.insert(it, make_pair(rejectsClients[j].first, min_insert.third) );

		min_insert = {-1, MAXFLOAT, 0.0};
	}
	this->atScore(0);
		
}

void Individuo::atScore(int repair){
	// Como a ordem de visitas de um veículo é dado pelo seu double
	// ordeno o vetor para saber qual a ordem de visitas aos clientes
	sort(this->cromossomo.begin(), this->cromossomo.end(),
     [](const pair<int, double>& lhs, const pair<int, double>& rhs) {
             return lhs.second < rhs.second; } );

	this->infeasibility = 0;
	int peso_penality   = 0;
	double score_fit = 0;
	int i = 0;
	int indi_1 = 0;
	int indi_2 = this->cromossomo[0].first-1;
	int actual_car = (int) this->cromossomo[0].second;//considerando que existe mais de um veículo
	int peso = 0;
	int init_route = i, final_route;

	vector<thrair<int, double, int>> rejectClients;
	vector<thrair<int, int, int>> routes;
	
	
	while ( i < (int)cromossomo.size()){
		
		if( (peso + demand_vec[indi_2] > capacity) and repair == 1){
			//cliente e indice de onde foi removido do vetor
			// cout << "Rejeitado: " << this->cromossomo[i].first << " " << this->cromossomo[i].second 
			// << endl;
			//removendo indi2
			rejectClients.push_back({this->cromossomo[i].first, this->cromossomo[i].second, i});
			vector<pair< int, double>>::iterator it = cromossomo.begin() + i;//calcula posição do iterador
			this->cromossomo.erase(it);
			
		}
		else{
			score_fit +=  distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);	
			peso += demand_vec[indi_2];
			indi_1 = indi_2;
			//peso += demand_vec[indi_2];
			i++;
			

		}
		//score_fit +=  distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);
		indi_2 = this->cromossomo[i].first-1;
		
		//significa que trocou de carro
		//ou seja, outra rota
		//então deve-se retornar ao depósito
		if(actual_car < ((int) this->cromossomo[i].second)){
			//cout_routes++;
			//cout << "troca carro" << endl;
			actual_car = (int) this->cromossomo[i].second;
			
			score_fit +=  distEuclidiana(distance_vec[indi_1], distance_vec[0]);//retorna ao depósito
			
			final_route = i - 1;
			//cout << "ini: " << init_route << " fi: " << final_route << endl;
			//limits_route.push_back({init_route, final_route, peso});
			routes.push_back({init_route, final_route, peso});
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
			
			// quer dizer que terminaram as entregas
			// então o caminhão atual deve retornar ao depósito
			final_route = i-1;
			//cout << "ini: " << init_route << " fi: " << final_route << endl;
			// limits_route.push_back({init_route, final_route, peso});
			routes.push_back({init_route, final_route, peso});

			score_fit += distEuclidiana(distance_vec[indi_1], distance_vec[0]);
			if(peso > capacity){//significa que a capacidade do caminhão foi ultrapassada
				peso_penality += (peso - capacity);
				this->infeasibility += 1;
			}

		}

	}	

	this->real_score = score_fit;//distância euclidiana efetivamente percorrida
	//cout << "distância percorrida sem penalização-> " << this->real_score << " " << score_fit <<endl;
	this->score = score_fit + (pow(peso_penality, 2.)) * infeasibility;
	//cout << "distância percorrida com penalização-> " << this->score << " " << score_fit + (pow(peso_penality, 2.)) * infeasibility <<endl;
	limits_route = routes;
	if (repair){
		//cout << "Reparando..." << endl;
		// for (auto k:limits_route){
		// 	cout << k.first << " " << k.second << " " << k.third << endl;
		// }

		// //cout << "Removidos : ";
		// for (auto i:rejectClients){
		// 	// vector<pair< int, double>>::iterator it = cromossomo.begin() + i.third;//calcula posição do iterador
		// 	// this->cromossomo.erase(it);
		// 	cout << i.first << " " << i.second << " " << i.third << endl;
		// }
		// cout << endl;
		// cout << "Antes reparar" << endl;
		// this->printGenes();
		this->reparationCromossome(rejectClients);
	}
	
}

Individuo::Individuo(): score(0), real_score(0), infeasibility(0){
	//cout << "Construtor do individuo" << endl;
	int j = 2;
	for (int i = 0; i < tam_genes; ++i){
		double gene =  fRand(1, qtd_carros+1);//gera o alelo(valor do gene)
		this->cromossomo.push_back(make_pair(j++, gene));//insere o gene no cromossomo
	}
	//cout << "Genes da criação de individuo" << endl;
	//this->printGenes();

	this->atScore(0); // atualiza o score do individuo
	//this->printGenes();
	double prob = fRand(0, 1);
	
	if(prob_repair > prob and this->infeasibility){//então tem que reparar o cromossomo
		//cout << "Reparando..." << endl;
		this->atScore(1); // atualiza o score do individuo, passando a flag que tem que reparar

	}
	//cout << "Saindo do Construtor do individuo" << endl;
}

//apenas para debug
Individuo::Individuo(double v[]): score(0), real_score(0), infeasibility(0){
	int j = 2;
	for (int i = 0; i < tam_genes; ++i){
		//double gene =  fRand(1, qtd_carros+1);//gera o alelo(valor do gene)
		this->cromossomo.push_back(make_pair(j++, v[i]));//insere o gene no cromossomo
	}

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
	cout << endl << endl;
}
