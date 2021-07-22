/**
 * @author Matheus Henrique de Cerqueira Pinto (11911104)
 * @author Pedro Lucas de Moliner de Castro (11795784)
 * @date 2021-07-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "arvoreb.h"

#include <stdbool.h>
#include <stdio.h>

#include "util.h"

const int NULO = -1;

/**
 *
 * Cabeçalho da Árvore B
 * 
 */

// Cria um cabeçalho da Árvore B por referência
void criaCabecalhoArvoreB(CabecalhoArvoreB *cabecalho) {
    cabecalho->status = '0';
    cabecalho->noRaiz = NULO;
    cabecalho->RRNproxNo = 0;
}

// Lê um cabeçalho da Árvore B por referência a partir de um arquivo
void leCabecalhoArvoreB(CabecalhoArvoreB *cabecalho, FILE *arvoreB) {
    // Posiciona o ponteiro no cabeçalho
    fseek(arvoreB, 0, SEEK_SET);

    fread(&cabecalho->status, sizeof(char), 1, arvoreB);
    fread(&cabecalho->noRaiz, sizeof(int), 1, arvoreB);
    fread(&cabecalho->RRNproxNo, sizeof(int), 1, arvoreB);
}

// Escreve o cabeçalho da Árvore B num arquivo
void escreveCabecalhoArvoreB(CabecalhoArvoreB cabecalho, FILE *arvoreB) {
    // Posiciona o ponteiro no cabeçalho
    fseek(arvoreB, 0, SEEK_SET);

    fwrite(&cabecalho.status, sizeof(char), 1, arvoreB);
    fwrite(&cabecalho.noRaiz, sizeof(int), 1, arvoreB);
    fwrite(&cabecalho.RRNproxNo, sizeof(int), 1, arvoreB);

    escreveLixoBinario(68, arvoreB);
}

/**
 *
 * Árvore B
 * 
 */

// Enum para o retorno da inserção na Árvore B
typedef enum {
    PROMOCAO,
    SEM_PROMOCAO,
    ERRO
} _RetornoInsercao;

// Funções auxiliares
static void _criaNoArvoreB(NoArvoreB *no, CabecalhoArvoreB *cabecalho);
static void _leNoArvoreB(NoArvoreB *no, int RRN, FILE *arvoreB);
static void _escreveNoArvoreB(NoArvoreB no, FILE *arvoreB);
static bool _buscaNoArvoreB(int chave, int *posicao, NoArvoreB no);
static void _insereNoArvoreB(ChaveArvoreB chave, int filho, NoArvoreB *no);
static _RetornoInsercao _insereArvoreB(ChaveArvoreB chave, int RRN,
                                       ChaveArvoreB *chave_promovida, int *filho_promovido, CabecalhoArvoreB *cabecalho, FILE *arvoreB);

// Realiza uma busca na Árvore B a partir de uma chave e um RRN numa Árvore B
// Retorna o offset da chave no arquivo de dados (se encontrado) ou -1
long long int buscaArvoreB(int chave, int RRN, FILE *arvoreB) {
    if (RRN == NULO)
        return NAO_ENCONTRADO;

    NoArvoreB no;
    _leNoArvoreB(&no, RRN, arvoreB);

    int posicao;
    if (_buscaNoArvoreB(chave, &posicao, no) == true)
        return no.chaves[posicao].PR;

    return buscaArvoreB(chave, no.P[posicao], arvoreB);
}

// Insere uma chave numa Árvore B com base num cabeçalho de Árvore B
// Retorna true ou false de acordo com o sucesso da operação
bool insereArvoreB(ChaveArvoreB chave, CabecalhoArvoreB *cabecalho, FILE *arvoreB) {
    ChaveArvoreB chave_promovida;
    int filho_promovido;

    // Executa a inserção recursiva
    _RetornoInsercao retorno = _insereArvoreB(chave, cabecalho->noRaiz, &chave_promovida,
                                              &filho_promovido, cabecalho, arvoreB);

    if (retorno == ERRO)
        return false;

    if (retorno == PROMOCAO) {
        // Cria uma nova raiz
        NoArvoreB novaRaiz;
        _criaNoArvoreB(&novaRaiz, cabecalho);

        // Insere a chave promovida na nova raiz
        _insereNoArvoreB(chave_promovida, filho_promovido, &novaRaiz);
        novaRaiz.P[0] = cabecalho->noRaiz;
        cabecalho->noRaiz = novaRaiz.RRNdoNo;

        _escreveNoArvoreB(novaRaiz, arvoreB);
    }

    return true;
}

