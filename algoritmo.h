#define MAX_ELE 100000	// Numero maximo de objectos

// EStrutura para armazenar parametros
struct info
{
    // Numero total de elementos
    int elements;
    // Numero de subgrupos
    int subconj;
    //Maximo de gerações
    int numGenerations;
    //Contar vezes que correu
    int nruns;
    //Elementos por subconjunto
    int N;
    //Tamanho da populacao
    int popsize;
    //Chance de recombinação
    int rchance;
    //Chance de mutacao
    int mchance;
};

// Individuo (solução)
typedef struct individual chrom, *pchrom;

struct individual
{
    // Solução (elementos que vão fazer parte de cada subconjunto)
    int    p[MAX_ELE];
    //Qualidade
    int   fitness[100];
};

chrom climb(pchrom pop, struct info d, int mat[][3]);

void trepacolinas(pchrom pop, struct info d, int mat[][3]);

void tournament(pchrom pop, struct info d, pchrom parent);

void crossover(pchrom parents, struct info d, pchrom offspring);

void mutation(pchrom offspring,struct info d);

void evolutivo(pchrom pop, struct info d, int mat[][3]);

void hibrido(pchrom pop, struct info d, int mat[][3]);
