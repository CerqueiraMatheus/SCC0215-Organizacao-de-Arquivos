/**
 * @author Matheus Henrique de Cerqueira Pinto (11911104)
 * @author Pedro Lucas de Moliner de Castro (11795784)
 * @date 2021-05-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "linha.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "util.h"

/**
 *
 * Cabeçalho da Linha 
 * 
 */

// Lê o cabeçalho de uma Linha a partir de um binário
void leCabecalhoLinhaBinario(CabecalhoLinha *cabecalhoLinha, FILE *binario) {
    fread(&cabecalhoLinha->status, sizeof(char), 1, binario);
    fread(&cabecalhoLinha->byteProxReg, sizeof(long long int), 1, binario);
    fread(&cabecalhoLinha->nroRegistros, sizeof(int), 1, binario);
    fread(&cabecalhoLinha->nroRegRemovidos, sizeof(int), 1, binario);

    leStringBinario(cabecalhoLinha->descreveCodigo, 15, binario);
    leStringBinario(cabecalhoLinha->descreveCartao, 13, binario);
    leStringBinario(cabecalhoLinha->descreveNome, 13, binario);
    leStringBinario(cabecalhoLinha->descreveLinha, 24, binario);
}

// Escreve o cabeçalho de uma Linha num binário
void escreveCabecalhoLinhaBinario(CabecalhoLinha *cabecalhoLinha, FILE *binario) {
    fwrite(&cabecalhoLinha->status, sizeof(char), 1, binario);
    fwrite(&cabecalhoLinha->byteProxReg, sizeof(long long int), 1, binario);
    fwrite(&cabecalhoLinha->nroRegistros, sizeof(int), 1, binario);
    fwrite(&cabecalhoLinha->nroRegRemovidos, sizeof(int), 1, binario);

    fwrite(cabecalhoLinha->descreveCodigo, sizeof(char), 15, binario);
    fwrite(cabecalhoLinha->descreveCartao, sizeof(char), 13, binario);
    fwrite(cabecalhoLinha->descreveNome, sizeof(char), 13, binario);
    fwrite(cabecalhoLinha->descreveLinha, sizeof(char), 24, binario);
}

/**
 *
 * Corpo da Linha 
 * 
 */

// Lê uma Linha a partir de um binário e retorna se ela foi removida
bool leLinhaBinario(Linha *linha, FILE *binario) {
    fread(&linha->removido, sizeof(char), 1, binario);
    fread(&linha->tamanhoRegistro, sizeof(int), 1, binario);

    // Checa por registro removido e retorna se sim
    if (registroFoiRemovido(linha->removido))
        return false;

    fread(&linha->codLinha, sizeof(int), 1, binario);

    leStringBinario(linha->aceitaCartao, 1, binario);

    fread(&linha->tamanhoNome, sizeof(int), 1, binario);
    leStringBinario(linha->nomeLinha, linha->tamanhoNome, binario);

    fread(&linha->tamanhoCor, sizeof(int), 1, binario);
    leStringBinario(linha->corLinha, linha->tamanhoCor, binario);

    return true;
}

// Lê uma Linha a partir da entrada padrão
void leLinhaEntrada(Linha *linha) {
    linha->removido = '1';

    linha->codLinha = leInteiroEntrada();

    scan_quote_string(linha->aceitaCartao);
    scan_quote_string(linha->nomeLinha);
    scan_quote_string(linha->corLinha);

    linha->tamanhoNome = strlen(linha->nomeLinha);
    linha->tamanhoCor = strlen(linha->corLinha);
    linha->tamanhoRegistro = 13 + linha->tamanhoNome + linha->tamanhoCor;
}

// Escreve uma Linha num binário
void escreveLinhaBinario(Linha *linha, FILE *binario) {
    fwrite(&linha->removido, sizeof(char), 1, binario);
    fwrite(&linha->tamanhoRegistro, sizeof(int), 1, binario);

    fwrite(&linha->codLinha, sizeof(int), 1, binario);
    fwrite(linha->aceitaCartao, sizeof(char), 1, binario);

    fwrite(&linha->tamanhoNome, sizeof(int), 1, binario);
    fwrite(linha->nomeLinha, sizeof(char), linha->tamanhoNome, binario);

    fwrite(&linha->tamanhoCor, sizeof(int), 1, binario);
    fwrite(linha->corLinha, sizeof(char), linha->tamanhoCor, binario);
}

// Imprime uma Linha
void imprimeLinha(CabecalhoLinha *cabecalhoLinha, Linha *linha) {
    printf("%s: ", cabecalhoLinha->descreveCodigo);
    imprimeCampoInteiro(linha->codLinha);

    printf("%s: ", cabecalhoLinha->descreveNome);
    imprimeCampoString(linha->nomeLinha, linha->tamanhoNome);

    printf("%s: ", cabecalhoLinha->descreveLinha);
    imprimeCampoString(linha->corLinha, linha->tamanhoCor);

    printf("%s: ", cabecalhoLinha->descreveCartao);
    imprimeAceitaCartao(linha->aceitaCartao);

    printf("\n");
}
