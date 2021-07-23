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


static void _posicionaBinarioCabecalhoVeiculo(FILE *binario);
static void _imprimeData(const char *data);
static int _comparaVeiculos(const void *primeiro, const void *segundo);


/**
 *
 * Cabeçalho do Veículo 
 * 
 */

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

// Cria um cabeçalho de veículo novo para ordenação baseado no original
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


/**
 *
 * Veículos
 * 
 */

// Lê um vetor de veículos válidos a partir de um binário
void leVeiculosBinario(Veiculo *veiculos, int numero, FILE *binario) {
    for (int i = 0, j = 0; i < numero; i++) {
        Veiculo temporario = leVeiculoBinario(binario);
        
        // Adiciona ao vetor se não removido
        if (!registroFoiRemovido(temporario.removido)) {
            veiculos[j++] = temporario;
        }
    }
}

// Escreve um vetor de veículos em um binário
void escreveVeiculosBinario(Veiculo *veiculos, int numero, FILE *binario) {
    for (int i = 0; i < numero; i++) {
        escreveVeiculoBinario(veiculos[i], binario);
    }
}

// Ordena um vetor de veículos por codLinha
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
