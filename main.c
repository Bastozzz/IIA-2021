#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "algoritmo.h"
#include "funcao.h"
#include "utils.h"

#define DEFAULT_RUNS	10

int main(int argc, char *argv[])
{
	char        nome_fich[100];
	struct info EA_param;
	pchrom      pop = NULL;
	chrom 		temp;
	int         r, runs, i, inv, mat[MAX_ELE][3], N;
	
    // Lê os argumentos de entrada
	if (argc == 3)
	{
		runs = atoi(argv[2]);
		strcpy(nome_fich, argv[1]);
	}
	else
        // Se o número de execuções do processo não for colocado nos argumentos de entrada, define-o com um valor por defeito
        if (argc == 2)
        {
            runs = DEFAULT_RUNS;
            strcpy(nome_fich, argv[1]);
        }
        // Se o nome do ficheiro de informações não for colocado nos argumentos de entrada, pede-o novamente
        else
        {
            runs = DEFAULT_RUNS;
            printf("Nome do Ficheiro: ");
            scanf("%s", nome_fich);
        }
    // Se o número de execuções do processo for menor ou igual a 0, termina o programa
	if (runs <= 0)
		return 0;
	
	init_rand();
    // Preenche a matriz com dados dos elementos (peso e valor) e a estrutura EA_param que foram definidos no ficheiro de input
	EA_param = init_data(nome_fich, mat);
	runs = EA_param.nruns;
	pop = init_ind(EA_param);
	temp = *pop;
	//printf("N=M/G %d=%d/%d\n", EA_param.N, EA_param.elements, EA_param.subconj);

	//Trapa-colinas
	trepacolinas(pop, EA_param, mat);

	//Algoritmo evolutivo
	*pop = temp;
	evolutivo(pop, EA_param, mat);
	
	//Algoritmo híbrido
	*pop = temp;
	hibrido(pop, EA_param, mat);


}
