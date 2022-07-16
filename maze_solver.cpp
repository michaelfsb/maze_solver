/*----------------------------------------------------------------------------------------------*
*                      UNIVERSIDADE FEDERAL DE MINAS GERAIS                    					*
*                                                                              					*
*                               	Labirinto                                  					*
*                    			    ATR 2017/2							       					*
*                                                                              					*
*Autor: Michael Feliphe da S. Barbosa     Matricula: 2013024643                					*
*                                                                              					*
*Breve descrição do trabalho: Ler um labirinto salvo em um arquivo .txt        					*
*                             Resolver esse labirinto se possivel              					*
*                             Usa threads na resoluzação										*
*																								*
*Comando de compilação utilizado: g++ -std=c++14 maze_solver.cpp -o maze_solver -lpthread	    *
*																								*
*Execução: maze_solver.exe maze.txt			                                        			*
*         .\maze_solver maze.txt				                                        		*
*-----------------------------------------------------------------------------------------------*/

#include <iostream>
#include <thread>
#include <fstream> 
#include <chrono>
#include <vector>

using namespace std;

/*------------------------------------------------------------------------------*
*                        Declaração de Funções                                  *
*-------------------------------------------------------------------------------*/

void imprime_labirinto(vector< vector<char> > *lab, int *enable);

void acha_saida(vector< vector<char> > *lab, int Xi, int Yi, int *fim);

/*------------------------------------------------------------------------------*
*                        MAIN             				                        *
*-------------------------------------------------------------------------------*/

int main(int argc, char *argv[]){

	int i, j, m, n, Xi, Yi, enPrint;
	char c, info[3];

	ifstream inFile(argv[1]);

	if ( !inFile ){	
		cerr << "Erro: Arquivo não pode ser aberto" << endl;
		exit( EXIT_FAILURE );
	}

	inFile.get(info,3);
	m = atoi(info);
	inFile.get(c); // pula o cursor uma posição
	inFile.get(info,3);
	n = atoi(info);
	inFile.get(c); // pula o cursor uma posição

	vector< vector<char> > labirinto(m, vector<char>(n));
	for(i=0; i<m; i++){
		for(j=0; j<n; j++){
			inFile.get(c);
			labirinto[i][j] = c;
			// Testa se é a entrada do labirinto
			if(c == 'e'){
				Xi = j;
				Yi = i;
			}
		}
		inFile.get(c);
	}
	inFile.close();

	enPrint = 1;
	thread tela(imprime_labirinto, &labirinto, &enPrint);

	thread sol(acha_saida, &labirinto, Xi, Yi, &enPrint);
	
	tela.join();
	sol.join();	
	return 0;	
}

/*------------------------------------------------------------------------------*
*                        Escopo das Funções                                     *
*-------------------------------------------------------------------------------*/

void imprime_labirinto(vector< vector<char> > *lab, int *enable){
	int i,j,m,n;

	m = lab->size();
	n = lab->at(1).size();
	cout << m << endl;
	cout << n << endl;
	
	while(*enable){
		system("cls");	
		for(i=0; i<m; i++){
 			for(j=0; j<n; j++){
 				cout << lab->at(i).at(j);
 			}
 			cout << endl;
 		}
 		this_thread::sleep_for(250ms);
 	}
}

