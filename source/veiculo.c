// Pedro Lucas de Moliner de Castro - 11795784
// Matheus Henrique de Cerqueira Pinto - 11911104

#include "veiculo.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

// Cabeçalho Veículo

int leCabecalhoVeiculoCsv(CabecalhoVeiculo *cabecalhoVeiculo, FILE *csv) {
    // Lê string do CSV
    char *string = leStringArquivo(csv);
    if (string == NULL)
        return EOF;
    char *leitor = string;

    cabecalhoVeiculo->status = '0';
    cabecalhoVeiculo->byteProxReg = 0;
    cabecalhoVeiculo->nroRegistros = 0;
    cabecalhoVeiculo->nroRegRemovidos = 0;

    strcpy(cabecalhoVeiculo->descrevePrefixo, stringParaCampoString(strsep(&leitor, ",")));
    strcpy(cabecalhoVeiculo->descreveData, stringParaCampoString(strsep(&leitor, ",")));
    strcpy(cabecalhoVeiculo->descreveLugares, stringParaCampoString(strsep(&leitor, ",")));
    strcpy(cabecalhoVeiculo->descreveLinha, stringParaCampoString(strsep(&leitor, ",")));
    strcpy(cabecalhoVeiculo->descreveModelo, stringParaCampoString(strsep(&leitor, ",")));
    strcpy(cabecalhoVeiculo->descreveCategoria, stringParaCampoString(strsep(&leitor, ",")));

    free(string);
    return 0;
}

void leCabecalhoVeiculoBinario(CabecalhoVeiculo *cabecalhoVeiculo, FILE *binario) {
    fread(&cabecalhoVeiculo->status, sizeof(char), 1, binario);
    fread(&cabecalhoVeiculo->byteProxReg, sizeof(long long int), 1, binario);
    fread(&cabecalhoVeiculo->nroRegistros, sizeof(int), 1, binario);
    fread(&cabecalhoVeiculo->nroRegRemovidos, sizeof(int), 1, binario);

    leStringBinario(cabecalhoVeiculo->descrevePrefixo, 18, binario);
    leStringBinario(cabecalhoVeiculo->descreveData, 35, binario);
    leStringBinario(cabecalhoVeiculo->descreveLugares, 42, binario);
    leStringBinario(cabecalhoVeiculo->descreveLinha, 26, binario);
    leStringBinario(cabecalhoVeiculo->descreveModelo, 17, binario);
    leStringBinario(cabecalhoVeiculo->descreveCategoria, 20, binario);
}

void escreveCabecalhoVeiculoBinario(CabecalhoVeiculo *cabecalhoVeiculo, FILE *binario) {
    fwrite(&cabecalhoVeiculo->status, sizeof(char), 1, binario);
    fwrite(&cabecalhoVeiculo->byteProxReg, sizeof(long long int), 1, binario);
    fwrite(&cabecalhoVeiculo->nroRegistros, sizeof(int), 1, binario);
    fwrite(&cabecalhoVeiculo->nroRegRemovidos, sizeof(int), 1, binario);

    fwrite(cabecalhoVeiculo->descrevePrefixo, sizeof(char), 18, binario);
    fwrite(cabecalhoVeiculo->descreveData, sizeof(char), 35, binario);
    fwrite(cabecalhoVeiculo->descreveLugares, sizeof(char), 42, binario);
    fwrite(cabecalhoVeiculo->descreveLinha, sizeof(char), 26, binario);
    fwrite(cabecalhoVeiculo->descreveModelo, sizeof(char), 17, binario);
    fwrite(cabecalhoVeiculo->descreveCategoria, sizeof(char), 20, binario);
}

// Veículo

