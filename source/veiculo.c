// Pedro Lucas de Moliner de Castro - 11795784
// Matheus Henrique de Cerqueira Pinto - 11911104

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "veiculo.h"


// Cabeçalho Veículo

int leCabecalhoVeiculoCsv(CabecalhoVeiculo *cabecalho, FILE *csv) {
    // Lê linha do CSV
    char *string = leStringCsv(csv);
    if (string == NULL)
        return EOF;
    char *leitor = string;

    cabecalho->status = '0';
    cabecalho->byteProxReg = 0;
    cabecalho->nroRegistros = 0;
    cabecalho->nroRegRemovidos = 0;

    strcpy(cabecalho->descrevePrefixo, strsep(&leitor, ","));
    strcpy(cabecalho->descreveData, strsep(&leitor, ","));
    strcpy(cabecalho->descreveLugares, strsep(&leitor, ","));
    strcpy(cabecalho->descreveLinha, strsep(&leitor, ","));
    strcpy(cabecalho->descreveModelo, strsep(&leitor, ","));
    strcpy(cabecalho->descreveCategoria, strsep(&leitor, ","));

    free(string);
    return 0;
}

void escreveCabecalhoVeiculoBinario(CabecalhoVeiculo *cabecalho, FILE *binario) {
    fwrite(&cabecalho->status, sizeof(char), 1, binario);
    fwrite(&cabecalho->byteProxReg, sizeof(long long int), 1, binario);
    fwrite(&cabecalho->nroRegistros, sizeof(int), 1, binario);
    fwrite(&cabecalho->nroRegRemovidos, sizeof(int), 1, binario);

    fwrite(cabecalho->descrevePrefixo, sizeof(char), 18, binario);
    fwrite(cabecalho->descreveData, sizeof(char), 35, binario);
    fwrite(cabecalho->descreveLugares, sizeof(char), 42, binario);
    fwrite(cabecalho->descreveLinha, sizeof(char), 26, binario);
    fwrite(cabecalho->descreveModelo, sizeof(char), 17, binario);
    fwrite(cabecalho->descreveCategoria, sizeof(char), 20, binario);
}


// Veículo

int leVeiculoCsv(Veiculo *veiculo, FILE *csv) {
    // Lê linha do CSV
    char *string = leStringCsv(csv);
    if (string == NULL)
        return EOF;
    char *leitor = string;

    // Checa por registro removido
    if (leitor[0] == '*') {
        veiculo->removido = '0';
        leitor++;
    } else {
        veiculo->removido = '1';
    }

    strcpy(veiculo->prefixo, strsep(&leitor, ","));
    strcpy(veiculo->data, trataNuloString(strsep(&leitor, ",")));
    veiculo->quantidadeLugares = trataNuloInteiro(strsep(&leitor, ","));
    veiculo->codLinha = trataNuloInteiro(strsep(&leitor, ","));
    strcpy(veiculo->modelo, trataNuloString(strsep(&leitor, ",")));
    strcpy(veiculo->categoria, trataNuloString(strsep(&leitor, ",")));
    
    veiculo->tamanhoModelo = strlen(veiculo->modelo);
    veiculo->tamanhoCategoria = strlen(veiculo->categoria);
    veiculo->tamanhoRegistro = 31 + veiculo->tamanhoModelo + veiculo->tamanhoCategoria;

    free(string);
    return 0;
}

void escreveVeiculoBinario(Veiculo *veiculo, FILE *binario) {
    fwrite(&veiculo->removido, sizeof(char), 1, binario);
    fwrite(&veiculo->tamanhoRegistro, sizeof(int), 1, binario);
    fwrite(veiculo->prefixo, sizeof(char), 5, binario);

    // Escreve o campo data possivelmente nulo
    if (strlen(veiculo->data) == 0)
        escreveStringNuloBinario(10, binario);
    else
        fwrite(veiculo->data, sizeof(char), 10, binario);

    fwrite(&veiculo->quantidadeLugares, sizeof(int), 1, binario);
    fwrite(&veiculo->codLinha, sizeof(int), 1, binario);

    fwrite(&veiculo->tamanhoModelo, sizeof(int), 1, binario);
    fwrite(veiculo->modelo, sizeof(char), veiculo->tamanhoModelo, binario);
    fwrite(&veiculo->tamanhoCategoria, sizeof(int), 1, binario);
    fwrite(veiculo->categoria, sizeof(char), veiculo->tamanhoCategoria, binario);
}
