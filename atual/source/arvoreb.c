/**
 * @author Matheus Henrique de Cerqueira Pinto (11911104)
 * @author Pedro Lucas de Moliner de Castro (11795784)
 * @date 2021-06-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#include "arvoreb.h"

#include <stdio.h>
#include <stdbool.h>


const int NULO = -1;


static void _leNoArvoreBporRRN(NoArvoreB *noArvoreB, int RRN, FILE *arvoreB);
static bool _buscaNoArvoreB(int chave, int *posicao, NoArvoreB *noArvoreB);


long long int buscaArvoreB(int RRN, int chave, FILE *arvoreB) {
    if (RRN == NULO)
        return NAO_ENCONTRADO;
    
    NoArvoreB noArvoreB;
    _leNoArvoreBporRRN(&noArvoreB, RRN, arvoreB);

    int posicao;
    if (_buscaNoArvoreB(chave, &posicao, &noArvoreB) == true)
        return noArvoreB.chaves[posicao].PR;

    return buscaArvoreB(noArvoreB.P[posicao], chave, arvoreB);
}


static void _leNoArvoreB(NoArvoreB *noArvoreB, FILE *arvoreB) {
    fread(&noArvoreB->folha, sizeof(char), 1, arvoreB);
    fread(&noArvoreB->nroChavesIndexadas, sizeof(int), 1, arvoreB);
    fread(&noArvoreB->RRNdoNo, sizeof(int), 1, arvoreB);

    for (int i = 0; i < MAX_NUMERO_CHAVES; i++) {
        fread(&noArvoreB->P[i], sizeof(int), 1, arvoreB);
        fread(&noArvoreB->chaves[i].C, sizeof(int), 1, arvoreB);
        fread(&noArvoreB->chaves[i].PR, sizeof(long long int), 1, arvoreB);
    }

    fread(&noArvoreB->P[MAX_NUMERO_CHAVES], sizeof(int), 1, arvoreB);
}

static void _leNoArvoreBporRRN(NoArvoreB *noArvoreB, int RRN, FILE *arvoreB) {
    fseek(arvoreB, (RRN + 1) * TAMANHO_PAGINA, SEEK_SET);
    _leNoArvoreB(noArvoreB, arvoreB);
}

static bool _buscaNoArvoreB(int chave, int *posicao, NoArvoreB *noArvoreB) {
    for (*posicao = 0; *posicao < noArvoreB->nroChavesIndexadas; *posicao++) {
        if (chave < noArvoreB->chaves[*posicao].C)
            return false;

        else if (chave == noArvoreB->chaves[*posicao].C)
            return true;
    }

    return false;
}
