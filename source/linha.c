// Pedro Lucas de Moliner de Castro - 11795784
// Matheus Henrique de Cerqueira Pinto - 11911104

#include "linha.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

// Cabeçalho Linha

void leCabecalhoLinhaCsv(CabecalhoLinha *cabecalhoLinha, FILE *csv) {
    cabecalhoLinha->status = '0';
    cabecalhoLinha->byteProxReg = 0;
    cabecalhoLinha->nroRegistros = 0;
    cabecalhoLinha->nroRegRemovidos = 0;

    leStringCsv(cabecalhoLinha->descreveCodigo, csv);
    leStringCsv(cabecalhoLinha->descreveCartao, csv);
    leStringCsv(cabecalhoLinha->descreveNome, csv);
    leStringCsv(cabecalhoLinha->descreveLinha, csv);
}

void leCabecalhoLinhaBinario(CabecalhoLinha *cabecalhoLinha, FILE *binario) {
    fread(&cabecalhoLinha->status, sizeof(char), 1, binario);
    fread(&cabecalhoLinha->byteProxReg, sizeof(long long int), 1, binario);
    fread(&cabecalhoLinha->nroRegistros, sizeof(int), 1, binario);
    fread(&cabecalhoLinha->nroRegRemovidos, sizeof(int), 1, binario);

    leStringBinario(cabecalhoLinha->descreveCodigo, 15, binario);
    leStringBinario(cabecalhoLinha->descreveCartao, 13, binario);
    leStringBinario(cabecalhoLinha->descreveNome, 13, binario);
    leStringBinario(cabecalhoLinha->descreveLinha, 24, binario);
}

void escreveCabecalhoLinhaBinario(CabecalhoLinha *cabecalhoLinha, FILE *binario) {
    fwrite(&cabecalhoLinha->status, sizeof(char), 1, binario);
    fwrite(&cabecalhoLinha->byteProxReg, sizeof(long long int), 1, binario);
    fwrite(&cabecalhoLinha->nroRegistros, sizeof(int), 1, binario);
    fwrite(&cabecalhoLinha->nroRegRemovidos, sizeof(int), 1, binario);

    fwrite(cabecalhoLinha->descreveCodigo, sizeof(char), 15, binario);
    fwrite(cabecalhoLinha->descreveCartao, sizeof(char), 13, binario);
    fwrite(cabecalhoLinha->descreveNome, sizeof(char), 13, binario);
    fwrite(cabecalhoLinha->descreveLinha, sizeof(char), 24, binario);
}

// Linha

int leLinhaCsv(Linha *linha, FILE *csv) {
    char verificador = fgetc(csv);

    // Checa pelo fim do arquivo
    if (verificador == EOF)
        return EOF;

    // Checa por registro removido
    if (verificador == '*') {
        linha->removido = '0';
    } else {
        linha->removido = '1';
        fseek(csv, -1, SEEK_CUR);
    }

    linha->codLinha = leInteiroCsv(csv);

    leStringCsv(linha->aceitaCartao, csv);
    leStringCsv(linha->nomeLinha, csv);
    leStringCsv(linha->corLinha, csv);

    linha->tamanhoNome = strlen(linha->nomeLinha);
    linha->tamanhoCor = strlen(linha->corLinha);
    linha->tamanhoRegistro = 13 + linha->tamanhoNome + linha->tamanhoCor;

    return 0;
}

bool leLinhaBinario(Linha *linha, FILE *binario) {
    fread(&linha->removido, sizeof(char), 1, binario);
    fread(&linha->tamanhoRegistro, sizeof(int), 1, binario);

    // Se o registro foi removido, mata a execução
    if (registroFoiRemovido(linha->removido))
        return false;

    fread(&linha->codLinha, sizeof(int), 1, binario);

    leStringBinario(linha->aceitaCartao, 1, binario);

    fread(&linha->tamanhoNome, sizeof(int), 1, binario);
    leStringBinario(linha->nomeLinha, linha->tamanhoNome, binario);

    fread(&linha->tamanhoCor, sizeof(int), 1, binario);
    leStringBinario(linha->corLinha, linha->tamanhoCor, binario);

    // Garante a completude do registro
    return true;
}

void leLinhaEntrada(Linha *linha) {
    linha->removido = '1';

    linha->codLinha = leInteiroEntrada();

    scan_quote_string(linha->aceitaCartao);
    scan_quote_string(linha->nomeLinha);
    scan_quote_string(linha->corLinha);

    linha->tamanhoNome = strlen(linha->nomeLinha);
    linha->tamanhoCor = strlen(linha->corLinha);
    linha->tamanhoRegistro = 13 + linha->tamanhoNome + linha->tamanhoCor;
}

void escreveLinhaBinario(Linha *linha, FILE *binario) {
    fwrite(&linha->removido, sizeof(char), 1, binario);
    fwrite(&linha->tamanhoRegistro, sizeof(int), 1, binario);

    fwrite(&linha->codLinha, sizeof(int), 1, binario);
    fwrite(linha->aceitaCartao, sizeof(char), 1, binario);

    fwrite(&linha->tamanhoNome, sizeof(int), 1, binario);
    fwrite(linha->nomeLinha, sizeof(char), linha->tamanhoNome, binario);

    fwrite(&linha->tamanhoCor, sizeof(int), 1, binario);
    fwrite(linha->corLinha, sizeof(char), linha->tamanhoCor, binario);
}

bool comparaLinha(Linha linha, char *nomeCampo, char *valor) {
    if (
        comparaRegistroCampoInt(STR_COD, nomeCampo,
                                linha.codLinha, valor) ||

        comparaRegistroCampoString(STR_CARTAO, nomeCampo,
                                   linha.aceitaCartao, valor) ||

        comparaRegistroCampoString(STR_NOME, nomeCampo,
                                   linha.nomeLinha, valor) ||

        comparaRegistroCampoString(STR_COR, nomeCampo,
                                   linha.corLinha, valor)) {
        return true;
    }

    return false;
}

void imprimeLinha(CabecalhoLinha cabecalhoLinha, Linha linha) {
    printf("%s: ", cabecalhoLinha.descreveCodigo);
    imprimeCampoInteiro(linha.codLinha);

    printf("%s: ", cabecalhoLinha.descreveNome);
    imprimeCampoString(linha.nomeLinha, linha.tamanhoNome);

    printf("%s: ", cabecalhoLinha.descreveLinha);
    imprimeCampoString(linha.corLinha, linha.tamanhoCor);

    printf("%s: ", cabecalhoLinha.descreveCartao);
    imprimeAceitaCartao(linha.aceitaCartao);

    printf("\n");
}
