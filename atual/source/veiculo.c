/**
 * @author Matheus Henrique de Cerqueira Pinto (11911104)
 * @author Pedro Lucas de Moliner de Castro (11795784)
 * @date 2021-05-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "mensagem.h"
#include "veiculo.h"
#include "util.h"


static const int TAMANHO_CABECALHO_VEICULO = 175;
static const int TAMANHO_BASE_VEICULO = 31;


static void _posicionaBinarioCabecalhoVeiculo(FILE *binario);
static void _imprimeData(const char *data);
static int _comparaVeiculos(const void *primeiro, const void *segundo);


/**
 *
 * Binário
 * 
 */

void posicionaBinarioProximoRegistroVeiculo(FILE *binario, CabecalhoVeiculo cabecalho) {
    fseek(binario, cabecalho.byteProxReg, SEEK_SET);
}


/**
 *
 * Cabeçalho do Veículo 
 * 
 */

// Lê o cabeçalho de um Veículo a partir de um CSV
CabecalhoVeiculo leCabecalhoVeiculoCsv(FILE *csv) {
    CabecalhoVeiculo cabecalho;

    cabecalho.status = '0';
    cabecalho.byteProxReg = TAMANHO_CABECALHO_VEICULO;
    cabecalho.nroRegistros = 0;
    cabecalho.nroRegRemovidos = 0;

    leStringCsv(cabecalho.descrevePrefixo, csv);
    leStringCsv(cabecalho.descreveData, csv);
    leStringCsv(cabecalho.descreveLugares, csv);
    leStringCsv(cabecalho.descreveLinha, csv);
    leStringCsv(cabecalho.descreveModelo, csv);
    leStringCsv(cabecalho.descreveCategoria, csv);

    return cabecalho;
}

// Lê o cabeçalho de um Veículo a partir de um binário
CabecalhoVeiculo leCabecalhoVeiculoBinario(FILE *binario) {
    _posicionaBinarioCabecalhoVeiculo(binario);

    CabecalhoVeiculo cabecalho;

    fread(&cabecalho.status, sizeof(char), 1, binario);
    fread(&cabecalho.byteProxReg, sizeof(long long int), 1, binario);
    fread(&cabecalho.nroRegistros, sizeof(int), 1, binario);
    fread(&cabecalho.nroRegRemovidos, sizeof(int), 1, binario);

    leStringBinario(cabecalho.descrevePrefixo, 18, binario);
    leStringBinario(cabecalho.descreveData, 35, binario);
    leStringBinario(cabecalho.descreveLugares, 42, binario);
    leStringBinario(cabecalho.descreveLinha, 26, binario);
    leStringBinario(cabecalho.descreveModelo, 17, binario);
    leStringBinario(cabecalho.descreveCategoria, 20, binario);

    return cabecalho;
}

CabecalhoVeiculo criaCabecalhoVeiculoOrdenado(CabecalhoVeiculo original) {
    CabecalhoVeiculo ordenado;

    ordenado.status = '0';
    ordenado.byteProxReg = TAMANHO_CABECALHO_VEICULO;
    ordenado.nroRegistros = 0;
    ordenado.nroRegRemovidos = 0;

    strcpy(ordenado.descrevePrefixo, original.descrevePrefixo);
    strcpy(ordenado.descreveData, original.descreveData);
    strcpy(ordenado.descreveLugares, original.descreveLugares);
    strcpy(ordenado.descreveLinha, original.descreveLinha);
    strcpy(ordenado.descreveModelo, original.descreveModelo);
    strcpy(ordenado.descreveCategoria, original.descreveCategoria);

    return ordenado;
}