/**
 *
 * RRN
 * 
 */

// Posiciona o ponteiro numa Árvore B a partir do RRN
static void _posicionaArvoreBRRN(int RRN, FILE *arvoreB) {
    fseek(arvoreB, (RRN + 1) * TAMANHO_PAGINA, SEEK_SET);
}

/**
 *
 * Nó da Árvore B
 * 
 */

// "Limpa" um nó da Árvore B por referência
static void _limpaNoArvoreB(NoArvoreB *no) {
    no->folha = '1';
    no->nroChavesIndexadas = 0;

    for (int i = 0; i < MAX_NUMERO_CHAVES; i++) {
        no->P[i] = NULO;
        no->chaves[i].C = NULO;
        no->chaves[i].PR = NULO;
    }
    no->P[MAX_NUMERO_CHAVES] = NULO;
}

// Cria um nó da Árvore B por referência a partir de um cabeçalho da Árvore B
static void _criaNoArvoreB(NoArvoreB *no, CabecalhoArvoreB *cabecalho) {
    no->RRNdoNo = (cabecalho->RRNproxNo)++;
    _limpaNoArvoreB(no);
}

// Lê um nó da Árvore B por referência a partir de um RRN e uma Árvore B
static void _leNoArvoreB(NoArvoreB *no, int RRN, FILE *arvoreB) {
    _posicionaArvoreBRRN(RRN, arvoreB);

    fread(&no->folha, sizeof(char), 1, arvoreB);
    fread(&no->nroChavesIndexadas, sizeof(int), 1, arvoreB);
    fread(&no->RRNdoNo, sizeof(int), 1, arvoreB);

    for (int i = 0; i < MAX_NUMERO_CHAVES; i++) {
        fread(&no->P[i], sizeof(int), 1, arvoreB);
        fread(&no->chaves[i].C, sizeof(int), 1, arvoreB);
        fread(&no->chaves[i].PR, sizeof(long long int), 1, arvoreB);
    }
    fread(&no->P[MAX_NUMERO_CHAVES], sizeof(int), 1, arvoreB);
}

// Escreve um nó na Árvore B
static void _escreveNoArvoreB(NoArvoreB no, FILE *arvoreB) {
    _posicionaArvoreBRRN(no.RRNdoNo, arvoreB);

    fwrite(&no.folha, sizeof(char), 1, arvoreB);
    fwrite(&no.nroChavesIndexadas, sizeof(int), 1, arvoreB);
    fwrite(&no.RRNdoNo, sizeof(int), 1, arvoreB);

    for (int i = 0; i < MAX_NUMERO_CHAVES; i++) {
        fwrite(&no.P[i], sizeof(int), 1, arvoreB);
        fwrite(&no.chaves[i].C, sizeof(int), 1, arvoreB);
        fwrite(&no.chaves[i].PR, sizeof(long long int), 1, arvoreB);
    }
    fwrite(&no.P[MAX_NUMERO_CHAVES], sizeof(int), 1, arvoreB);
}

/**
 *
 * Busca
 * 
 */

// Busca uma chave na Árvore B a partir de uma posição e um nó
// Retorna true ou false dependendo do resultado
static bool _buscaNoArvoreB(int chave, int *posicao, NoArvoreB no) {
    for (*posicao = 0; *posicao < no.nroChavesIndexadas; (*posicao)++) {
        if (chave < no.chaves[*posicao].C)
            return false;

        else if (chave == no.chaves[*posicao].C)
            return true;
    }

    return false;
}

/**
 *
 * Particionamento
 * 
 */

// Struct para um nó temporário da Árvore B
typedef struct {
    ChaveArvoreB chaves[MAX_NUMERO_CHAVES + 1];
    int filhos[MAX_NUMERO_CHAVES + 2];
} _NoTemporario;

// Cria um nó temporário passado por referência a partir de um nó, uma chave e um filho
static void _criaNoTemporario(_NoTemporario *temporario, NoArvoreB no,
                              ChaveArvoreB chave, int filho) {
    temporario->filhos[0] = no.P[0];
    int posicao;

    // Copia chaves e filhos anteriores a nova chave
    for (posicao = 0; posicao < MAX_NUMERO_CHAVES && no.chaves[posicao].C < chave.C; posicao++) {
        temporario->chaves[posicao] = no.chaves[posicao];
        temporario->filhos[posicao + 1] = no.P[posicao + 1];
    }

    // Insere a nova chave
    temporario->chaves[posicao] = chave;
    temporario->filhos[posicao + 1] = filho;

    // Copia chaves e filhos posteriores a nova chave
    for (posicao += 1; posicao <= MAX_NUMERO_CHAVES; posicao++) {
        temporario->chaves[posicao] = no.chaves[posicao - 1];
        temporario->filhos[posicao + 1] = no.P[posicao];
    }
}

