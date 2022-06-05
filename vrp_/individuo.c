#include "individuo.h"


Individuo::Individuo(){
	int j = 2;
	for (int i = 0; i < tam_genes; ++i){
		double gene =  fRand(1, qtd_carros+1);//gera o alelo(valor do gene)

		this->cromossomo.push_back(make_pair(j++, gene));//insere o gene no cromossomo
	}
	this->infeasibility = 0;//diz se a solução é viável ou não
	this->atScore(); // atualiza o score do individuo, ou seja, a distância total * pela inviabilidade
}

Individuo::~Individuo(){
    //destrutor
}

void Individuo::atScore(){
	this->infeasibility = 0;
	//cout << endl << "Função Atualiza Score" << endl;
	vector<pair<int, double>> v = this->cromossomo;
	// Como a ordem de visitas de um veículo é dado pelo seu double
	// ordeno o vetor para saber qual a ordem de visitas aos clientes
	sort(v.begin(), v.end(),
     [](const pair<int, double>& lhs, const pair<int, double>& rhs) {
             return lhs.second < rhs.second; } );
	
	// for(auto i: v)
	// 	cout << "Cliente: " << i.first << " " << i.second << " ";

	// cout << endl;

	double score_fit = 0;
	int i = 0;
	int indi_1 = 0;
	int indi_2 = v[i].first-1;
	int actual_car = 1;//considerando que existe mais de um veículo
	int peso = 0;
	
	//cout << "veículo " << (int) v[i].second << " saindo do depósito" << endl;

	while ( i < tam_genes){
		//cout << indi_1+1 << " e " << indi_2+1 << "; ";
		score_fit +=  distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]);
		//cout << distEuclidiana(distance_vec[indi_1], distance_vec[indi_2]) << endl;;
		
		indi_1 = indi_2;
		peso+= demand_vec[indi_1];
		// cout << "Peso a ser adicionado: " << demand_vec[indi_1] <<
		// "; Peso até então: " << peso << endl;
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
			//cout << "retornando ao depósito" << endl;
			//cout << indi_1+1 << " e " << "1" << "; ";
			//cout << distEuclidiana(distance_vec[indi_1], distance_vec[0]) << endl;
			score_fit +=  distEuclidiana(distance_vec[indi_1], distance_vec[0]);//retorna ao depósito
			//cout << "Peso total usado: " << peso << endl;
			//cout << "Total Percorrido: " << score_fit << endl;
			
			//após retornar o depósito, checamos se aquela rota
			// não excedeu o quanto o caminhão pode levar
			// caso sim, multiplicamos o quanto foi excedido
			// para "penalizar" a solução
			if(peso - capacity > 0){//significa que a capacidade do caminhão foi ultrapassada
				score_fit *= (peso - capacity);
				this->infeasibility += actual_car;
			}
			
			// cout << score_fit << endl;
			//cout << "Total Percorrido com penalização: " << score_fit << endl;
			// após isso, significa que o próximo cliente, será visitado por outro caminhão
			// então, o mesmo deve sair do depósito
			//cout << "Veículo " <<(int)v[i].second << " saindo do depósito" << endl;
			//cout << "1" << " e " << indi_2+1 << "; ";
			//cout << distEuclidiana(distance_vec[0], distance_vec[indi_2]) << endl;
			score_fit += distEuclidiana(distance_vec[0], distance_vec[indi_2]);//adiciona a distância de fato
			
			peso = 0;
			peso += demand_vec[indi_2];
			indi_1 = indi_2;
			i++;
			indi_2 = v[i].first-1;
		}
		if(indi_2+1 == 0){//fim da linha
			//cout << "Voltando ao depósito" << endl;
			//cout << "Peso : " << peso << endl;
			// quer dizer que terminaram as entregas
			// então o caminhão atual deve retornar ao depósito
			//cout << indi_1+1 << " e " << "1" << "; " ;
			//cout << distEuclidiana(distance_vec[indi_1], distance_vec[0]) << endl;
			score_fit+=distEuclidiana(distance_vec[indi_1], distance_vec[0]);
			if(peso - capacity > 0){//significa que a capacidade do caminhão foi ultrapassada
				score_fit *= (peso - capacity);
				this->infeasibility += 1;
			}
			// cout << score_fit << endl;
			//cout << "Total Percorrido com penalização: " << score_fit << endl;

		}

	}
	
	this->score = score_fit;
	//cout << "Total Percorrido: " << score_fit << endl;
	//cout << "Finaliza Função Score" << endl << endl;
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
		indi->cromossomo.at(i) = pair<int, double>(indi->cromossomo[i].first, gene);//altera o valor do gene
	}

}

void Individuo::mutation(void(*type_mutation)(Individuo *indi)){
	type_mutation(this);
}

void Individuo::printGenes(){
	for(auto i: this->cromossomo){
		//cout << "Cliente: " << i.first << " " << i.second << " ";
		cout << i.second << " ";
	}
	cout << "; Fit: " << this->score;
	cout << " ; Inviavel? " << this->infeasibility;
	cout << endl;
}
