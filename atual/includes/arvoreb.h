/**
 * @author Matheus Henrique de Cerqueira Pinto (11911104)
 * @author Pedro Lucas de Moliner de Castro (11795784)
 * @date 2021-06-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef ARVORE_B
#define ARVORE_B


#include <stdio.h>


#define NAO_ENCONTRADO -1
#define TAMANHO_PAGINA 77
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

typedef enum {
    PROMOCAO,
    SEM_PROMOCAO,
    ERRO
} RetornoInsercao;


long long int buscaArvoreB(int chave, int RRN, FILE *arvoreB);
RetornoInsercao insereArvoreB(ChaveArvoreB chave, int RRN, ChaveArvoreB *chave_promovida,
                              int *filho_promovido, CabecalhoArvoreB *cabecalho, FILE *arvoreB);


#endif // ARVORE_B
