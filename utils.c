#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "algoritmo.h"
#include "utils.h"
#include <signal.h>
#include <unistd.h>

// Inicialização do gerador de números aleatórios
void init_rand()
{
	srand((unsigned)time(NULL));
}

//Recebe o sinal SIGEGV(Segmentation error normalmente) e retorna um aviso 
void signalh(int sign){
	printf("Houve um erro de segmentação no decorrer do programa\n");
	printf("Sinal: %d\n", sign);
	exit(0);
}

//Recolhe toda a informação importante e coloca na estrutura info e na matriz mat
struct info init_data(char *filename, int mat[][3])
{
	struct  info x;
	FILE    *f;
	int     i, temp;
	char *	s;

	char dir[255];
	x.nruns=0;
	snprintf(dir, sizeof(dir), "./instâncias/%s", filename);
	//snprintf(dir, sizeof(dir), "./instâncias/n240.txt");
	printf("Nome: %s<--\n", dir);
	signal(SIGSEGV, signalh);
	f = fopen(dir, "rt");

	if (!f)
	{
		printf("Ficheiro nao encontrado\n");
		exit(1);
	}
	
	// Leitura dos parâmetros do problema
	fscanf(f, "%d ",&x.elements);
	fscanf(f, "%d ",&x.subconj);
	
	// Leitura dos dados
	fscanf(f, "ss ");

	do{
		fscanf(f, "%d ", &temp);
	}while(temp != 0);

	mat[0][0]=0;
	fscanf(f, "%d %d ",&mat[x.nruns][1], &mat[x.nruns][2]);
	//printf("N%d: %d %d %d\n",x.nruns, mat[x.nruns][0], mat[x.nruns][1], mat[x.nruns][2]);
	x.nruns++;

	while(fscanf(f, "%d %d %d ",&mat[x.nruns][0], &mat[x.nruns][1], &mat[x.nruns][2]) != EOF){
		//printf("N%d: %d %d %d\n",x.nruns, mat[x.nruns][0], mat[x.nruns][1], mat[x.nruns][2]);
		x.nruns++;
	}
	fclose(f);
	//printf("Runs %d\n", x.nruns);
	if(x.subconj > 0)
		x.N=x.elements/x.subconj;
	else
		x.N=0;
	// Devolve a estrutura com os parâmetros
	return x;
}

// Escolhe um numero random entre s numeros
int rands(int s)
{	
	float frac = (float)rand() / RAND_MAX;
	//printf("Percentagem %f\n", frac); 
	for(int i=0; i < s; i++){
		float temp = ((float)1/s)*(i+1);
		//printf("Temp %f\n", temp); 
		if(frac < temp)
			return i+1;
	}
}

//Voltar a arranjar os números de pop
void rearange(pchrom pop, struct info d){
	int i, temp[d.subconj], count;
	
	for (i=0; i< d.elements; i++){
		pop->p[i] = 0;
	}
	for(i=0; i<d.subconj; i++)
	temp[i] = 0;

	for (i=0; i<d.elements; i++){
		count = 0;
		while(1){
			int temp2 = rands(d.subconj);
				if(temp[temp2-1] < d.N && temp2 <= d.subconj){
					pop->p[i] = temp2;
					temp[temp2-1] += 1;
					break;		
				}
			count++;
			if(count >= 10000) break;
		}
	}
	
}

//Inicia uma população inteira
pchrom init_pop(pchrom pop, struct info d){
	int     i, x, temp[d.subconj], count, ig = 0;
	pchrom  indiv; 

	indiv = malloc(sizeof(chrom)*d.popsize);
	if (indiv==NULL)
	{
		printf("Erro na alocacao de memoria\n");
		exit(1);
	}
	if(d.subconj == 0) return indiv;

	for(x=0; x < d.popsize; x++){
		//printf("\nIndiv %d: ", x);
		for (i=0; i<d.subconj; i++) temp[i] = 0;
		for (i=0; i<d.elements; i++){
			while(1){
				int temp2 = rands(d.subconj);
					if(temp[temp2-1] < d.N){
						indiv[x].p[i] = temp2;
						//printf("%d ",indiv[x].p[i]);
						temp[temp2-1] += 1;
						break;		
					}
				count++;
				if(count >= 10000*d.elements) break;
			}
		}

		//Verificar se há algum igual, caso haja, criar outro
		if(d.subconj > 2){
			for(int k=0; k < x-1; k++){
				for(int n=0; n<d.elements; n++){
					if(indiv[x].p[n] == indiv[k].p[n]) 
						ig++;
					if(ig == d.elements){
						x--;
						ig = 0;
						//printf("\n\n\nAqui\n\n\n");
						break;
					}
				}
			ig = 0;
			}	
		}
	count = 0;
	}
return indiv;
}


//Inicia um indivíduo de uma população
pchrom init_ind(struct info d)
{
	int     i, temp[d.subconj], count;
	pchrom  indiv; 
	
	indiv = malloc(sizeof(chrom)*1);
	if (indiv==NULL)
	{
		printf("Erro na alocacao de memoria\n");
		exit(1);
	}

	if(d.subconj == 0) return indiv;
	for (i=0; i<d.elements; i++){
		while(1){
			int temp2 = rands(d.subconj);
				if(temp[temp2-1] < d.N){
					indiv->p[i] = temp2;
					temp[temp2-1] += 1;
					break;		
				}
			count++;
			if(count >= 100000) break;
		}
	}
	return indiv;
}

//Determina qual é o melhor individuo da populacao e retorna o mesmo
chrom bestpop(chrom best, pchrom pop, struct info d){
	for(int i=1; i< d.popsize; i++)
		if(pop[i].fitness[d.subconj] > best.fitness[d.subconj]){
			best = pop[i];
			printf("Melhor fitness até agora: %d\n", pop[i].fitness[d.subconj]);
		}
	return best;
}

// Devolve um valor inteiro distribuido uniformemente entre min e max
int random_l_h(int min, int max)
{
	return min + rand() % (max-min+1);
}

// Devolve um valor real distribuido uniformemente entre 0 e 1
float rand_01()
{
	return ((float)rand())/RAND_MAX;
}