int leVeiculoCsv(Veiculo *veiculo, FILE *csv) {
    // Lê string do CSV
    char *string = leStringArquivo(csv);
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

    strcpy(veiculo->prefixo, stringParaCampoString(strsep(&leitor, ",")));
    strcpy(veiculo->data, stringParaCampoString(strsep(&leitor, ",")));

    veiculo->quantidadeLugares = stringParaCampoInteiro(strsep(&leitor, ","));
    veiculo->codLinha = stringParaCampoInteiro(strsep(&leitor, ","));

    strcpy(veiculo->modelo, stringParaCampoString(strsep(&leitor, ",")));
    strcpy(veiculo->categoria, stringParaCampoString(strsep(&leitor, ",")));

    veiculo->tamanhoModelo = strlen(veiculo->modelo);
    veiculo->tamanhoCategoria = strlen(veiculo->categoria);
    veiculo->tamanhoRegistro = 31 + veiculo->tamanhoModelo + veiculo->tamanhoCategoria;

    free(string);
    return 0;
}

bool leVeiculoBinario(Veiculo *veiculo, FILE *binario) {
    fread(&veiculo->removido, sizeof(char), 1, binario);
    fread(&veiculo->tamanhoRegistro, sizeof(int), 1, binario);

    // Se o registro foi removido, mata a execução
    if (registroFoiRemovido(veiculo->removido))
        return false;

    leStringBinario(veiculo->prefixo, 5, binario);
    leStringBinario(veiculo->data, 10, binario);

    fread(&veiculo->quantidadeLugares, sizeof(int), 1, binario);
    fread(&veiculo->codLinha, sizeof(int), 1, binario);

    fread(&veiculo->tamanhoModelo, sizeof(int), 1, binario);
    leStringBinario(veiculo->modelo, veiculo->tamanhoModelo, binario);

    fread(&veiculo->tamanhoCategoria, sizeof(int), 1, binario);
    leStringBinario(veiculo->categoria, veiculo->tamanhoCategoria, binario);

    // Garante a completude do registro
    return true;
}

void leVeiculoEntrada(Veiculo *veiculo) {
    veiculo->removido = '1';

    scan_quote_string(veiculo->prefixo);
    scan_quote_string(veiculo->data);
    
    veiculo->quantidadeLugares = leInteiroEntrada();
    veiculo->codLinha = leInteiroEntrada();

    scan_quote_string(veiculo->modelo);
    scan_quote_string(veiculo->categoria);

    veiculo->tamanhoModelo = strlen(veiculo->modelo);
    veiculo->tamanhoCategoria = strlen(veiculo->categoria);
    veiculo->tamanhoRegistro = 31 + veiculo->tamanhoModelo + veiculo->tamanhoCategoria;
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

bool comparaVeiculo(Veiculo veiculo, char *nomeCampo, char *valor) {
    if (
        comparaRegistroCampoString(STR_PREFIXO, nomeCampo,
                                   veiculo.prefixo, valor) ||

        comparaRegistroCampoString(STR_MODELO, nomeCampo,
                                   veiculo.modelo, valor) ||

        comparaRegistroCampoString(STR_CATEGORIA, nomeCampo,
                                   veiculo.categoria, valor) ||

        comparaRegistroCampoString(STR_DATA, nomeCampo,
                                   veiculo.data, valor) ||

        comparaRegistroCampoInt(STR_QTDE_LUGARES, nomeCampo,
                                veiculo.quantidadeLugares, valor)) {
        return true;
    }

    return false;
}

void imprimeVeiculo(CabecalhoVeiculo cabecalhoVeiculo, Veiculo veiculo) {
    printf("%s: ", cabecalhoVeiculo.descrevePrefixo);
    imprimeCampoString(veiculo.prefixo, strlen(veiculo.prefixo));

    printf("%s: ", cabecalhoVeiculo.descreveModelo);
    imprimeCampoString(veiculo.modelo, veiculo.tamanhoModelo);

    printf("%s: ", cabecalhoVeiculo.descreveCategoria);
    imprimeCampoString(veiculo.categoria, veiculo.tamanhoCategoria);

    printf("%s: ", cabecalhoVeiculo.descreveData);
    imprimeData(veiculo.data);

    printf("%s: ", cabecalhoVeiculo.descreveLugares);
    imprimeCampoInteiro(veiculo.quantidadeLugares);

    printf("\n");
}
