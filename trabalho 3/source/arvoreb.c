/**
 * @author Matheus Henrique de Cerqueira Pinto (11911104)
 * @author Pedro Lucas de Moliner de Castro (11795784)
 * @date 2021-07-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdbool.h>
#include <stdio.h>

#include "arvoreb.h"
#include "util.h"


static const int NULO = -1;


/**
 *
 * Cabeçalho da Árvore-B
 * 
 */

CabecalhoArvoreB leCabecalhoArvoreB(FILE *arvoreB) {
    // Posiciona o ponteiro no cabeçalho
    fseek(arvoreB, 0, SEEK_SET);

    CabecalhoArvoreB cabecalho;

    fread(&cabecalho.status, sizeof(char), 1, arvoreB);
    fread(&cabecalho.noRaiz, sizeof(int), 1, arvoreB);
    fread(&cabecalho.RRNproxNo, sizeof(int), 1, arvoreB);

    return cabecalho;
}


/**
 *
 * Árvore-B
 * 
 */


static NoArvoreB _leNoArvoreB(int RRN, FILE *arvoreB);
static bool _buscaNoArvoreB(int chave, int *posicao, NoArvoreB no);


long long int buscaArvoreB(int chave, int RRN, FILE *arvoreB) {
    if (RRN == NULO) {
        return NAO_ENCONTRADO;
    }

    NoArvoreB no = _leNoArvoreB(RRN, arvoreB);

    int posicao;
    if (_buscaNoArvoreB(chave, &posicao, no) == true) {
        return no.chaves[posicao].PR;
    }

    return buscaArvoreB(chave, no.P[posicao], arvoreB);
}


/**
 *
 * RRN
 * 
 */

static void _posicionaArvoreBRRN(FILE *arvoreB, int RRN) {
    fseek(arvoreB, (RRN + 1) * TAMANHO_PAGINA, SEEK_SET);
}


/**
 *
 * Nó da Árvore-B
 * 
 */

static NoArvoreB _leNoArvoreB(int RRN, FILE *arvoreB) {
    _posicionaArvoreBRRN(arvoreB, RRN);

    NoArvoreB no;

    fread(&no.folha, sizeof(char), 1, arvoreB);
    fread(&no.nroChavesIndexadas, sizeof(int), 1, arvoreB);
    fread(&no.RRNdoNo, sizeof(int), 1, arvoreB);

    for (int i = 0; i < MAX_NUMERO_CHAVES; i++) {
        fread(&no.P[i], sizeof(int), 1, arvoreB);
        fread(&no.chaves[i].C, sizeof(int), 1, arvoreB);
        fread(&no.chaves[i].PR, sizeof(long long int), 1, arvoreB);
    }
    fread(&no.P[MAX_NUMERO_CHAVES], sizeof(int), 1, arvoreB);

    return no;
}


/**
 *
 * Busca
 * 
 */

static bool _buscaNoArvoreB(int chave, int *posicao, NoArvoreB no) {
    for (*posicao = 0; *posicao < no.nroChavesIndexadas; (*posicao)++) {
        if (chave < no.chaves[*posicao].C) {
            return false;
        }
        else if (chave == no.chaves[*posicao].C) {
            return true;
        }
    }

    return false;
}
