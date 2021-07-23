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
#include "linha.h"
#include "util.h"


static const int TAMANHO_CABECALHO_LINHA = 82;
static const int TAMANHO_BASE_LINHA = 13;


static void _posicionaBinarioCabecalhoLinha(FILE *binario);
static void _imprimeAceitaCartao(const char *aceitaCartao);
static int _comparaLinhas(const void *primeira, const void *segunda);


/**
 *
 * Binário
 * 
 */

void posicionaBinarioPrimeiroRegistroLinha(FILE *binario) {
    fseek(binario, TAMANHO_CABECALHO_LINHA, SEEK_SET);
}

void posicionaBinarioProximoRegistroLinha(FILE *binario, CabecalhoLinha cabecalho) {
    fseek(binario, cabecalho.byteProxReg, SEEK_SET);
}


/**
 *
 * Cabeçalho da Linha 
 * 
 */

// Lê o cabeçalho de uma Linha a partir de um CSV
CabecalhoLinha leCabecalhoLinhaCsv(FILE *csv) {
    CabecalhoLinha cabecalho;

    cabecalho.status = '0';
    cabecalho.byteProxReg = TAMANHO_CABECALHO_LINHA;
    cabecalho.nroRegistros = 0;
    cabecalho.nroRegRemovidos = 0;

    leStringCsv(cabecalho.descreveCodigo, csv);
    leStringCsv(cabecalho.descreveCartao, csv);
    leStringCsv(cabecalho.descreveNome, csv);
    leStringCsv(cabecalho.descreveLinha, csv);

    return cabecalho;
}

// Lê o cabeçalho de uma Linha a partir de um binário
CabecalhoLinha leCabecalhoLinhaBinario(FILE *binario) {
    _posicionaBinarioCabecalhoLinha(binario);

    CabecalhoLinha cabecalho;

    fread(&cabecalho.status, sizeof(char), 1, binario);
    fread(&cabecalho.byteProxReg, sizeof(long long int), 1, binario);
    fread(&cabecalho.nroRegistros, sizeof(int), 1, binario);
    fread(&cabecalho.nroRegRemovidos, sizeof(int), 1, binario);

    leStringBinario(cabecalho.descreveCodigo, 15, binario);
    leStringBinario(cabecalho.descreveCartao, 13, binario);
    leStringBinario(cabecalho.descreveNome, 13, binario);
    leStringBinario(cabecalho.descreveLinha, 24, binario);

    return cabecalho;
}

CabecalhoLinha criaCabecalhoLinhaOrdenado(CabecalhoLinha original) {
    CabecalhoLinha ordenado;

    ordenado.status = '0';
    ordenado.byteProxReg = TAMANHO_CABECALHO_LINHA;
    ordenado.nroRegistros = 0;
    ordenado.nroRegRemovidos = 0;

    strcpy(ordenado.descreveCodigo, original.descreveCodigo);
    strcpy(ordenado.descreveCartao, original.descreveCartao);
    strcpy(ordenado.descreveNome, original.descreveNome);
    strcpy(ordenado.descreveLinha, original.descreveLinha);

    return ordenado;
}

// Escreve o cabeçalho de uma Linha num binário
void escreveCabecalhoLinhaBinario(CabecalhoLinha cabecalho, FILE *binario) {
    _posicionaBinarioCabecalhoLinha(binario);

    fwrite(&cabecalho.status, sizeof(char), 1, binario);
    fwrite(&cabecalho.byteProxReg, sizeof(long long int), 1, binario);
    fwrite(&cabecalho.nroRegistros, sizeof(int), 1, binario);
    fwrite(&cabecalho.nroRegRemovidos, sizeof(int), 1, binario);

    fwrite(cabecalho.descreveCodigo, sizeof(char), 15, binario);
    fwrite(cabecalho.descreveCartao, sizeof(char), 13, binario);
    fwrite(cabecalho.descreveNome, sizeof(char), 13, binario);
    fwrite(cabecalho.descreveLinha, sizeof(char), 24, binario);
}


/**
 *
 * Linha
 * 
 */

// Lê uma Linha a partir de um CSV
Linha leLinhaCsv(FILE *csv, bool *ehEOF) {
    Linha linha;

    // Checa pelo fim do arquivo
    char verificador = fgetc(csv);
    if (verificador == EOF) {
        *ehEOF = true;
        return linha;
    }

    // Checa por registro removido
    if (verificador == '*') {
        linha.removido = '0';
    }
    else {
        linha.removido = '1';
        fseek(csv, -1, SEEK_CUR);
    }

    linha.codLinha = leInteiroCsv(csv);

    leStringCsv(linha.aceitaCartao, csv);
    leStringCsv(linha.nomeLinha, csv);
    leStringCsv(linha.corLinha, csv);

    linha.tamanhoNome = strlen(linha.nomeLinha);
    linha.tamanhoCor = strlen(linha.corLinha);
    linha.tamanhoRegistro = TAMANHO_BASE_LINHA + linha.tamanhoNome + linha.tamanhoCor;

    return linha;
}

