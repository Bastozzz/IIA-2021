struct info init_data(char *s, int mat[][3]);

pchrom init_pop(pchrom pop, struct info d);

pchrom init_ind(struct info d);

void rearange(pchrom pop, struct info d);

chrom bestpop(chrom best, pchrom pop, struct info d);

void init_rand();

int random_l_h(int min, int max);

float rand_01();

int rands(int s);
