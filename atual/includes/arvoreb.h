#ifndef ARVORE_B
#define ARVORE_B


#define MAX_NUMERO_CHAVES 4


typedef struct {
    char status;
    int noRaiz;
    int RRNproxNo;
} CabecalhoArvoreB;

typedef struct {
    int C;
    long long int PR;
} ChaveArvoreB;

typedef struct {
    char folha;
    int nroChavesIndexadas;
    int RRNdoNo;
    ChaveArvoreB chaves[MAX_NUMERO_CHAVES];
    int P[MAX_NUMERO_CHAVES + 1];
} NoArvoreB;


#endif // ARVORE_B