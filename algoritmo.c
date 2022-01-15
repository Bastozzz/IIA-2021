#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include "algoritmo.h"
#include "utils.h"
#include "funcao.h"
#include <string.h>

//Função usada para o método de hill-climbing
chrom climb(pchrom pop, struct info d, int mat[][3]){
    int g = 0;
    chrom viz = *pop;
    chrom max;
    //Quantas vezes vai executar o algorítmo
    for(int i = 0; i<5; i++){
        /*printf("\n\nGrupo inicial %d: ", i+1);
        for(int i = 0; i < d.elements; i++)
            printf("%d ", viz.p[i]);
        printf("\nQualidade:  %d\n", viz.fitness[d.subconj]);
        */

        for(int j = 0; j<3; j++){
            //Mudar primeiro número
            int r = rand()%d.elements;
            int temp1 = r;
            int temp2 = pop->p[r];
            pop->p[r] = changev(d, pop->p[r]);
            int temp3 = pop->p[r];
            //printf("Posicao: %d de: %d para %d\n", r+1, temp2, pop->p[r]);
        
            //Mudar segundo número
            while(1){
                r = rand()%d.elements;
                if(r != temp1 && pop->p[r] != temp2, pop->p[r] == temp3){
                    pop->p[r] = temp2;
                    break;
                }
            }
            if(j == 0)
            viz = *pop;

            //Depois de avaliar, se for de melhor qualidade max = pop atual
            pop = eval_one(pop, d, mat);
            if(pop->fitness[d.subconj] > viz.fitness[d.subconj]){
                viz = *pop;
            }

            if (pop->fitness[d.subconj] > max.fitness[d.subconj]){
                max = *pop;
                g = i+1;
            }

            //Mostrar vizinho atual
            /*printf("\nVizinho %d:  ", j+1);
            for(int i = 0; i < d.elements; i++)
                printf("%d ", pop->p[i]);
            printf("\nQualidade:  %d", pop->fitness[d.subconj]);*/
        }

        //Mostrar melhor vizinho do grupo
        /*printf("\nMelhor vizinho:");
        for(int i = 0; i < d.elements; i++)
            printf("%d ", viz.p[i]);
        printf("\nQualidade: %d\n", viz.fitness[d.subconj]);*/
    } 
    return max;
}

//Função de Hill-climbing
void trepacolinas(pchrom pop, struct info d, int mat[][3]){
printf("\n------------------------------------------------------------------\n");
printf("          /\\                                                                   \n");
printf("         /**\\                    Hill-Climbing                                 \n");
printf("        /****\\   /\\                                                           \n");
printf("       /      \\ /**\\                                                          \n");
printf("      /  /\\    /    \\        /\\    /\\  /\\       /\\          /\\           \n");
printf("     /  /  \\  /      \\      /  \\/\\/  \\/  \\  /\\ /  \\/\\  /\\  /  \\      \n");
printf("    /  /    \\/ /\\     \\    /    \\ \\  /    \\/  /   /  \\/  \\/    \\       \n");
printf("   /  /      \\/  \\/\\   \\  /      \\  /     /                     \\         \n");
printf("__/__/________\\__/__\\___\\/________\\______________________________\\         \n");
    
    //Verificar se a matriz é inválida. Caso seja, muda coisas dela para funcionar com um número de subconjuntos diferente
    if(d.subconj == 0 || d.N == d.elements || d.N != (float)d.elements/d.subconj || d.subconj == d.elements){
        printf("O número de subconjuntos é incompatível com o número de elementos\n");
        printf("Visto isto, vai-se subtraír ou incrementar um número até ser possível usar\n");
        if(d.subconj <= 0 || d.N == d.elements)
            do{
                d.subconj++;
                d.N = d.elements/d.subconj;
            }while(d.N != (float)d.elements/d.subconj || d.subconj <= 1);

        else
        do{
            d.subconj--;
            d.N = d.elements/d.subconj;
        }while(d.N != (float)d.elements/d.subconj || d.subconj == d.elements);

        printf("Novo numero de subconjuntos: %d. N = %d\n", d.subconj, d.N);
        rearange(pop, d);
    }
    
    //Vai avaliar a pop e criar uma nova estrutura para guardar o melhor grupo
    pop = eval_one(pop, d, mat);
    chrom max = *pop;
    int g = 0;
    printf("\nGrupo inicial: ");
    for(int i = 0; i < d.elements; i++)
        printf("%d ", max.p[i]);
    printf("\nQualidade: %d\n", pop->fitness[d.subconj]);

    max = climb(pop, d, mat);

    //Mostra a qualidade máxima
    printf("\n\nQualidade máxima: %d\nGrupos: ", max.fitness[d.subconj]);
    for(int i = 0; i < d.elements; i++)
       printf("%d ", max.p[i]);
    printf("\n------------------------------------------------------------------\n");
}

