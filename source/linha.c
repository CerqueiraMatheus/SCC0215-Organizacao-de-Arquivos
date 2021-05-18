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

}

void escreveLinhaBinario(Linha *linha, FILE *binario) {

}
