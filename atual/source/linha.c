/**
 * @author Matheus Henrique de Cerqueira Pinto (11911104)
 * @author Pedro Lucas de Moliner de Castro (11795784)
 * @date 2021-05-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "mensagem.h"
#include "linha.h"
#include "util.h"


static const int TAMANHO_BASE_LINHA = 13;


static void _imprimeAceitaCartao(const char *aceitaCartao);


/**
 *
 * Cabeçalho da Linha 
 * 
 */

// Lê o cabeçalho de uma Linha a partir de um CSV
CabecalhoLinha leCabecalhoLinhaCsv(FILE *csv) {
    CabecalhoLinha cabecalho;

    cabecalho.status = '0';
    cabecalho.byteProxReg = 0;
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

// Escreve o cabeçalho de uma Linha num binário
void escreveCabecalhoLinhaBinario(CabecalhoLinha cabecalhoLinha, FILE *binario) {
    fwrite(&cabecalhoLinha.status, sizeof(char), 1, binario);
    fwrite(&cabecalhoLinha.byteProxReg, sizeof(long long int), 1, binario);
    fwrite(&cabecalhoLinha.nroRegistros, sizeof(int), 1, binario);
    fwrite(&cabecalhoLinha.nroRegRemovidos, sizeof(int), 1, binario);

    fwrite(cabecalhoLinha.descreveCodigo, sizeof(char), 15, binario);
    fwrite(cabecalhoLinha.descreveCartao, sizeof(char), 13, binario);
    fwrite(cabecalhoLinha.descreveNome, sizeof(char), 13, binario);
    fwrite(cabecalhoLinha.descreveLinha, sizeof(char), 24, binario);
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
bool comparaLinha(Linha *linha, const char *campo, const char *valor) {
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
 * Auxiliares 
 * 
 */

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