//Função de seleção de torneio
void tournament(pchrom pop, struct info d, pchrom parent){
    int i, x1, x2, n;

	// Realiza d.popsize torneios
	for(i=0; i<d.popsize;i++){
		x1 = random_l_h(0, d.popsize-1);
		do
			x2 = random_l_h(0, d.popsize-1);
		while(x1==x2);
        //printf("Filhos: %d %d\n", pop[x1].fitness[d.subconj], pop[x2].fitness[d.subconj]);
		if(pop[x1].fitness[d.subconj] > pop[x2].fitness[d.subconj])	// Problema de maximizacao
			parent[i]=pop[x1];
		else
			parent[i]=pop[x2];
            n=2;
        
        //Probabilidade de ser selecionado o mais pequeno
        if(random_l_h(0, 10) == 1){
            if(pop[x1].fitness[d.subconj] > pop[x2].fitness[d.subconj]) parent[i]=pop[x2];
            else parent[i]=pop[x1];
        }
	}
}

//Função de recombinação crossover para o algorítmo evolutivo
void crossover(pchrom parents, struct info d, pchrom offspring){
    int i, j, point, r = 0;
    
    for (i=0; i<d.popsize; i+=2){
	    if (rands(d.rchance) == 1){
            do{
                chrom offtemp1, offtemp2;
                offtemp1 = offspring[i];
                offtemp2 = offspring[i+1]; 
                r = 0;
                point = random_l_h(2, d.elements-2);
                //printf("\nAqui %d cross: %d\n\n", i, point);
                for (j=0; j<point; j++){
    		        offspring[i].p[j] = parents[i].p[j];
	    		    offspring[i+1].p[j] = parents[i+1].p[j];
    		    }
           		for (j=point; j<d.elements; j++){
    	    		offspring[i].p[j]= parents[i+1].p[j];
        			offspring[i+1].p[j] = parents[i].p[j];
    	    	}
                
                int temp[d.subconj];
                for(int p = 0; p < d.subconj; p++) temp[p] = 0;
                for(int z = 0; z < d.elements; z++){
                    //printf("Z: %d   SUbconj: %d   Temp: %d\n", z, offspring[i].p[z], temp[offspring[i].p[z]-1]);
				    temp[offspring[i].p[z]-1] += 1;
                    if(temp[offspring[i].p[z]-1] > d.N){
                        offspring[i] = offtemp1;
                        offspring[i+1] = offtemp2;
                        r = 0;
                        break;
                    }
                }
                
            }while(r==1);
        }
		else
		{
			offspring[i] = parents[i];
			offspring[i+1] = parents[i+1];
		}
	}
}


