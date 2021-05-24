// Pedro Lucas de Moliner de Castro - 11795784
// Matheus Henrique de Cerqueira Pinto - 11911104

#include "veiculo.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"


const char *STRING_PREFIXO  = "prefixo";
const char *STRING_DATA  = "data";
const char *STRING_QUANTIDADE_LUGARES  = "quantidadeLugares";
const char *STRING_MODELO = "modelo";
const char *STRING_CATEGORIA = "categoria";


// Cabeçalho Veículo

void leCabecalhoVeiculoCsv(CabecalhoVeiculo *cabecalhoVeiculo, FILE *csv) {
    cabecalhoVeiculo->status = '0';
    cabecalhoVeiculo->byteProxReg = 0;
    cabecalhoVeiculo->nroRegistros = 0;
    cabecalhoVeiculo->nroRegRemovidos = 0;

    leStringCsv(cabecalhoVeiculo->descrevePrefixo, csv);
    leStringCsv(cabecalhoVeiculo->descreveData, csv);
    leStringCsv(cabecalhoVeiculo->descreveLugares, csv);
    leStringCsv(cabecalhoVeiculo->descreveLinha, csv);
    leStringCsv(cabecalhoVeiculo->descreveModelo, csv);
    leStringCsv(cabecalhoVeiculo->descreveCategoria, csv);
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
    char verificador = fgetc(csv);

    // Checa pelo fim do arquivo
    if (verificador == EOF)
        return EOF;

    // Checa por registro removido
    if (verificador == '*') {
        veiculo->removido = '0';
    } else {
        veiculo->removido = '1';
        fseek(csv, -1, SEEK_CUR);
    }

    leStringCsv(veiculo->prefixo, csv);
    leStringCsv(veiculo->data, csv);

    veiculo->quantidadeLugares = leInteiroCsv(csv);
    veiculo->codLinha = leInteiroCsv(csv);

    leStringCsv(veiculo->modelo, csv);
    leStringCsv(veiculo->categoria, csv);

    veiculo->tamanhoModelo = strlen(veiculo->modelo);
    veiculo->tamanhoCategoria = strlen(veiculo->categoria);
    veiculo->tamanhoRegistro = 31 + veiculo->tamanhoModelo + veiculo->tamanhoCategoria;

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

bool comparaVeiculo(Veiculo veiculo, char *campo, char *valor) {
    if (comparaCampoString(campo, STRING_PREFIXO, valor, veiculo.prefixo))
        return true;
    else if (comparaCampoString(campo, STRING_DATA, valor, veiculo.data))
        return true;
    else if (comparaCampoInteiro(campo, STRING_QUANTIDADE_LUGARES, stringParaInteiro(valor),  veiculo.quantidadeLugares))
        return true;
    else if (comparaCampoString(campo, STRING_MODELO, valor,  veiculo.modelo))
        return true;
    else if (comparaCampoString(campo, STRING_CATEGORIA, valor,  veiculo.categoria))
        return true;
    else
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