void acha_saida(vector< vector<char> > *lab, int X, int Y, int *fim){
	int direc[4];
	int xmax, ymax, nun_threads, aux, i;
	vector<thread> novas_threads;

	nun_threads = 0;
	direc[0] = 0;
	direc[1] = 0;
	direc[2] = 0;
	direc[3] = 0;

	if(lab->at(Y).at(X) == 's'){
		*fim = 0; 
	}

	lab->at(Y).at(X) = 'o';

	while(*fim == 1){
		
		ymax = lab->size() -1;
		xmax = lab->at(1).size() -1;

		if(X < xmax){
			if(lab->at(Y).at(X+1) == ' ') // esquerda
				direc[0] = 1;
			if(lab->at(Y).at(X+1) == 's')
				*fim = 0;
		} 
		if(Y < ymax){
			if(lab->at(Y+1).at(X) == ' ') // baixo
				direc[1] = 1;
			if(lab->at(Y+1).at(X) == 's')
				*fim = 0;
		}
		if(X > 0){	
			if(lab->at(Y).at(X-1) == ' ') // direita
				direc[2] = 1;
			if(lab->at(Y).at(X-1) == 's')
				*fim = 0;
		}
		if(Y > 0){
			if(lab->at(Y-1).at(X) == ' ') // cima
				direc[3] = 1;
			if(lab->at(Y-1).at(X) == 's')
				*fim = 0;
		}

		aux = (1*direc[0] + 2*direc[1] + 4*direc[2] + 8*direc[3]);

		while(aux>0){
			if(aux<=2 || aux==4 || aux==8){ // só uma direção possivel
				if(direc[0] == 1){
					X++;
					lab->at(Y).at(X-1) = '.';
				}
				if(direc[1] == 1){
					Y++;
					lab->at(Y-1).at(X) = '.';
				}
				if(direc[2] == 1){
					X--;
					lab->at(Y).at(X+1) = '.';
				}
				if(direc[3] == 1){
					Y--;
					lab->at(Y+1).at(X) = '.';
				}
			}
			else{
				if(aux == 3){
					novas_threads.emplace_back(thread(acha_saida, lab, X, (Y+1), fim));
					nun_threads++;
					X++;
					lab->at(Y).at(X-1) = '.';
				}
				else if(aux>4 && aux<8){
					if(direc[0] == 1){
						novas_threads.emplace_back(thread(acha_saida, lab, (X+1), Y, fim));
						nun_threads++;
					}
					if(direc[1] == 1){
						novas_threads.emplace_back(thread(acha_saida, lab, X, (Y+1), fim));	
						nun_threads++;
					}
					X--;
					lab->at(Y).at(X+1) = '.';
				}
				else if(aux>8){
					if(direc[0] == 1){
						novas_threads.emplace_back(thread(acha_saida, lab, (X+1), Y, fim));
						nun_threads++;
					}
					if(direc[1] == 1){
						novas_threads.emplace_back(thread(acha_saida, lab, X, (Y+1), fim));	
						nun_threads++;
					}
					if(direc[2] == 1){
						novas_threads.emplace_back(thread(acha_saida, lab, (X-1), Y, fim));	
						nun_threads++;
					}
					Y--;
					lab->at(Y+1).at(X) = '.';
				}
				
			}

			lab->at(Y).at(X) = 'o';

			direc[0] = 0;
			direc[1] = 0;
			direc[2] = 0;
			direc[3] = 0;

			if(X < xmax){
				if(lab->at(Y).at(X+1) == ' ') // esquerda
					direc[0] = 1;
				if(lab->at(Y).at(X+1) == 's')
					*fim = 0;
			} 
			if(Y < ymax){
				if(lab->at(Y+1).at(X) == ' ') // baixo
					direc[1] = 1;
				if(lab->at(Y+1).at(X) == 's')
					*fim = 0;
			}
			if(X > 0){	
				if(lab->at(Y).at(X-1) == ' ') // direita
					direc[2] = 1;
				if(lab->at(Y).at(X-1) == 's')
					*fim = 0;
			}
			if(Y > 0){
				if(lab->at(Y-1).at(X) == ' ') // cima
					direc[3] = 1;
				if(lab->at(Y-1).at(X) == 's')
					*fim = 0;
			}
			aux = (1*direc[0] + 2*direc[1] + 4*direc[2] + 8*direc[3]);
			this_thread::sleep_for(250ms);

		}
		for (i=0; i<nun_threads; ++i){
			novas_threads[i].join();
		}
	}
}
/*----------------------------------------------------------------------------------------------*
*                                      FIM                                      				*
*-----------------------------------------------------------------------------------------------*/