//Função de mutação para o algorítmo evolutivo
void mutation(pchrom offspring,struct info d){
    int i, j, r, x, temp;

	for (i=0; i<d.popsize; i++)
		for (j=0; j<d.elements; j++)
			if (rands(d.mchance) == 1){
                
                /*printf("Pop, elem: %d %d\n", i, j);
                printf("\nHere: %d:      ", i);
                for(int z = 0; z < d.elements; z++)
                        printf("%d ", offspring[i].p[z]);
                printf("\n");*/                

                do{
                    r = rands(d.subconj);
                }while(offspring[i].p[j] == r);
                
                //printf("P: %d  Subconj: %d", offspring[i].p[j], r);                    
                temp = offspring[i].p[j];
                offspring[i].p[j] = r;
                
                do{
                    r = rands(d.elements);
                }while(offspring[i].p[r] != offspring[i].p[j] && r != j);

                offspring[i].p[r] = temp;
                
                /*printf("\nR: %d  Temp: %d", r, temp);
                printf("\nHere: %d:      ", i);
                for(int z = 0; z < d.elements; z++)
                        printf("%d ", offspring[i].p[z]);
                printf("\n");*/
            }
}

//Função do algorítmo evolutivo
void evolutivo(pchrom pop, struct info d, int mat[][3]){
printf("\n------------------------------------------------------------------\n");
printf("───────────────▄▄───▐█  \n");
printf("───▄▄▄───▄██▄──█▀───█─▄ \n");
printf("─▄██▀█▌─██▄▄──▐█▀▄─▐█▀  \n");
printf("▐█▀▀▌───▄▀▌─▌─█─▌──▌─▌  \n");
printf("▌▀▄─▐──▀▄─▐▄─▐▄▐▄─▐▄─▐▄ \n");

    //Verificar se a matriz é inválida. Caso seja, muda coisas dela para funcionar com um número de subconjuntos diferente
    if(d.subconj == 0 || d.N == d.elements || d.N != (float)d.elements/d.subconj || d.subconj == d.elements){
        printf("O número de subconjuntos é incompatível com o número de elementos\n");
        printf("Visto isto, vai-se subtraír ou incrementar um número até ser possível usar\n");
        if(d.subconj <= 0 || d.N == d.elements)
            do{
                d.subconj++;
                d.N = d.elements/d.subconj;
            }while(d.N != (float)d.elements/d.subconj || d.subconj <= 1);

        else
        do{
            d.subconj--;
            d.N = d.elements/d.subconj;
        }while(d.N != (float)d.elements/d.subconj || d.subconj == d.elements);

        printf("Novo numero de subconjuntos: %d. N = %d\n", d.subconj, d.N);
        rearange(pop, d);
    }
    
    //Vai avaliar a pop e criar uma nova estrutura para guardar o melhor grupo
    pchrom parents;
    chrom best;

    //A d.popsize
    d.popsize = 10;
    pop = init_pop(pop, d);
    /*for(int i = 0; i < d.popsize; i++){
        printf("\nGrupo %d:  ", i);
        for(int j = 0; j < d.elements; j++)
            printf("%d ", pop[i].p[j]);
    }*/

    best = pop[0];
    evaluate(pop, d, mat);
    /*for(int x = 0; x < d.popsize; x++)
        printf("Qualidade: %d\n\n", pop[x].fitness[d.subconj]);
    */
    parents = malloc(sizeof(chrom)*d.popsize);
    d.rchance = 10;
    d.mchance = 50;

    for(int i=0; i<20; i++){
        tournament(pop, d, parents);
        /*for(int x = 0; x < d.popsize; x++){
            printf("\nParents: %d:      ", x);
        
            for(int z = 0; z < d.elements; z++){
                printf("%d ", parents[x].p[z]);
            }
            printf("\nOffspring: %d:    ", x);
        
            for(int z = 0; z < d.elements; z++){
                printf("%d ", pop[x].p[z]);
            }
        }printf("\n\n\n\n");
        */
        crossover(parents, d, pop);
        mutation(pop, d);

        /*for(int x = 0; x < d.popsize; x++){
            printf("\nParents: %d:      ", x);
        
            for(int z = 0; z < d.elements; z++){
                printf("%d ", parents[x].p[z]);
            }
            printf("\nOffspring: %d:    ", x);

            for(int z = 0; z < d.elements; z++){
                printf("%d ", pop[x].p[z]);
            }
        }
        */
       evaluate(pop, d, mat);
       best = bestpop(best, pop, d);
    }

    printf("Melhor solução encontrada: ");
    for(int z = 0; z < d.elements; z++)
                printf("%d ", best.p[z]);
    printf("\nFitness: %d\n", best.fitness[d.subconj]);
            
}

