#define _CRT_SECURE_NO_WARNINGS 1
#include "algoritmo.h"
#include "funcao.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Serve apenas para mudar um numero para outro
int changev(struct info d, int i){
    int number=0;
    do{
        number = rand()%d.subconj + 1;
        //printf("Numero random: %d", number); 
    }while(number == i);
    return number;
}

//Avalia um só cromossoma, em que neste caso é apenas utilizado no hill-climbing
pchrom eval_one(pchrom pop, struct info d, int mat[][3]){
    int i, j;
    memset(pop->fitness, 0, sizeof pop->fitness);
    for(i = 0; i < d.nruns; i++){
        for(j = 0; j < d.subconj; j++){
            if(pop->p[mat[i][0]] == pop->p[mat[i][1]]){
                pop->fitness[pop->p[mat[i][0]]-1] += mat[i][2];
                pop->fitness[d.subconj] += mat[i][2]; 
                //printf("R: %d %d %d\n", i, pop->p[mat[i][0]], pop->p[mat[i][1]]);
                //printf("Aqui: %d %d %d\n", mat[i][0], mat[i][1], mat[i][2]);
                //printf("Qualidade: %d=%d+%d\n\n", pop->qualidade[2], pop->qualidade[0], pop->qualidade[1]);
                break;
            }
        }
    }
    return pop;
}

//Avalia cada membro da pop que recebe e preenche o fitness da mesma
void eval_individual(pchrom pop, struct info d, int mat[][3]){
    int i, j;
    for(int x = 0; x < d.popsize; x++)
        memset(pop[x].fitness, 0, sizeof pop[x].fitness);
    
    for(int x = 0; x < d.popsize; x++){
        for(i = 0; i < d.nruns; i++){
            for(j = 0; j < d.subconj; j++){
                if(pop[x].p[mat[i][0]] == pop[x].p[mat[i][1]]){
                    pop[x].fitness[pop[x].p[mat[i][0]]-1] += mat[i][2];
                    pop[x].fitness[d.subconj] += mat[i][2]; 
                    //printf("R: %d %d %d\n", i, pop[x].p[mat[i][0]], pop[x].p[mat[i][1]]);
                    //printf("Aqui: %d %d %d\n", mat[i][0], mat[i][1], mat[i][2]);
                    //printf("Qualidade: %d=%d+%d\n\n", pop[x].fitness[2], pop[x].fitness[0], pop[x].fitness[1]);
                    break;
               }
            }
        }
    }
}


//Recebe a estrutura e avalia todos os membros da pop da mesma
pchrom evaluate(pchrom pop, struct info d, int mat[][3])
{
	int i;
    eval_individual(pop, d, mat);

    return pop;
}
