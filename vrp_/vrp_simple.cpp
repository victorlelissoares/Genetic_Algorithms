#include "population.hpp"

void read_file(string file_);


// Uma certa porcentagem das melhores soluções é mantida para a proxima geração(elitismo)
// Uma taxa de mutação bastante alta é usada para manter a diversidade
// e por fim, o restante da população é produzido por reprodução
int main(int argc, char const *argv[]){
	
	if(argc < 2){
		cout << "error:./vrp arquivo_entrada" << endl;
		exit(0);
	}
	
	read_file(argv[1]);
	srand(time(0));
	Population pop;
	Individuo teste;

	//pop.printPopulation();

	// teste de cruzamento
	// teste = pop.population[2];
	// teste.printGenes();
	// teste.mutation(&imigracaoMutation);
	// teste.printGenes();
	// teste.atScore();

	// teste de reprodução
	// Individuo filho;
	// filho.crossing(&pop.population[0], &pop.population[1], &uniformCrossing);
	// filho.printGenes();
	
	//Para que o cruzamento seja aplicado, os "pais" devem ser selecionados
	//Há vários procedimentos de seleção, dentre eles: Seleção por Roleta e a Seleção por Torneio 
	// No procedimento de seleção por torneio, sorteiam-se dois indivíduos ao acaso, 
	// comparam-se suas aptidões e o mais apto destes dois é selecionado. Este procedimento é repetido para
	// cada indivíduo a ser selecionado.

	pop.printPopulation();
	sort(pop.population.begin(), pop.population.end(),[](const Individuo& lhs, const Individuo& rhs) {
              return lhs.score < rhs.score; } );
	pop.printPopulation();//ordena população com base no seu score, para selecionar os melhores individuos

	cout<<endl;
	for(int numGeracoes = 0; numGeracoes < geracoes; numGeracoes++){
		cout << "Geração " << numGeracoes << endl;
		sort(pop.population.begin(), pop.population.end(),[](const Individuo& lhs, const Individuo& rhs) {
              return lhs.score < rhs.score; } );
		pop.printPopulation();//ordena população com base no seu score, para selecionar os melhores individuos
		
		int tam_elitismo = rand() % tam_pop / 2;//define quantos "melhores elementos" serão mantidos
		//cout << "elitismo: " << tam_elitismo << endl;

		//o restante da população será produzido por cruzamento e mutação
		for (int i = 0; i < tam_torneio; i++){//implementa o torneio
			
			double prob = fRand(0, 1);
			//cout << "probabilidade: " << prob << endl;
			if(prob < prob_cruz){
				
				int indice_pai1;
				do
				{
					indice_pai1 = rand() % tam_pop;
				} while (indice_pai1 >= 0 && indice_pai1 <= tam_elitismo-1);
				//cout << "Indice pai 1: " << indice_pai1 << endl;
				 
				
				int indice_pai2 = rand() % tam_pop;
				
				//garante que os indices são diferentes e que mantem o n primeiros "melhores números"
				while(indice_pai2 == indice_pai1 && (indice_pai2 >= 0 && indice_pai2 <= tam_elitismo-1)){
					indice_pai2 = rand() % tam_pop;
				}
				//cout << "Indice pai 2: " << indice_pai2 << endl;
				Individuo filho;

				//filho.print_genes();
				filho.crossing(&pop.population[indice_pai1], &pop.population[indice_pai2], &uniformCrossing);
				filho.atScore();//atualiza o score do filho

				//cout << "Filho: ";
				//filho.printGenes();

				double prob = fRand(0, 1);

				if(prob < prob_mut){
					filho.mutation(&imigracaoMutation);
					filho.atScore();//atualiza o score
				}

				if( pop.population[indice_pai1].score > filho.score){  
					//cout << "pai antes: ";
					pop.population[indice_pai1].printGenes();
					//copia genes do filho para o pai
					for (int k = 0; k < tam_genes; ++k){
						pop.population[indice_pai1].cromossomo[k] = filho.cromossomo[k];
					}
					//cout << "pai depois: ";
					pop.population[indice_pai1].atScore();//atualiza o score
					pop.population[indice_pai1].printGenes();

				}
			}

		}
		cout << endl;

	}
	cout << endl;
	pop.printPopulation();
	cout << endl;
	return 0;
}

void read_file(string file){
	ifstream myfile;
	string myline;
	char *line;
	int i = 0;

	myfile.open(file);
	getline(myfile, myline, '\n');//ignore name
	getline(myfile, myline, '\n');//ignore comment
	getline(myfile, myline, '\n');//ignore type
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
	
	//ignora EDGE_WEIGHT_TYPE : EUC_2D 
	getline(myfile, myline, '\n');//ignore name

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



