// Pedro Lucas de Moliner de Castro - 11795784
// Matheus Henrique de Cerqueira Pinto - 11911104

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "linha.h"


// Cabeçalho Linha

int leCabecalhoLinhaCsv(CabecalhoLinha *cabecalho, FILE *csv) {
    // Lê string do CSV
    char *string = leStringCsv(csv);
    if (string == NULL)
        return EOF;
    char *leitor = string;

    cabecalho->status = '0';
    cabecalho->byteProxReg = 0;
    cabecalho->nroRegistros = 0;
    cabecalho->nroRegRemovidos = 0;

    strcpy(cabecalho->descreveCodigo, strsep(&leitor, ","));
    strcpy(cabecalho->descreveCartao, strsep(&leitor, ","));
    strcpy(cabecalho->descreveNome, strsep(&leitor, ","));
    strcpy(cabecalho->descreveLinha, strsep(&leitor, ","));

    free(string);
    return 0;
}

void escreveCabecalhoLinhaBinario(CabecalhoLinha *cabecalho, FILE *binario) {
    fwrite(&cabecalho->status, sizeof(char), 1, binario);
    fwrite(&cabecalho->byteProxReg, sizeof(long long int), 1, binario);
    fwrite(&cabecalho->nroRegistros, sizeof(int), 1, binario);
    fwrite(&cabecalho->nroRegRemovidos, sizeof(int), 1, binario);

    fwrite(cabecalho->descreveCodigo, sizeof(char), 15, binario);
    fwrite(cabecalho->descreveCartao, sizeof(char), 13, binario);
    fwrite(cabecalho->descreveNome, sizeof(char), 13, binario);
    fwrite(cabecalho->descreveLinha, sizeof(char), 24, binario);
}


// Linha

int leLinhaCsv(Linha *linha, FILE *csv) {
    // Lê string do CSV
    char *string = leStringCsv(csv);
    if (string == NULL)
        return EOF;
    char *leitor = string;

    // Checa por registro removido
    if (leitor[0] == '*') {
        linha->removido = '0';
        leitor++;
    } else {
        linha->removido = '1';
    }

    linha->codLinha = atoi(strsep(&leitor, ","));
    linha->aceitaCartao = stringParaCampoString(strsep(&leitor, ","))[0];

    strcpy(linha->nomeLinha, stringParaCampoString(strsep(&leitor, ",")));
    strcpy(linha->corLinha, stringParaCampoString(strsep(&leitor, ",")));

    linha->tamanhoNome = strlen(linha->nomeLinha);
    linha->tamanhoCor = strlen(linha->corLinha);
    linha->tamanhoRegistro = 13 + linha->tamanhoNome + linha->tamanhoCor;

    free(string);
    return 0;
}

void escreveLinhaBinario(Linha *linha, FILE *binario) {
    fwrite(&linha->removido, sizeof(char), 1, binario);
    fwrite(&linha->tamanhoRegistro, sizeof(int), 1, binario);

    fwrite(&linha->codLinha, sizeof(int), 1, binario);
    fwrite(&linha->aceitaCartao, sizeof(char), 1, binario);

    fwrite(&linha->tamanhoNome, sizeof(int), 1, binario);
    fwrite(linha->nomeLinha, sizeof(char), linha->tamanhoNome, binario);

    fwrite(&linha->tamanhoCor, sizeof(int), 1, binario);
    fwrite(linha->corLinha, sizeof(char), linha->tamanhoCor, binario);
}
