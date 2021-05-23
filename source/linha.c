// Pedro Lucas de Moliner de Castro - 11795784
// Matheus Henrique de Cerqueira Pinto - 11911104

#include "linha.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

// Cabeçalho Linha

int leCabecalhoLinhaCsv(CabecalhoLinha *cabecalhoLinha, FILE *csv) {
    // Lê string do CSV
    char *string = leStringCsv(csv);
    if (string == NULL)
        return EOF;
    char *leitor = string;

    cabecalhoLinha->status = '0';
    cabecalhoLinha->byteProxReg = 0;
    cabecalhoLinha->nroRegistros = 0;
    cabecalhoLinha->nroRegRemovidos = 0;

    strcpy(cabecalhoLinha->descreveCodigo, stringParaCampoString(strsep(&leitor, ",")));
    strcpy(cabecalhoLinha->descreveCartao, stringParaCampoString(strsep(&leitor, ",")));
    strcpy(cabecalhoLinha->descreveNome, stringParaCampoString(strsep(&leitor, ",")));
    strcpy(cabecalhoLinha->descreveLinha, stringParaCampoString(strsep(&leitor, ",")));

    free(string);
    return 0;
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
    // Lê string do CSV
    char *string = leStringCsv(csv);
    if (string == NULL)
        return EOF;
    char *leitor = string;

    // Checa por registro removido
    if (leitor[0] == '*') {
        linha->removido = '0';
        leitor++;
    } else {
        linha->removido = '1';
    }

    linha->codLinha = stringParaCampoInteiro(strsep(&leitor, ","));

    strcpy(linha->aceitaCartao, stringParaCampoString(strsep(&leitor, ",")));
    strcpy(linha->nomeLinha, stringParaCampoString(strsep(&leitor, ",")));
    strcpy(linha->corLinha, stringParaCampoString(strsep(&leitor, ",")));

    linha->tamanhoNome = strlen(linha->nomeLinha);
    linha->tamanhoCor = strlen(linha->corLinha);
    linha->tamanhoRegistro = 13 + linha->tamanhoNome + linha->tamanhoCor;

    free(string);
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
                                   linha.corLinha, valor))  {
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
