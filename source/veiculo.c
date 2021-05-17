// Pedro Lucas de Moliner de Castro - 11795784
// Matheus Henrique de Cerqueira Pinto - 11911104

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "veiculo.h"
#include "util.h"


int leCabecalhoVeiculoCsv(CabecalhoVeiculo *cabecalho, FILE *csv) {
    // Inicializa valores do cabeçalho
    cabecalho->status = '0';
    cabecalho->byteProxReg = 0;
    cabecalho->nroRegistros = 0;
    cabecalho->nroRegRemovidos = 0;
    
    // Lê linha do CSV em uma string
    char *string = leStringCsv(csv);
    if (string == NULL)
        return EOF;
    char *leitor = string;

    // Lê strings descritoras
    strcpy(cabecalho->descrevePrefixo, strsep(&leitor, ","));
    strcpy(cabecalho->descreveData, strsep(&leitor, ","));
    strcpy(cabecalho->descreveLugares, strsep(&leitor, ","));
    strcpy(cabecalho->descreveLinha, strsep(&leitor, ","));
    strcpy(cabecalho->descreveModelo, strsep(&leitor, ","));
    strcpy(cabecalho->descreveCategoria, strsep(&leitor, ","));

    free(string);
    return 0;
}

int leVeiculoCsv(Veiculo *veiculo, FILE *csv) {
    // Lê linha do CSV em uma string
    char *string = leStringCsv(csv);
    if (string == NULL)
        return EOF;
    char *leitor = string;

    // Checa se o registro foi removido
    if (leitor[0] == '*') {
        veiculo->removido = '0';
        leitor++;
    } else {
        veiculo->removido = '1';
    }

    // Lê campos não nulos
    strcpy(veiculo->prefixo, strsep(&leitor, ","));

    // Lê campos possivelmente nulos
    strcpy(veiculo->data, trataNuloString(strsep(&leitor, ",")));
    veiculo->quantidadeLugares = trataNuloInteiro(strsep(&leitor, ","));
    veiculo->codLinha = trataNuloInteiro(strsep(&leitor, ","));
    strcpy(veiculo->modelo, trataNuloString(strsep(&leitor, ",")));
    strcpy(veiculo->categoria, trataNuloString(strsep(&leitor, ",")));
    
    // Calcula tamanho das strings variáveis
    veiculo->tamanhoModelo = strlen(veiculo->modelo);
    veiculo->tamanhoCategoria = strlen(veiculo->categoria);
    
    // Calcula tamanho final do registro
    veiculo->tamanhoRegistro = 31 + veiculo->tamanhoModelo + veiculo->tamanhoCategoria;

    free(string);
    return 0;
}
