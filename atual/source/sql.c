/**
 * @author Matheus Henrique de Cerqueira Pinto (11911104)
 * @author Pedro Lucas de Moliner de Castro (11795784)
 * @date 2021-05-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "sql.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arvoreb.h"
#include "linha.h"
#include "util.h"
#include "veiculo.h"

// Mensagens de erro
const char *MENSAGEM_FALHA_PROCESSAMENTO = "Falha no processamento do arquivo.";
const char *MENSAGEM_REGISTRO_INEXISTENTE = "Registro inexistente.";

/**
 * 
 * Create Table
 * 
 */

void createTableVeiculo() {
    char nomeCsv[255];
    char nomeBinario[255];

    // Recebe os nomes dos arquivos
    if (scanf("%s %s", nomeCsv, nomeBinario) != 2) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Abre o CSV
    FILE *csv = fopen(nomeCsv, "r");
    if (csv == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Abre o binário
    FILE *binario = fopen(nomeBinario, "wb");
    if (binario == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(csv);
        exit(0);
    }

    // Inicializa o cabeçalho
    CabecalhoVeiculo cabecalhoVeiculo;
    leCabecalhoVeiculoCsv(&cabecalhoVeiculo, csv);
    escreveCabecalhoVeiculoBinario(&cabecalhoVeiculo, binario);

    // Percorre o CSV escrevendo os registros
    Veiculo veiculo;
    while (leVeiculoCsv(&veiculo, csv) != EOF) {
        escreveVeiculoBinario(&veiculo, binario);

        if (registroFoiRemovido(veiculo.removido))
            cabecalhoVeiculo.nroRegRemovidos++;
        else
            cabecalhoVeiculo.nroRegistros++;
    }

    // Atualiza o cabeçalho
    cabecalhoVeiculo.byteProxReg = ftell(binario);
    cabecalhoVeiculo.status = '1';
    fseek(binario, 0, SEEK_SET);
    escreveCabecalhoVeiculoBinario(&cabecalhoVeiculo, binario);

    fclose(csv);
    fclose(binario);
    binarioNaTela(nomeBinario);
}

void createTableLinha() {
    char nomeCsv[255];
    char nomeBinario[255];

    // Recebe os nomes dos arquivos
    if (scanf("%s %s", nomeCsv, nomeBinario) != 2) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Abre o CSV
    FILE *csv = fopen(nomeCsv, "r");
    if (csv == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Abre o binário
    FILE *binario = fopen(nomeBinario, "wb");
    if (binario == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(csv);
        exit(0);
    }

    // Inicializa o cabeçalho
    CabecalhoLinha cabecalhoLinha;
    leCabecalhoLinhaCsv(&cabecalhoLinha, csv);
    escreveCabecalhoLinhaBinario(&cabecalhoLinha, binario);

    // Percorre o CSV escrevendo os registros
    Linha linha;
    while (leLinhaCsv(&linha, csv) != EOF) {
        escreveLinhaBinario(&linha, binario);

        if (registroFoiRemovido(linha.removido))
            cabecalhoLinha.nroRegRemovidos++;
        else
            cabecalhoLinha.nroRegistros++;
    }

    // Atualiza o cabeçalho
    cabecalhoLinha.byteProxReg = ftell(binario);
    cabecalhoLinha.status = '1';
    fseek(binario, 0, SEEK_SET);
    escreveCabecalhoLinhaBinario(&cabecalhoLinha, binario);

    fclose(csv);
    fclose(binario);
    binarioNaTela(nomeBinario);
}

/**
 * 
 * Select From
 * 
 */

void selectFromVeiculo() {
    char nomeBinario[255];

    // Recebe o nome do arquivo
    if (scanf("%s", nomeBinario) != 1) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Abre o binário
    FILE *binario = fopen(nomeBinario, "rb+");
    if (binario == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Lê o Cabeçalho do Veículo
    CabecalhoVeiculo cabecalhoVeiculo;
    leCabecalhoVeiculoBinario(&cabecalhoVeiculo, binario);
    atualizaStatusBinario('0', binario);

    // Checa a integridade do arquivo
    if (arquivoFoiCorrompido(cabecalhoVeiculo.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(binario);
        exit(0);
    }

    // Checa a existência de registros não removidos
    if (cabecalhoVeiculo.nroRegistros == 0) {
        printf("%s\n", MENSAGEM_REGISTRO_INEXISTENTE);
        atualizaStatusBinario('1', binario);
        fclose(binario);
        exit(0);
    }

    Veiculo veiculo;
    int nroTotalRegistros = cabecalhoVeiculo.nroRegistros + cabecalhoVeiculo.nroRegRemovidos;

    // Percorre os registros
    for (int i = 0; i < nroTotalRegistros; i++) {
        // Caso registro lido não removido
        if (leVeiculoBinario(&veiculo, binario) == true)
            imprimeVeiculo(&cabecalhoVeiculo, &veiculo);

        // Caso removido, pula corpo
        else
            fseek(binario, veiculo.tamanhoRegistro, SEEK_CUR);
    }

    atualizaStatusBinario('1', binario);
    fclose(binario);
}

void selectFromLinha() {
    char nomeBinario[255];

    // Recebe o nome do arquivo
    if (scanf("%s", nomeBinario) != 1) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Abre o binário
    FILE *binario = fopen(nomeBinario, "rb+");
    if (binario == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Lê o Cabeçalho da Linha
    CabecalhoLinha cabecalhoLinha;
    leCabecalhoLinhaBinario(&cabecalhoLinha, binario);
    atualizaStatusBinario('0', binario);

    // Checa a integridade do arquivo
    if (arquivoFoiCorrompido(cabecalhoLinha.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(binario);
        exit(0);
    }

    // Checa a existência de registros não removidos
    if (cabecalhoLinha.nroRegistros == 0) {
        printf("%s\n", MENSAGEM_REGISTRO_INEXISTENTE);
        atualizaStatusBinario('1', binario);
        fclose(binario);
        exit(0);
    }

    Linha linha;
    int nroTotalRegistros = cabecalhoLinha.nroRegistros + cabecalhoLinha.nroRegRemovidos;

    // Percorre os registros
    for (int i = 0; i < nroTotalRegistros; i++) {
        // Caso registro lido não removido
        if (leLinhaBinario(&linha, binario) == true)
            imprimeLinha(&cabecalhoLinha, &linha);

        // Caso removido, pula corpo
        else
            fseek(binario, linha.tamanhoRegistro, SEEK_CUR);
    }

    atualizaStatusBinario('1', binario);
    fclose(binario);
}

/**
 * 
 * Select From Where
 * 
 */

void selectFromWhereVeiculo() {
    char nomeBinario[255];
    char campo[20];

    // Recebe os nomes do arquivo e do campo
    if (scanf("%s %s", nomeBinario, campo) != 2) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Recebe o valor
    char valor[100];
    scan_quote_string(valor);

    // Abre o binário
    FILE *binario = fopen(nomeBinario, "rb+");
    if (binario == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Lê o Cabeçalho do Veículo
    CabecalhoVeiculo cabecalhoVeiculo;
    leCabecalhoVeiculoBinario(&cabecalhoVeiculo, binario);
    atualizaStatusBinario('0', binario);

    // Checa a integridade do arquivo
    if (arquivoFoiCorrompido(cabecalhoVeiculo.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(binario);
        exit(0);
    }

    // Checa a existência de registros não removidos
    if (cabecalhoVeiculo.nroRegistros == 0) {
        printf("%s\n", MENSAGEM_REGISTRO_INEXISTENTE);
        atualizaStatusBinario('1', binario);
        fclose(binario);
        exit(0);
    }

    Veiculo veiculo;
    int nroTotalRegistros = cabecalhoVeiculo.nroRegistros + cabecalhoVeiculo.nroRegRemovidos;

    // Flag para indicar se houve ao menos um registro encontrado
    bool houveCorrespondencia = false;

    // Percorre os registros
    for (int i = 0; i < nroTotalRegistros; i++) {
        // Caso registro lido não removido
        if (leVeiculoBinario(&veiculo, binario) == true) {
            // Checa valor desejado
            if (comparaVeiculo(&veiculo, campo, valor)) {
                imprimeVeiculo(&cabecalhoVeiculo, &veiculo);
                houveCorrespondencia = true;
            }
        }

        // Caso removido, pula corpo
        else
            fseek(binario, veiculo.tamanhoRegistro, SEEK_CUR);
    }

    if (!houveCorrespondencia)
        printf("%s\n", MENSAGEM_REGISTRO_INEXISTENTE);

    atualizaStatusBinario('1', binario);
    fclose(binario);
}

void selectFromWhereLinha() {
    char nomeBinario[255];
    char campo[20];

    // Recebe os nomes do arquivo e do campo
    if (scanf("%s %s", nomeBinario, campo) != 2) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Recebe o valor
    char valor[100];
    scan_quote_string(valor);

    // Abre o binário
    FILE *binario = fopen(nomeBinario, "rb+");
    if (binario == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Lê o Cabeçalho da Linha
    CabecalhoLinha cabecalhoLinha;
    leCabecalhoLinhaBinario(&cabecalhoLinha, binario);
    atualizaStatusBinario('0', binario);

    // Checa a integridade do arquivo
    if (arquivoFoiCorrompido(cabecalhoLinha.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(binario);
        exit(0);
    }

    // Checa a existência de registros não removidos
    if (cabecalhoLinha.nroRegistros == 0) {
        printf("%s\n", MENSAGEM_REGISTRO_INEXISTENTE);
        atualizaStatusBinario('1', binario);
        fclose(binario);
        exit(0);
    }

    Linha linha;
    int nroTotalRegistros = cabecalhoLinha.nroRegistros + cabecalhoLinha.nroRegRemovidos;

    // Flag para indicar se houve ao menos um registro encontrado
    bool houveCorrespondencia = false;

    // Percorre os registros
    for (int i = 0; i < nroTotalRegistros; i++) {
        // Caso registro lido não removido
        if (leLinhaBinario(&linha, binario) == true) {
            // Checa valor desejado
            if (comparaLinha(&linha, campo, valor)) {
                imprimeLinha(&cabecalhoLinha, &linha);
                houveCorrespondencia = true;
            }
        }

        // Caso removido, pula corpo
        else
            fseek(binario, linha.tamanhoRegistro, SEEK_CUR);
    }

    if (!houveCorrespondencia)
        printf("%s\n", MENSAGEM_REGISTRO_INEXISTENTE);

    atualizaStatusBinario('1', binario);
    fclose(binario);
}

/**
 * 
 * Insert Into
 * 
 */

void insertIntoVeiculo() {
    char nomeBinario[255];

    // Recebe o nome do arquivo
    if (scanf("%s", nomeBinario) != 1) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Abre o binário
    FILE *binario = fopen(nomeBinario, "rb+");
    if (binario == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Lê o Cabeçalho do Veículo
    CabecalhoVeiculo cabecalhoVeiculo;
    leCabecalhoVeiculoBinario(&cabecalhoVeiculo, binario);
    atualizaStatusBinario('0', binario);

    // Checa a integridade do arquivo
    if (arquivoFoiCorrompido(cabecalhoVeiculo.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(binario);
        exit(0);
    }

    // Recebe o número de inserções e posiciona o arquivo
    int insercoes;
    scanf("%d", &insercoes);
    fseek(binario, cabecalhoVeiculo.byteProxReg, SEEK_SET);

    // Percorre a entrada escrevendo os registros
    Veiculo veiculo;
    for (int i = 0; i < insercoes; i++) {
        leVeiculoEntrada(&veiculo);
        escreveVeiculoBinario(&veiculo, binario);
        cabecalhoVeiculo.nroRegistros++;
    }

    // Atualiza o cabeçalho
    cabecalhoVeiculo.byteProxReg = ftell(binario);
    cabecalhoVeiculo.status = '1';
    fseek(binario, 0, SEEK_SET);
    escreveCabecalhoVeiculoBinario(&cabecalhoVeiculo, binario);

    fclose(binario);
    binarioNaTela(nomeBinario);
}

void insertIntoLinha() {
    char nomeBinario[255];

    // Recebe o nome do arquivo
    if (scanf("%s", nomeBinario) != 1) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Abre o binário
    FILE *binario = fopen(nomeBinario, "rb+");
    if (binario == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Lê o Cabeçalho da Linha
    CabecalhoLinha cabecalhoLinha;
    leCabecalhoLinhaBinario(&cabecalhoLinha, binario);
    atualizaStatusBinario('0', binario);

    // Checa a integridade do arquivo
    if (arquivoFoiCorrompido(cabecalhoLinha.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(binario);
        exit(0);
    }

    // Recebe o número de inserções e posiciona o arquivo
    int insercoes;
    scanf("%d", &insercoes);
    fseek(binario, cabecalhoLinha.byteProxReg, SEEK_SET);

    // Percorre a entrada escrevendo os registros
    Linha linha;
    for (int i = 0; i < insercoes; i++) {
        leLinhaEntrada(&linha);
        escreveLinhaBinario(&linha, binario);
        cabecalhoLinha.nroRegistros++;
    }

    // Atualiza o cabeçalho
    cabecalhoLinha.byteProxReg = ftell(binario);
    cabecalhoLinha.status = '1';
    fseek(binario, 0, SEEK_SET);
    escreveCabecalhoLinhaBinario(&cabecalhoLinha, binario);

    fclose(binario);
    binarioNaTela(nomeBinario);
}

void createTableIndexVeiculo() {
    char nomeVeiculosBinario[255];
    char nomeArvoreB[255];

    // Recebe o nome do arquivo
    if (scanf("%s %s", nomeVeiculosBinario, nomeArvoreB) != 2) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Abre o binário
    FILE *veiculosBinario = fopen(nomeVeiculosBinario, "rb");
    if (veiculosBinario == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Abre o binário
    FILE *arvoreB = fopen(nomeArvoreB, "wb+");
    if (arvoreB == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Lê o Cabeçalho do Veículo
    CabecalhoVeiculo cabecalhoVeiculo;
    leCabecalhoVeiculoBinario(&cabecalhoVeiculo, veiculosBinario);

    // Checa a integridade do arquivo
    if (arquivoFoiCorrompido(cabecalhoVeiculo.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(veiculosBinario);
        fclose(arvoreB);
        exit(0);
    }

    // Checa a existência de registros não removidos
    if (cabecalhoVeiculo.nroRegistros == 0) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(veiculosBinario);
        fclose(arvoreB);
        exit(0);
    }

    CabecalhoArvoreB cabecalhoArvoreB;
    criaCabecalhoArvoreB(&cabecalhoArvoreB);
    escreveCabecalhoArvoreB(cabecalhoArvoreB, arvoreB);

    Veiculo veiculo;
    int nroTotalRegistros = cabecalhoVeiculo.nroRegistros + cabecalhoVeiculo.nroRegRemovidos;

    // Percorre os registros
    for (int i = 0; i < nroTotalRegistros; i++) {
        long long int offsetAtual = ftell(veiculosBinario);

        // Caso registro lido não removido
        if (leVeiculoBinario(&veiculo, veiculosBinario)) {
            // imprimeVeiculo(&cabecalhoVeiculo, &veiculo);
            ChaveArvoreB chave;
            chave.C = convertePrefixo(veiculo.prefixo);
            chave.PR = offsetAtual;
            insereArvoreB(chave, &cabecalhoArvoreB, arvoreB);
        }

        // Caso removido, pula corpo
        else {
            fseek(veiculosBinario, veiculo.tamanhoRegistro, SEEK_CUR);
        }
    }

    cabecalhoArvoreB.status = '1';
    escreveCabecalhoArvoreB(cabecalhoArvoreB, arvoreB);

    fclose(veiculosBinario);
    fclose(arvoreB);

    binarioNaTela(nomeArvoreB);
}

void createTableIndexLinha() {
    char nomeLinhaBinario[255];
    char nomeArvoreB[255];

    // Recebe o nome do arquivo
    if (scanf("%s %s", nomeLinhaBinario, nomeArvoreB) != 2) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Abre o binário
    FILE *linhaBinario = fopen(nomeLinhaBinario, "rb");
    if (linhaBinario == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Abre o binário
    FILE *arvoreB = fopen(nomeArvoreB, "wb+");
    if (arvoreB == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Lê o Cabeçalho do Veículo
    CabecalhoLinha cabecalhoLinha;
    leCabecalhoLinhaBinario(&cabecalhoLinha, linhaBinario);

    // Checa a integridade do arquivo
    if (arquivoFoiCorrompido(cabecalhoLinha.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(linhaBinario);
        fclose(arvoreB);
        exit(0);
    }

    // Checa a existência de registros não removidos
    if (cabecalhoLinha.nroRegistros == 0) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(linhaBinario);
        fclose(arvoreB);
        exit(0);
    }

    CabecalhoArvoreB cabecalhoArvoreB;
    criaCabecalhoArvoreB(&cabecalhoArvoreB);
    escreveCabecalhoArvoreB(cabecalhoArvoreB, arvoreB);

    Linha linha;
    int nroTotalRegistros = cabecalhoLinha.nroRegistros + cabecalhoLinha.nroRegRemovidos;

    // Percorre os registros
    for (int i = 0; i < nroTotalRegistros; i++) {
        long long int offsetAtual = ftell(linhaBinario);

        // Caso registro lido não removido
        if (leLinhaBinario(&linha, linhaBinario)) {
            // imprimeVeiculo(&cabecalhoVeiculo, &veiculo);
            ChaveArvoreB chave;
            chave.C = linha.codLinha;
            chave.PR = offsetAtual;
            insereArvoreB(chave, &cabecalhoArvoreB, arvoreB);
        }

        // Caso removido, pula corpo
        else {
            fseek(linhaBinario, linha.tamanhoRegistro, SEEK_CUR);
        }
    }

    cabecalhoArvoreB.status = '1';
    escreveCabecalhoArvoreB(cabecalhoArvoreB, arvoreB);

    fclose(linhaBinario);
    fclose(arvoreB);

    binarioNaTela(nomeArvoreB);
}