// Escreve o cabeçalho de um Veículo num binário
void escreveCabecalhoVeiculoBinario(CabecalhoVeiculo cabecalho, FILE *binario) {
    _posicionaBinarioCabecalhoVeiculo(binario);

    fwrite(&cabecalho.status, sizeof(char), 1, binario);
    fwrite(&cabecalho.byteProxReg, sizeof(long long int), 1, binario);
    fwrite(&cabecalho.nroRegistros, sizeof(int), 1, binario);
    fwrite(&cabecalho.nroRegRemovidos, sizeof(int), 1, binario);

    fwrite(cabecalho.descrevePrefixo, sizeof(char), 18, binario);
    fwrite(cabecalho.descreveData, sizeof(char), 35, binario);
    fwrite(cabecalho.descreveLugares, sizeof(char), 42, binario);
    fwrite(cabecalho.descreveLinha, sizeof(char), 26, binario);
    fwrite(cabecalho.descreveModelo, sizeof(char), 17, binario);
    fwrite(cabecalho.descreveCategoria, sizeof(char), 20, binario);
}


/**
 *
 * Veículo 
 * 
 */

// Lê um Veículo a partir de um CSV
Veiculo leVeiculoCsv(FILE *csv, bool *ehEOF) {
    Veiculo veiculo;

    // Checa pelo fim do CSV
    char verificador = fgetc(csv);
    if (verificador == EOF) {
        *ehEOF = true;
        return veiculo;
    }

    // Checa por registro removido
    if (verificador == '*') {
        veiculo.removido = '0';
    }
    else {
        veiculo.removido = '1';
        fseek(csv, -1, SEEK_CUR);
    }

    leStringCsv(veiculo.prefixo, csv);
    leStringCsv(veiculo.data, csv);

    veiculo.quantidadeLugares = leInteiroCsv(csv);
    veiculo.codLinha = leInteiroCsv(csv);

    leStringCsv(veiculo.modelo, csv);
    leStringCsv(veiculo.categoria, csv);

    veiculo.tamanhoModelo = strlen(veiculo.modelo);
    veiculo.tamanhoCategoria = strlen(veiculo.categoria);
    veiculo.tamanhoRegistro = TAMANHO_BASE_VEICULO + veiculo.tamanhoModelo + veiculo.tamanhoCategoria;

    return veiculo;
}

// Lê um Veículo a partir de um binário e retorna se ele foi removido
Veiculo leVeiculoBinario(FILE *binario) {
    Veiculo veiculo;

    fread(&veiculo.removido, sizeof(char), 1, binario);
    fread(&veiculo.tamanhoRegistro, sizeof(int), 1, binario);

    // Checa por registro removido e retorna se sim
    if (registroFoiRemovido(veiculo.removido)) {
        fseek(binario, veiculo.tamanhoRegistro, SEEK_CUR);
        return veiculo;
    }

    leStringBinario(veiculo.prefixo, 5, binario);
    leStringBinario(veiculo.data, 10, binario);

    fread(&veiculo.quantidadeLugares, sizeof(int), 1, binario);
    fread(&veiculo.codLinha, sizeof(int), 1, binario);

    fread(&veiculo.tamanhoModelo, sizeof(int), 1, binario);
    leStringBinario(veiculo.modelo, veiculo.tamanhoModelo, binario);

    fread(&veiculo.tamanhoCategoria, sizeof(int), 1, binario);
    leStringBinario(veiculo.categoria, veiculo.tamanhoCategoria, binario);

    return veiculo;
}

// Lê um Veículo a partir da entrada padrão
Veiculo leVeiculoEntrada() {
    Veiculo veiculo;

    veiculo.removido = '1';

    scan_quote_string(veiculo.prefixo);
    scan_quote_string(veiculo.data);

    veiculo.quantidadeLugares = leInteiroEntrada();
    veiculo.codLinha = leInteiroEntrada();

    scan_quote_string(veiculo.modelo);
    scan_quote_string(veiculo.categoria);

    veiculo.tamanhoModelo = strlen(veiculo.modelo);
    veiculo.tamanhoCategoria = strlen(veiculo.categoria);
    veiculo.tamanhoRegistro = TAMANHO_BASE_VEICULO + veiculo.tamanhoModelo + veiculo.tamanhoCategoria;

    return veiculo;
}

