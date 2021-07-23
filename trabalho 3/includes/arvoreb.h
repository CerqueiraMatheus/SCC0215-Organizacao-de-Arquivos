/**
 * @author Matheus Henrique de Cerqueira Pinto (11911104)
 * @author Pedro Lucas de Moliner de Castro (11795784)
 * @date 2021-07-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef ARVORE_B
#define ARVORE_B

#include <stdbool.h>
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

/** Cabeçalho da Árvore-B **/
CabecalhoArvoreB leCabecalhoArvoreB(FILE *arvoreB);

/** Árvore-B **/
long long int buscaArvoreB(int chave, int RRN, FILE *arvoreB);

#endif  // ARVORE_B