//Função do método híbrido, que contem o algorítmo evolutivo junto com o Hill-climbing
void hibrido(pchrom pop, struct info d, int mat[][3]){
printf("\n------------------------------------------------------------------\n");
printf("           P                                                                    \n");
printf("          /\\                                                                   \n");
printf("         /**\\     P               Evolutivo + Hill-Climbing                    \n");
printf("        /****\\   /\\                                                           \n");
printf("       /  P   \\ /**\\          P     P   P        P           P                \n");
printf("      /  /\\    /    \\        /\\    /\\  /\\       /\\          /\\           \n");
printf("     /  /  \\  /      \\      /  \\/\\/  \\/  \\  /\\ /  \\/\\  /\\  /  \\      \n");
printf("    /  /    \\/ /\\     \\    /    \\ \\  /    \\/  /   /  \\/  \\/    \\       \n");
printf("   /  /      \\/  \\/\\   \\  /      \\  /     /                     \\         \n");
printf("__/__/________\\__/__\\___\\/________\\______________________________\\         \n");

    //Verificar se a matriz é inválida. Caso seja, muda coisas dela para funcionar com um número de subconjuntos diferente
    if(d.subconj == 0 || d.N == d.elements || d.N != (float)d.elements/d.subconj || d.subconj == d.elements){
        printf("O número de subconjuntos é incompatível com o número de elementos\n");
        printf("Visto isto, vai-se subtraír ou incrementar um número até ser possível usar\n");
        if(d.subconj <= 0 || d.N == d.elements)
            do{
                d.subconj++;
                d.N = d.elements/d.subconj;
            }while(d.N != (float)d.elements/d.subconj || d.subconj <= 1);

        else
        do{
            d.subconj--;
            d.N = d.elements/d.subconj;
        }while(d.N != (float)d.elements/d.subconj || d.subconj == d.elements);

        printf("Novo numero de subconjuntos: %d. N = %d\n", d.subconj, d.N);
        rearange(pop, d);
    }
    
    //Vai avaliar a pop e criar uma nova estrutura para guardar o melhor grupo
    pchrom parents;
    chrom best;

    //A d.popsize
    d.popsize = 200;
    pop = init_pop(pop, d);
    /*for(int i = 0; i < d.popsize; i++){
        printf("\nGrupo %d:  ", i);
        for(int j = 0; j < d.elements; j++)
            printf("%d ", pop[i].p[j]);
    }*/

    best = pop[0];
    evaluate(pop, d, mat);
    /*for(int x = 0; x < d.popsize; x++)
        printf("Qualidade: %d\n\n", pop[x].fitness[d.subconj]);
    */
    parents = malloc(sizeof(chrom)*d.popsize);
    d.rchance = 100;
    d.mchance = 50;

    for(int i=0; i<20; i++){
        tournament(pop, d, parents);
        crossover(parents, d, pop);
        mutation(pop, d);
        for(int c=0; c< d.popsize; c++)
            pop[i] = climb(pop, d, mat);
        evaluate(pop, d, mat);
        best = bestpop(best, pop, d);
    }

    printf("Melhor solução encontrada: ");
    for(int z = 0; z < d.elements; z++)
                printf("%d ", best.p[z]);
    printf("\nFitness: %d\n", best.fitness[d.subconj]);
            
}