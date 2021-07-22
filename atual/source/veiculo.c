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


static void _imprimeData(const char *data);
static int _comparaVeiculos(const void *primeiro, const void *segundo);


/**
 *
 * Cabeçalho do Veículo 
 * 
 */

// Lê o cabeçalho de um Veículo a partir de um CSV
CabecalhoVeiculo leCabecalhoVeiculoCsv(FILE *csv) {
    CabecalhoVeiculo cabecalho;

    cabecalho.status = '0';
    cabecalho.byteProxReg = 0;
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

CabecalhoVeiculo criaCabecalhoVeiculoNovo(CabecalhoVeiculo original) {
    CabecalhoVeiculo novo;

    novo.status = '0';
    novo.byteProxReg = 0;

    strcpy(novo.descrevePrefixo, original.descrevePrefixo);
    strcpy(novo.descreveData, original.descreveData);
    strcpy(novo.descreveLugares, original.descreveLugares);
    strcpy(novo.descreveLinha, original.descreveLinha);
    strcpy(novo.descreveModelo, original.descreveModelo);
    strcpy(novo.descreveCategoria, original.descreveCategoria);

    novo.nroRegistros = original.nroRegistros;
    novo.nroRegRemovidos = 0;

    return novo;
}

// Escreve o cabeçalho de um Veículo num binário
void escreveCabecalhoVeiculoBinario(CabecalhoVeiculo cabecalhoVeiculo, FILE *binario) {
    fwrite(&cabecalhoVeiculo.status, sizeof(char), 1, binario);
    fwrite(&cabecalhoVeiculo.byteProxReg, sizeof(long long int), 1, binario);
    fwrite(&cabecalhoVeiculo.nroRegistros, sizeof(int), 1, binario);
    fwrite(&cabecalhoVeiculo.nroRegRemovidos, sizeof(int), 1, binario);

    fwrite(cabecalhoVeiculo.descrevePrefixo, sizeof(char), 18, binario);
    fwrite(cabecalhoVeiculo.descreveData, sizeof(char), 35, binario);
    fwrite(cabecalhoVeiculo.descreveLugares, sizeof(char), 42, binario);
    fwrite(cabecalhoVeiculo.descreveLinha, sizeof(char), 26, binario);
    fwrite(cabecalhoVeiculo.descreveModelo, sizeof(char), 17, binario);
    fwrite(cabecalhoVeiculo.descreveCategoria, sizeof(char), 20, binario);
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
    veiculo.tamanhoRegistro = 31 + veiculo.tamanhoModelo + veiculo.tamanhoCategoria;

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
    veiculo.tamanhoRegistro = 31 + veiculo.tamanhoModelo + veiculo.tamanhoCategoria;

    return veiculo;
}

// Escreve um Veículo num binário
void escreveVeiculoBinario(Veiculo veiculo, FILE *binario) {
    fwrite(&veiculo.removido, sizeof(char), 1, binario);
    fwrite(&veiculo.tamanhoRegistro, sizeof(int), 1, binario);

    fwrite(veiculo.prefixo, sizeof(char), 5, binario);

    // Escreve o campo data possivelmente nulo
    if (strlen(veiculo.data) == 0)
        escreveStringNuloBinario(10, binario);
    else
        fwrite(veiculo.data, sizeof(char), 10, binario);

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
bool comparaVeiculo(Veiculo *veiculo, const char *campo, const char *valor) {
    if (comparaCampoString(campo, "prefixo", valor, veiculo->prefixo))
        return true;
    else if (comparaCampoString(campo, "data", valor, veiculo->data))
        return true;
    else if (comparaCampoInteiro(campo, "quantidadeLugares", stringParaInteiro(valor), veiculo->quantidadeLugares))
        return true;
    else if (comparaCampoString(campo, "modelo", valor, veiculo->modelo))
        return true;
    else if (comparaCampoString(campo, "categoria", valor, veiculo->categoria))
        return true;
    else
        return false;
}


/**
 *
 * Veículos
 * 
 */

void leVeiculosValidosBinario(Veiculo *veiculos, int total, FILE *binario) {
    for (int i = 0, j = 0; i < total; i++) {
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
