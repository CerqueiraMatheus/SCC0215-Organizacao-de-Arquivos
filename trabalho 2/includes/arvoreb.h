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

#include <stdbool.h>
#include <stdio.h>

// Constantes para manipulação
#define NAO_ENCONTRADO -1
#define TAMANHO_PAGINA 77
#define MAX_NUMERO_CHAVES 4

// Struct para o cabeçalho da Árvore B
typedef struct {
    char status;
    int noRaiz;
    int RRNproxNo;
} CabecalhoArvoreB;

// Struct para a chave da Árvore B
typedef struct {
    int C;
    long long int PR;
} ChaveArvoreB;

// Struct para o nó da Árvore B
typedef struct {
    char folha;
    int nroChavesIndexadas;
    int RRNdoNo;
    ChaveArvoreB chaves[MAX_NUMERO_CHAVES];
    int P[MAX_NUMERO_CHAVES + 1];
} NoArvoreB;

// Funções para o cabeçalho da Árvore B
void criaCabecalhoArvoreB(CabecalhoArvoreB *cabecalho);
void leCabecalhoArvoreB(CabecalhoArvoreB *cabecalho, FILE *arvoreB);
void escreveCabecalhoArvoreB(CabecalhoArvoreB cabecalho, FILE *arvoreB);

// Funções para a Árvore B
long long int buscaArvoreB(int chave, int RRN, FILE *arvoreB);
bool insereArvoreB(ChaveArvoreB chave, CabecalhoArvoreB *cabecalho, FILE *arvoreB);

#endif  // ARVORE_B
