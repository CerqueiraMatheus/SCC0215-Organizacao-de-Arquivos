// Pedro Lucas de Moliner de Castro - 11795784
// Matheus Henrique de Cerqueira Pinto - 11911104

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "util.h"
#include "linha.h"


const char *STRING_CODIGO_LINHA = "codLinha";
const char *STRING_ACEITA_CARTAO = "aceitaCartao";
const char *STRING_NOME_LINHA = "nomeLinha";
const char *STRING_COR_LINHA = "corLinha";


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
    // Checa pelo fim do arquivo
    char verificador = fgetc(csv);
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

bool comparaLinha(Linha linha, char *campo, char *valor) {
    if (comparaCampoInteiro(campo, STRING_CODIGO_LINHA, stringParaInteiro(valor), linha.codLinha))
        return true;
    else if (comparaCampoString(campo, STRING_ACEITA_CARTAO, valor, linha.aceitaCartao))
        return true;
    else if (comparaCampoString(campo, STRING_NOME_LINHA, valor, linha.nomeLinha))
        return true;
    else if (comparaCampoString(campo, STRING_COR_LINHA, valor, linha.corLinha))
        return true;
    else
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