// Lê uma Linha a partir de um binário e retorna se ela foi removida
Linha leLinhaBinario(FILE *binario) {
    Linha linha;

    fread(&linha.removido, sizeof(char), 1, binario);
    fread(&linha.tamanhoRegistro, sizeof(int), 1, binario);

    // Checa por registro removido e retorna se sim
    if (registroFoiRemovido(linha.removido)) {
        fseek(binario, linha.tamanhoRegistro, SEEK_CUR);
        return linha;
    }

    fread(&linha.codLinha, sizeof(int), 1, binario);

    leStringBinario(linha.aceitaCartao, 1, binario);

    fread(&linha.tamanhoNome, sizeof(int), 1, binario);
    leStringBinario(linha.nomeLinha, linha.tamanhoNome, binario);

    fread(&linha.tamanhoCor, sizeof(int), 1, binario);
    leStringBinario(linha.corLinha, linha.tamanhoCor, binario);

    return linha;
}

// Lê uma Linha a partir da entrada padrão
Linha leLinhaEntrada() {
    Linha linha;

    linha.removido = '1';

    linha.codLinha = leInteiroEntrada();

    scan_quote_string(linha.aceitaCartao);
    scan_quote_string(linha.nomeLinha);
    scan_quote_string(linha.corLinha);

    linha.tamanhoNome = strlen(linha.nomeLinha);
    linha.tamanhoCor = strlen(linha.corLinha);
    linha.tamanhoRegistro = TAMANHO_BASE_LINHA + linha.tamanhoNome + linha.tamanhoCor;

    return linha;
}

// Escreve uma Linha num binário
void escreveLinhaBinario(Linha linha, FILE *binario) {
    fwrite(&linha.removido, sizeof(char), 1, binario);
    fwrite(&linha.tamanhoRegistro, sizeof(int), 1, binario);

    fwrite(&linha.codLinha, sizeof(int), 1, binario);
    fwrite(linha.aceitaCartao, sizeof(char), 1, binario);

    fwrite(&linha.tamanhoNome, sizeof(int), 1, binario);
    fwrite(linha.nomeLinha, sizeof(char), linha.tamanhoNome, binario);

    fwrite(&linha.tamanhoCor, sizeof(int), 1, binario);
    fwrite(linha.corLinha, sizeof(char), linha.tamanhoCor, binario);
}

// Imprime uma Linha
void imprimeLinha(Linha linha, CabecalhoLinha cabecalho) {
    printf("%s: ", cabecalho.descreveCodigo);
    imprimeCampoInteiro(linha.codLinha);

    printf("%s: ", cabecalho.descreveNome);
    imprimeCampoString(linha.nomeLinha, linha.tamanhoNome);

    printf("%s: ", cabecalho.descreveLinha);
    imprimeCampoString(linha.corLinha, linha.tamanhoCor);

    printf("%s: ", cabecalho.descreveCartao);
    _imprimeAceitaCartao(linha.aceitaCartao);
}

// Verifica se uma Linha corresponde ao campo e valor pesquisados
bool comparaCampoLinha(Linha *linha, const char *campo, const char *valor) {
    if (
        comparaCampoInteiro(campo, "codLinha", stringParaInteiro(valor), linha->codLinha) ||
        comparaCampoString(campo, "aceitaCartao", valor, linha->aceitaCartao) ||
        comparaCampoString(campo, "nomeLinha", valor, linha->nomeLinha) ||
        comparaCampoString(campo, "corLinha", valor, linha->corLinha)
    ) {
        return true;
    }

    return false;
}


/**
 *
 * Linhas
 * 
 */

void leLinhasBinario(Linha *linhas, int numero, FILE *binario) {
    for (int i = 0, j = 0; i < numero; i++) {
        Linha temporario = leLinhaBinario(binario);

        if (!registroFoiRemovido(temporario.removido)) {
            linhas[j++] = temporario;
        }
    }
}

void escreveLinhasBinario(Linha *linhas, int numero, FILE *binario) {
    for (int i = 0; i < numero; i++) {
        escreveLinhaBinario(linhas[i], binario);
    }
}

void ordenaLinhas(Linha *linhas, int numero) {
    qsort(linhas, numero, sizeof(Linha), _comparaLinhas);
}


/**
 *
 * Auxiliares 
 * 
 */

static void _posicionaBinarioCabecalhoLinha(FILE *binario) {
    fseek(binario, 0, SEEK_SET);
}

// Imprime o aceite de cartão por extenso (trata casos nulos)
static void _imprimeAceitaCartao(const char *aceitaCartao) {
    if (aceitaCartao[0] == '\0') {
        printf("%s\n", CAMPO_NULO);
        return;
    }

    switch (aceitaCartao[0]) {
    case 'S':
        printf("PAGAMENTO SOMENTE COM CARTAO SEM PRESENCA DE COBRADOR\n");
        break;

    case 'N':
        printf("PAGAMENTO EM CARTAO E DINHEIRO\n");
        break;

    case 'F':
        printf("PAGAMENTO EM CARTAO SOMENTE NO FINAL DE SEMANA\n");
        break;
    }
}

static int _comparaLinhas(const void *primeira, const void *segunda) {
    return ((Linha *)primeira)->codLinha - ((Linha *)segunda)->codLinha;
}
