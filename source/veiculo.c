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
