/**
 * @author Matheus Henrique de Cerqueira Pinto (11911104)
 * @author Pedro Lucas de Moliner de Castro (11795784)
 * @date 2021-07-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "linha.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mensagem.h"
#include "util.h"

static const int TAMANHO_CABECALHO_LINHA = 82;

static void _posicionaBinarioCabecalhoLinha(FILE *binario);
static void _imprimeAceitaCartao(const char *aceitaCartao);
static int _comparaLinhas(const void *primeira, const void *segunda);

/**
 *
 * Binário
 * 
 */

// Posiciona o ponteiro do binário no começo do primeiro registro de linha
void posicionaBinarioPrimeiroRegistroLinha(FILE *binario) {
    fseek(binario, TAMANHO_CABECALHO_LINHA, SEEK_SET);
}

/**
 *
 * Cabeçalho da Linha 
 * 
 */

// Lê o cabeçalho de uma Linha a partir de um binário
CabecalhoLinha leCabecalhoLinhaBinario(FILE *binario) {
    _posicionaBinarioCabecalhoLinha(binario);

    CabecalhoLinha cabecalho;

    fread(&cabecalho.status, sizeof(char), 1, binario);
    fread(&cabecalho.byteProxReg, sizeof(long long int), 1, binario);
    fread(&cabecalho.nroRegistros, sizeof(int), 1, binario);
    fread(&cabecalho.nroRegRemovidos, sizeof(int), 1, binario);

    leStringBinario(cabecalho.descreveCodigo, 15, binario);
    leStringBinario(cabecalho.descreveCartao, 13, binario);
    leStringBinario(cabecalho.descreveNome, 13, binario);
    leStringBinario(cabecalho.descreveLinha, 24, binario);

    return cabecalho;
}

// Cria um cabeçalho de linha novo para ordenação baseado no original
CabecalhoLinha criaCabecalhoLinhaOrdenado(CabecalhoLinha original) {
    CabecalhoLinha ordenado;

    ordenado.status = '0';
    ordenado.byteProxReg = TAMANHO_CABECALHO_LINHA;
    ordenado.nroRegistros = 0;
    ordenado.nroRegRemovidos = 0;

    strcpy(ordenado.descreveCodigo, original.descreveCodigo);
    strcpy(ordenado.descreveCartao, original.descreveCartao);
    strcpy(ordenado.descreveNome, original.descreveNome);
    strcpy(ordenado.descreveLinha, original.descreveLinha);

    return ordenado;
}

// Escreve o cabeçalho de uma Linha num binário
void escreveCabecalhoLinhaBinario(CabecalhoLinha cabecalho, FILE *binario) {
    _posicionaBinarioCabecalhoLinha(binario);

    fwrite(&cabecalho.status, sizeof(char), 1, binario);
    fwrite(&cabecalho.byteProxReg, sizeof(long long int), 1, binario);
    fwrite(&cabecalho.nroRegistros, sizeof(int), 1, binario);
    fwrite(&cabecalho.nroRegRemovidos, sizeof(int), 1, binario);

    fwrite(cabecalho.descreveCodigo, sizeof(char), 15, binario);
    fwrite(cabecalho.descreveCartao, sizeof(char), 13, binario);
    fwrite(cabecalho.descreveNome, sizeof(char), 13, binario);
    fwrite(cabecalho.descreveLinha, sizeof(char), 24, binario);
}

/**
 *
 * Linha
 * 
 */

// Lê uma Linha a partir de um binário e retorna se ela foi removida
Linha leLinhaBinario(FILE *binario) {
    Linha linha;

    fread(&linha.removido, sizeof(char), 1, binario);
    fread(&linha.tamanhoRegistro, sizeof(int), 1, binario);

    // Checa por registro removido e retorna se sim
    if (registroFoiRemovido(linha.removido)) {
        fseek(binario, linha.tamanhoRegistro, SEEK_CUR);
        return linha;
    }

    fread(&linha.codLinha, sizeof(int), 1, binario);

    leStringBinario(linha.aceitaCartao, 1, binario);

    fread(&linha.tamanhoNome, sizeof(int), 1, binario);
    leStringBinario(linha.nomeLinha, linha.tamanhoNome, binario);

    fread(&linha.tamanhoCor, sizeof(int), 1, binario);
    leStringBinario(linha.corLinha, linha.tamanhoCor, binario);

    return linha;
}

// Escreve uma Linha num binário
void escreveLinhaBinario(Linha linha, FILE *binario) {
    fwrite(&linha.removido, sizeof(char), 1, binario);
    fwrite(&linha.tamanhoRegistro, sizeof(int), 1, binario);

    fwrite(&linha.codLinha, sizeof(int), 1, binario);
    fwrite(linha.aceitaCartao, sizeof(char), 1, binario);

    fwrite(&linha.tamanhoNome, sizeof(int), 1, binario);
    fwrite(linha.nomeLinha, sizeof(char), linha.tamanhoNome, binario);

    fwrite(&linha.tamanhoCor, sizeof(int), 1, binario);
    fwrite(linha.corLinha, sizeof(char), linha.tamanhoCor, binario);
}

// Imprime uma Linha
void imprimeLinha(Linha linha, CabecalhoLinha cabecalho) {
    printf("%s: ", cabecalho.descreveCodigo);
    imprimeCampoInteiro(linha.codLinha);

    printf("%s: ", cabecalho.descreveNome);
    imprimeCampoString(linha.nomeLinha, linha.tamanhoNome);

    printf("%s: ", cabecalho.descreveLinha);
    imprimeCampoString(linha.corLinha, linha.tamanhoCor);

    printf("%s: ", cabecalho.descreveCartao);
    _imprimeAceitaCartao(linha.aceitaCartao);
}

/**
 *
 * Linhas
 * 
 */

// Lê um vetor de linhas válidas a partir de um binário
void leLinhasBinario(Linha *linhas, int numero, FILE *binario) {
    for (int i = 0, j = 0; i < numero; i++) {
        Linha temporario = leLinhaBinario(binario);

        // Adiciona ao vetor se não removido
        if (!registroFoiRemovido(temporario.removido)) {
            linhas[j++] = temporario;
        }
    }
}

// Escreve um vetor de linhas em um binário
void escreveLinhasBinario(Linha *linhas, int numero, FILE *binario) {
    for (int i = 0; i < numero; i++) {
        escreveLinhaBinario(linhas[i], binario);
    }
}

// Ordena um vetor de linhas por codLinha
void ordenaLinhas(Linha *linhas, int numero) {
    qsort(linhas, numero, sizeof(Linha), _comparaLinhas);
}

/**
 *
 * Auxiliares 
 * 
 */

static void _posicionaBinarioCabecalhoLinha(FILE *binario) {
    fseek(binario, 0, SEEK_SET);
}

static void _imprimeAceitaCartao(const char *aceitaCartao) {
    if (aceitaCartao[0] == '\0') {
        printf("%s\n", CAMPO_NULO);
        return;
    }

    switch (aceitaCartao[0]) {
        case 'S':
            printf("PAGAMENTO SOMENTE COM CARTAO SEM PRESENCA DE COBRADOR\n");
            break;

        case 'N':
            printf("PAGAMENTO EM CARTAO E DINHEIRO\n");
            break;

        case 'F':
            printf("PAGAMENTO EM CARTAO SOMENTE NO FINAL DE SEMANA\n");
            break;
    }
}

static int _comparaLinhas(const void *primeira, const void *segunda) {
    return ((Linha *)primeira)->codLinha - ((Linha *)segunda)->codLinha;
}