// Copia um nó temporário a partir de um nó passado por referência e o indicador de início
static void _copiaNoTemporario(NoArvoreB *no, int inicio, _NoTemporario temporario) {
    no->P[0] = temporario.filhos[inicio];
    no->folha = no->P[0] == NULO ? '1' : '0';

    for (int i = 0; i < MAX_NUMERO_CHAVES / 2; i++)
        _insereNoArvoreB(temporario.chaves[inicio + i], temporario.filhos[inicio + 1 + i], no);
}

// Particiona um nó da Árvore B
static void _particionaNoArvoreB(ChaveArvoreB chave, int filho, NoArvoreB *no,
                                 ChaveArvoreB *chave_promovida, int *filho_promovido,
                                 NoArvoreB *novoNo, CabecalhoArvoreB *cabecalho) {
    _NoTemporario temporario;
    _criaNoTemporario(&temporario, *no, chave, filho);

    _criaNoArvoreB(novoNo, cabecalho);

    // Seleciona chave e filho promovidos
    *chave_promovida = temporario.chaves[MAX_NUMERO_CHAVES / 2];
    *filho_promovido = novoNo->RRNdoNo;

    _limpaNoArvoreB(no);
    _copiaNoTemporario(no, 0, temporario);
    _copiaNoTemporario(novoNo, MAX_NUMERO_CHAVES / 2 + 1, temporario);
}

/**
 *
 * Inserção
 * 
 */

// Insere num nó na Árvore B a partir de uma chave, um indicador do filho e um nó
// passado por referência
static void _insereNoArvoreB(ChaveArvoreB chave, int filho, NoArvoreB *no) {
    // Verifica se continua folha
    if (filho != NULO)
        no->folha = '0';

    // Busca a posição da nova chave
    int posicao;
    _buscaNoArvoreB(chave.C, &posicao, *no);

    // Desloca as chaves e filhos posteriores
    for (int i = no->nroChavesIndexadas - 1; i >= posicao; i--) {
        no->chaves[i + 1] = no->chaves[i];
        no->P[i + 2] = no->P[i + 1];
    }

    // Insere a nova chave
    no->chaves[posicao] = chave;
    no->P[posicao + 1] = filho;
    (no->nroChavesIndexadas)++;
}

// Insere na Árvore B
// Retorna um dos itens possíveis para a inserção dependendo da execução
static _RetornoInsercao _insereArvoreB(ChaveArvoreB chave, int RRN,
                                       ChaveArvoreB *chave_promovida, int *filho_promovido, CabecalhoArvoreB *cabecalho, FILE *arvoreB) {
    // Construção a partir da folha
    if (RRN == NULO) {
        *chave_promovida = chave;
        *filho_promovido = NULO;
        return PROMOCAO;
    }

    NoArvoreB no;
    _leNoArvoreB(&no, RRN, arvoreB);

    // Busca pela chave
    int posicao;
    if (_buscaNoArvoreB(chave.C, &posicao, no) == true)
        return ERRO;

    // Recursão até inserir
    _RetornoInsercao retorno = _insereArvoreB(chave, no.P[posicao], chave_promovida, filho_promovido,
                                              cabecalho, arvoreB);

    if (retorno == SEM_PROMOCAO || retorno == ERRO)
        return retorno;

    // Insere promoção sem particionamento
    else if (no.nroChavesIndexadas < MAX_NUMERO_CHAVES) {
        _insereNoArvoreB(*chave_promovida, *filho_promovido, &no);
        _escreveNoArvoreB(no, arvoreB);
        return SEM_PROMOCAO;
    }

    // Insere promoção com particionamento
    else {
        NoArvoreB novoNo;
        _particionaNoArvoreB(*chave_promovida, *filho_promovido, &no,
                             chave_promovida, filho_promovido, &novoNo, cabecalho);
        _escreveNoArvoreB(no, arvoreB);
        _escreveNoArvoreB(novoNo, arvoreB);
        return PROMOCAO;
    }
}
