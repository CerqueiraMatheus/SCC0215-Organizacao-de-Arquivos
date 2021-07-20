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


/**
 *
 * Create Index
 * 
 */

void createIndexVeiculo() {
    char nomeVeiculosBinario[255];
    char nomeArvoreB[255];

    // Recebe o nome dos arquivos
    if (scanf("%s %s", nomeVeiculosBinario, nomeArvoreB) != 2) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Abre o binário veículo
    FILE *veiculosBinario = fopen(nomeVeiculosBinario, "rb");
    if (veiculosBinario == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Lê o cabeçalho do veículo
    CabecalhoVeiculo cabecalhoVeiculo;
    leCabecalhoVeiculoBinario(&cabecalhoVeiculo, veiculosBinario);

    // Checa a integridade do binário veículo
    if (arquivoFoiCorrompido(cabecalhoVeiculo.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(veiculosBinario);
        exit(0);
    }

    // Checa a existência de registros não removidos
    if (cabecalhoVeiculo.nroRegistros == 0) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(veiculosBinario);
        exit(0);
    }

    // Abre a árvore-B
    FILE *arvoreB = fopen(nomeArvoreB, "wb+");
    if (arvoreB == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(veiculosBinario);
        exit(0);
    }

    // Cria e inicializa o cabeçalho da árvore-B
    CabecalhoArvoreB cabecalhoArvoreB;
    criaCabecalhoArvoreB(&cabecalhoArvoreB);
    escreveCabecalhoArvoreB(cabecalhoArvoreB, arvoreB);

    int nroTotalRegistros = cabecalhoVeiculo.nroRegistros + cabecalhoVeiculo.nroRegRemovidos;

    // Percorre os registros
    for (int i = 0; i < nroTotalRegistros; i++) {
        Veiculo veiculo;
        long long int offset = ftell(veiculosBinario);

        // Caso registro não removido
        if (leVeiculoBinario(&veiculo, veiculosBinario) == true) {
            // Cria a chave a ser inserida
            ChaveArvoreB chave;
            chave.C = convertePrefixo(veiculo.prefixo);
            chave.PR = offset;

            // Insere a chave na árvore-B
            insereArvoreB(chave, &cabecalhoArvoreB, arvoreB);
        }

        // Caso removido, pula corpo
        else {
            fseek(veiculosBinario, veiculo.tamanhoRegistro, SEEK_CUR);
        }
    }

    // Atualiza o cabeçalho da árvore-B
    cabecalhoArvoreB.status = '1';
    escreveCabecalhoArvoreB(cabecalhoArvoreB, arvoreB);

    // Fecha os arquivos
    fclose(veiculosBinario);
    fclose(arvoreB);

    binarioNaTela(nomeArvoreB);
}

void createIndexLinha() {
    char nomeLinhasBinario[255];
    char nomeArvoreB[255];

    // Recebe o nome dos arquivos
    if (scanf("%s %s", nomeLinhasBinario, nomeArvoreB) != 2) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Abre o binário linha
    FILE *linhasBinario = fopen(nomeLinhasBinario, "rb");
    if (linhasBinario == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Lê o cabeçalho da linha
    CabecalhoLinha cabecalhoLinha;
    leCabecalhoLinhaBinario(&cabecalhoLinha, linhasBinario);

    // Checa a integridade do binário linha
    if (arquivoFoiCorrompido(cabecalhoLinha.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(linhasBinario);
        exit(0);
    }

    // Checa a existência de registros não removidos
    if (cabecalhoLinha.nroRegistros == 0) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(linhasBinario);
        exit(0);
    }

    // Abre a árvore-B
    FILE *arvoreB = fopen(nomeArvoreB, "wb+");
    if (arvoreB == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(linhasBinario);
        exit(0);
    }

    // Cria e inicializa o cabeçalho da árvore-B
    CabecalhoArvoreB cabecalhoArvoreB;
    criaCabecalhoArvoreB(&cabecalhoArvoreB);
    escreveCabecalhoArvoreB(cabecalhoArvoreB, arvoreB);

    int nroTotalRegistros = cabecalhoLinha.nroRegistros + cabecalhoLinha.nroRegRemovidos;

    // Percorre os registros
    for (int i = 0; i < nroTotalRegistros; i++) {
        Linha linha;
        long long int offset = ftell(linhasBinario);

        // Caso registro não removido
        if (leLinhaBinario(&linha, linhasBinario) == true) {
            // Cria a chave a ser inserida
            ChaveArvoreB chave;
            chave.C = linha.codLinha;
            chave.PR = offset;

            // Insere a chave na árvore-B
            insereArvoreB(chave, &cabecalhoArvoreB, arvoreB);
        }

        // Caso removido, pula corpo
        else {
            fseek(linhasBinario, linha.tamanhoRegistro, SEEK_CUR);
        }
    }

    // Atualiza o cabeçalho da árvore-B
    cabecalhoArvoreB.status = '1';
    escreveCabecalhoArvoreB(cabecalhoArvoreB, arvoreB);

    // Fecha os arquivos
    fclose(linhasBinario);
    fclose(arvoreB);

    binarioNaTela(nomeArvoreB);
}


/**
 *
 * Select From Where Index
 * 
 */

void selectFromWhereIndexVeiculo() {
    char nomeVeiculosBinario[255];
    char nomeArvoreB[255];
    char campo[20];

    // Recebe os nomes dos arquivos e do campo
    if (scanf("%s %s %s", nomeVeiculosBinario, nomeArvoreB, campo) != 3) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Checa se o campo é chave primária
    if (strcmp(campo, "prefixo") != 0) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Recebe o prefixo
    char prefixo[6];
    scan_quote_string(prefixo);

    // Abre o binário veículo
    FILE *veiculosBinario = fopen(nomeVeiculosBinario, "rb");
    if (veiculosBinario == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Lê o cabeçalho do veículo
    CabecalhoVeiculo cabecalhoVeiculo;
    leCabecalhoVeiculoBinario(&cabecalhoVeiculo, veiculosBinario);

    // Checa a integridade do binário veículo
    if (arquivoFoiCorrompido(cabecalhoVeiculo.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(veiculosBinario);
        exit(0);
    }

    // Abre a árvore-B
    FILE *arvoreB = fopen(nomeArvoreB, "rb");
    if (arvoreB == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(veiculosBinario);
        exit(0);
    }

    // Lê o cabeçalho da árvore-B
    CabecalhoArvoreB cabecalhoArvoreB;
    leCabecalhoArvoreB(&cabecalhoArvoreB, arvoreB);

    // Checa a integridade da árvore-B
    if (arquivoFoiCorrompido(cabecalhoArvoreB.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(veiculosBinario);
        fclose(arvoreB);
        exit(0);
    }

    // Busca na árvore-B
    long long int offset = buscaArvoreB(convertePrefixo(prefixo), cabecalhoArvoreB.noRaiz, arvoreB);

    // Caso encontrado
    if (offset != NAO_ENCONTRADO) {
        // Lê o veículo especificado
        Veiculo veiculo;
        fseek(veiculosBinario, offset, SEEK_SET);
        leVeiculoBinario(&veiculo, veiculosBinario);

        // Caso não removido
        if (!registroFoiRemovido(veiculo.removido)) {
            imprimeVeiculo(&cabecalhoVeiculo, &veiculo);
        }

        // Caso removido
        else {
            printf("%s\n", MENSAGEM_REGISTRO_INEXISTENTE);
        }
    }

    // Caso não encontrado
    else {
        printf("%s\n", MENSAGEM_REGISTRO_INEXISTENTE);
    }

    // Fecha os arquivos
    fclose(veiculosBinario);
    fclose(arvoreB);
}

void selectFromWhereIndexLinha() {
    char nomeLinhasBinario[255];
    char nomeArvoreB[255];
    char campo[20];

    // Recebe os nomes dos arquivos e do campo
    if (scanf("%s %s %s", nomeLinhasBinario, nomeArvoreB, campo) != 3) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Checa se o campo é chave primária
    if (strcmp(campo, "codLinha") != 0) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Recebe o código da linha
    int codLinha;
    scanf("%d", &codLinha);

    // Abre o binário linha
    FILE *linhasBinario = fopen(nomeLinhasBinario, "rb");
    if (linhasBinario == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Lê o cabeçalho da linha
    CabecalhoLinha cabecalhoLinha;
    leCabecalhoLinhaBinario(&cabecalhoLinha, linhasBinario);

    // Checa a integridade do binário linha
    if (arquivoFoiCorrompido(cabecalhoLinha.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(linhasBinario);
        exit(0);
    }

    // Abre a árvore-B
    FILE *arvoreB = fopen(nomeArvoreB, "rb");
    if (arvoreB == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(linhasBinario);
        exit(0);
    }

    // Lê o cabeçalho da árvore-B
    CabecalhoArvoreB cabecalhoArvoreB;
    leCabecalhoArvoreB(&cabecalhoArvoreB, arvoreB);

    // Checa a integridade da árvore-B
    if (arquivoFoiCorrompido(cabecalhoArvoreB.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(linhasBinario);
        fclose(arvoreB);
        exit(0);
    }

    // Busca na árvore-B
    long long int offset = buscaArvoreB(codLinha, cabecalhoArvoreB.noRaiz, arvoreB);

    // Caso encontrado
    if (offset != NAO_ENCONTRADO) {
        // Lê a linha especificada
        Linha linha;
        fseek(linhasBinario, offset, SEEK_SET);
        leLinhaBinario(&linha, linhasBinario);

        // Caso não removido
        if (!registroFoiRemovido(linha.removido)) {
            imprimeLinha(&cabecalhoLinha, &linha);
        }

        // Caso removido
        else {
            printf("%s\n", MENSAGEM_REGISTRO_INEXISTENTE);
        }
    }

    // Caso não encontrado
    else {
        printf("%s\n", MENSAGEM_REGISTRO_INEXISTENTE);
    }

    // Fecha os arquivos
    fclose(linhasBinario);
    fclose(arvoreB);
}


/**
 *
 * Insert Into Index
 * 
 */

void insertIntoIndexVeiculo() {
    char nomeVeiculosBinario[255];
    char nomeArvoreB[255];

    // Recebe os nomes dos arquivos
    if (scanf("%s %s", nomeVeiculosBinario, nomeArvoreB) != 2) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Abre o binário veículo
    FILE *veiculosBinario = fopen(nomeVeiculosBinario, "rb+");
    if (veiculosBinario == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Lê o cabeçalho do veículo
    CabecalhoVeiculo cabecalhoVeiculo;
    leCabecalhoVeiculoBinario(&cabecalhoVeiculo, veiculosBinario);

    // Checa a integridade do binário veículo
    if (arquivoFoiCorrompido(cabecalhoVeiculo.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(veiculosBinario);
        exit(0);
    }
    atualizaStatusBinario('0', veiculosBinario);

    // Abre a árvore-B
    FILE *arvoreB = fopen(nomeArvoreB, "rb+");
    if (arvoreB == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(veiculosBinario);
        exit(0);
    }

    // Lê o cabeçalho da árvore-B
    CabecalhoArvoreB cabecalhoArvoreB;
    leCabecalhoArvoreB(&cabecalhoArvoreB, arvoreB);

    // Checa a integridade da árvore-B
    if (arquivoFoiCorrompido(cabecalhoArvoreB.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(veiculosBinario);
        fclose(arvoreB);
        exit(0);
    }
    atualizaStatusBinario('0', arvoreB);

    // Recebe o número de inserções e posiciona o arquivo
    int insercoes;
    scanf("%d", &insercoes);
    fseek(veiculosBinario, cabecalhoVeiculo.byteProxReg, SEEK_SET);

    // Percorre a entrada escrevendo os registros
    for (int i = 0; i < insercoes; i++) {
        Veiculo veiculo;
        leVeiculoEntrada(&veiculo);

        // Cria a chave a ser inserida
        ChaveArvoreB chave;
        chave.C = convertePrefixo(veiculo.prefixo);
        chave.PR = ftell(veiculosBinario);

        // Escreve o veículo no binário veículo
        escreveVeiculoBinario(&veiculo, veiculosBinario);
        cabecalhoVeiculo.nroRegistros++;

        // Insere a chave na árvore-B
        insereArvoreB(chave, &cabecalhoArvoreB, arvoreB);
    }

    // Atualiza o cabeçalho do veículo
    cabecalhoVeiculo.status = '1';
    cabecalhoVeiculo.byteProxReg = ftell(veiculosBinario);
    fseek(veiculosBinario, 0, SEEK_SET);
    escreveCabecalhoVeiculoBinario(&cabecalhoVeiculo, veiculosBinario);

    // Atualiza o cabeçalho da árvore-B
    cabecalhoArvoreB.status = '1';
    escreveCabecalhoArvoreB(cabecalhoArvoreB, arvoreB);

    // Fecha os arquivos
    fclose(veiculosBinario);
    fclose(arvoreB);

    binarioNaTela(nomeArvoreB);
}

void insertIntoIndexLinha() {
    char nomeLinhasBinario[255];
    char nomeArvoreB[255];

    // Recebe os nome do arquivos
    if (scanf("%s %s", nomeLinhasBinario, nomeArvoreB) != 2) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Abre o binário linha
    FILE *linhasBinario = fopen(nomeLinhasBinario, "rb+");
    if (linhasBinario == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Lê o cabeçalho da linha
    CabecalhoLinha cabecalhoLinha;
    leCabecalhoLinhaBinario(&cabecalhoLinha, linhasBinario);

    // Checa a integridade dos arquivos
    if (arquivoFoiCorrompido(cabecalhoLinha.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(linhasBinario);
        exit(0);
    }
    atualizaStatusBinario('0', linhasBinario);

    // Abre a árvore-B
    FILE *arvoreB = fopen(nomeArvoreB, "rb+");
    if (arvoreB == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(linhasBinario);
        exit(0);
    }

    // Lê o cabeçalho da árvore-B
    CabecalhoArvoreB cabecalhoArvoreB;
    leCabecalhoArvoreB(&cabecalhoArvoreB, arvoreB);

    // Checa a integridade dos arquivos
    if (arquivoFoiCorrompido(cabecalhoArvoreB.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(linhasBinario);
        fclose(arvoreB);
        exit(0);
    }
    atualizaStatusBinario('0', arvoreB);

    // Recebe o número de inserções e posiciona o arquivo
    int insercoes;
    scanf("%d", &insercoes);
    fseek(linhasBinario, cabecalhoLinha.byteProxReg, SEEK_SET);

    // Percorre a entrada escrevendo os registros
    for (int i = 0; i < insercoes; i++) {
        Linha linha;
        leLinhaEntrada(&linha);

        // Cria a chave a ser inserida
        ChaveArvoreB chave;
        chave.C = linha.codLinha;
        chave.PR = ftell(linhasBinario);

        // Escreve a linha no binário linha
        escreveLinhaBinario(&linha, linhasBinario);
        cabecalhoLinha.nroRegistros++;

        // Insere a chave na árvore-B
        insereArvoreB(chave, &cabecalhoArvoreB, arvoreB);
    }

    // Atualiza o cabeçalho da linha
    cabecalhoLinha.status = '1';
    cabecalhoLinha.byteProxReg = ftell(linhasBinario);
    fseek(linhasBinario, 0, SEEK_SET);
    escreveCabecalhoLinhaBinario(&cabecalhoLinha, linhasBinario);

    // Atualiza o cabeçalho da árvore-B
    cabecalhoArvoreB.status = '1';
    escreveCabecalhoArvoreB(cabecalhoArvoreB, arvoreB);

    // Fecha os arquivos
    fclose(linhasBinario);
    fclose(arvoreB);

    binarioNaTela(nomeArvoreB);
}


/**
 * 
 * Order By
 * 
 */

void orderByVeiculo() {
    char nomeArquivoOriginal[255];
    char nomeArquivoOrdenado[255];
    char campo[20];

    if (scanf("%s %s %s", nomeArquivoOriginal, nomeArquivoOrdenado, campo) != 3) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    if (!ehCampoOrdenavel(campo)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    FILE *arquivoOriginal = fopen(nomeArquivoOriginal, "rb");
    if (arquivoOriginal == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    CabecalhoVeiculo cabecalhoOriginal;
    leCabecalhoVeiculoBinario(&cabecalhoOriginal, arquivoOriginal);

    if (arquivoFoiCorrompido(cabecalhoOriginal.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(arquivoOriginal);
        exit(0);
    }

    FILE *arquivoOrdenado = fopen(nomeArquivoOrdenado, "wb");
    if (arquivoOrdenado == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(arquivoOriginal);
        exit(0);
    }

    CabecalhoVeiculo cabecalhoOrdenado;
    criaCabecalhoVeiculoNovo(&cabecalhoOrdenado, cabecalhoOriginal);
    escreveCabecalhoVeiculoBinario(&cabecalhoOrdenado, arquivoOrdenado);

    int nroTotalRegistros = cabecalhoOriginal.nroRegistros + cabecalhoOriginal.nroRegRemovidos;

    Veiculo veiculos[cabecalhoOrdenado.nroRegistros];
    leVeiculosValidosBinario(veiculos, nroTotalRegistros, arquivoOriginal);
    ordenaVeiculos(veiculos, cabecalhoOrdenado.nroRegistros);

    escreveVeiculosBinario(veiculos, cabecalhoOrdenado.nroRegistros, arquivoOrdenado);

    cabecalhoOrdenado.status = '1';
    cabecalhoOrdenado.byteProxReg = ftell(arquivoOrdenado);
    fseek(arquivoOrdenado, 0, SEEK_SET);
    escreveCabecalhoVeiculoBinario(&cabecalhoOrdenado, arquivoOrdenado);

    fclose(arquivoOriginal);
    fclose(arquivoOrdenado);

    binarioNaTela(nomeArquivoOrdenado);
}

void orderByLinha() {

}


/**
 * 
 * Select From Join On
 * 
 */

void selectFromJoinOnLoop() {

}

void selectFromJoinOnIndex() {

}

void selectFromJoinOnMerge() {

}