// Escreve um Veículo num binário
void escreveVeiculoBinario(Veiculo veiculo, FILE *binario) {
    fwrite(&veiculo.removido, sizeof(char), 1, binario);
    fwrite(&veiculo.tamanhoRegistro, sizeof(int), 1, binario);

    fwrite(veiculo.prefixo, sizeof(char), 5, binario);

    // Escreve o campo data possivelmente nulo
    if (strlen(veiculo.data) == 0) {
        escreveStringNuloBinario(10, binario);
    }
    else {
        fwrite(veiculo.data, sizeof(char), 10, binario);
    }

    fwrite(&veiculo.quantidadeLugares, sizeof(int), 1, binario);
    fwrite(&veiculo.codLinha, sizeof(int), 1, binario);

    fwrite(&veiculo.tamanhoModelo, sizeof(int), 1, binario);
    fwrite(veiculo.modelo, sizeof(char), veiculo.tamanhoModelo, binario);

    fwrite(&veiculo.tamanhoCategoria, sizeof(int), 1, binario);
    fwrite(veiculo.categoria, sizeof(char), veiculo.tamanhoCategoria, binario);
}

// Imprime um Veículo
void imprimeVeiculo(Veiculo veiculo, CabecalhoVeiculo cabecalho) {
    printf("%s: ", cabecalho.descrevePrefixo);
    imprimeCampoString(veiculo.prefixo, strlen(veiculo.prefixo));

    printf("%s: ", cabecalho.descreveModelo);
    imprimeCampoString(veiculo.modelo, veiculo.tamanhoModelo);

    printf("%s: ", cabecalho.descreveCategoria);
    imprimeCampoString(veiculo.categoria, veiculo.tamanhoCategoria);

    printf("%s: ", cabecalho.descreveData);
    _imprimeData(veiculo.data);

    printf("%s: ", cabecalho.descreveLugares);
    imprimeCampoInteiro(veiculo.quantidadeLugares);
}

// Verifica se um Veículo corresponde ao campo e valor pesquisados
bool comparaCampoVeiculo(Veiculo *veiculo, const char *campo, const char *valor) {
    if (
        comparaCampoString(campo, "prefixo", valor, veiculo->prefixo) ||
        comparaCampoString(campo, "data", valor, veiculo->data) ||
        comparaCampoInteiro(campo, "quantidadeLugares", stringParaInteiro(valor), veiculo->quantidadeLugares) ||
        comparaCampoString(campo, "modelo", valor, veiculo->modelo) ||
        comparaCampoString(campo, "categoria", valor, veiculo->categoria)
    ) {
        return true;
    }

    return false;
}


/**
 *
 * Veículos
 * 
 */

void leVeiculosBinario(Veiculo *veiculos, int numero, FILE *binario) {
    for (int i = 0, j = 0; i < numero; i++) {
        Veiculo temporario = leVeiculoBinario(binario);
        
        if (!registroFoiRemovido(temporario.removido)) {
            veiculos[j++] = temporario;
        }
    }
}

void escreveVeiculosBinario(Veiculo *veiculos, int numero, FILE *binario) {
    for (int i = 0; i < numero; i++) {
        escreveVeiculoBinario(veiculos[i], binario);
    }
}

void ordenaVeiculos(Veiculo *veiculos, int numero) {
    qsort(veiculos, numero, sizeof(Veiculo), _comparaVeiculos);
}


/**
 *
 * Auxiliares
 * 
 */

static void _posicionaBinarioCabecalhoVeiculo(FILE *binario) {
    fseek(binario, 0, SEEK_SET);
}

// Imprime uma data por extenso (trata casos nulos)
static void _imprimeData(const char *data) {
    if (data[0] == '\0') {
        printf("%s\n", CAMPO_NULO);
        return;
    }

    int dia, mes, ano;
    sscanf(data, "%d-%d-%d", &ano, &mes, &dia);

    printf("%02d de %s de %d\n", dia, MESES[mes - 1], ano);
}

static int _comparaVeiculos(const void *primeiro, const void *segundo) {
    return ((Veiculo *)primeiro)->codLinha - ((Veiculo *)segundo)->codLinha;
}
