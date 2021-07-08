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
 * Create Index
 *
 **/

// Função 9:
//  -> Lê um arquivo de dados de Veículo e cria um índice de prefixos
//  -> Exibe o binário na tela do arquivo de índice
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

    // Abre a Árvore B
    FILE *arvoreB = fopen(nomeArvoreB, "wb+");
    if (arvoreB == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(veiculosBinario);
        exit(0);
    }

    // Cria e inicializa o cabeçalho da Árvore B
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

            // Insere a chave na Árvore B
            insereArvoreB(chave, &cabecalhoArvoreB, arvoreB);
        }

        // Caso removido, pula corpo
        else {
            fseek(veiculosBinario, veiculo.tamanhoRegistro, SEEK_CUR);
        }
    }

    // Atualiza o cabeçalho da Árvore B
    cabecalhoArvoreB.status = '1';
    escreveCabecalhoArvoreB(cabecalhoArvoreB, arvoreB);

    // Fecha os arquivos
    fclose(veiculosBinario);
    fclose(arvoreB);

    binarioNaTela(nomeArvoreB);
}

// Função 10:
//  -> Lê um arquivo de dados de Linha e cria um índice de códigos de Linha
//  -> Exibe o binário na tela do arquivo de índice
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

    // Abre a Árvore B
    FILE *arvoreB = fopen(nomeArvoreB, "wb+");
    if (arvoreB == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(linhasBinario);
        exit(0);
    }

    // Cria e inicializa o cabeçalho da Árvore B
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

            // Insere a chave na Árvore B
            insereArvoreB(chave, &cabecalhoArvoreB, arvoreB);
        }

        // Caso removido, pula corpo
        else {
            fseek(linhasBinario, linha.tamanhoRegistro, SEEK_CUR);
        }
    }

    // Atualiza o cabeçalho da Árvore B
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
 **/

// Função 11:
//  -> Lê um arquivo de dados de Veículo, um índice de prefixos,
//     um campo a pesquisar e um valor para o campo
//  -> Exibe o Veículo (caso encontrado)
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

    // Abre a Árvore B
    FILE *arvoreB = fopen(nomeArvoreB, "rb");
    if (arvoreB == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(veiculosBinario);
        exit(0);
    }

    // Lê o cabeçalho da Árvore B
    CabecalhoArvoreB cabecalhoArvoreB;
    leCabecalhoArvoreB(&cabecalhoArvoreB, arvoreB);

    // Checa a integridade da Árvore B
    if (arquivoFoiCorrompido(cabecalhoArvoreB.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(veiculosBinario);
        fclose(arvoreB);
        exit(0);
    }

    // Busca na Árvore B
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

// Função 12:
//  -> Lê um arquivo de dados de Linha, um índice de códigos de Linha,
//     um campo a pesquisar e um valor para o campo
//  -> Exibe a Linha (caso encontrada)
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

    // Abre a Árvore B
    FILE *arvoreB = fopen(nomeArvoreB, "rb");
    if (arvoreB == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(linhasBinario);
        exit(0);
    }

    // Lê o cabeçalho da Árvore B
    CabecalhoArvoreB cabecalhoArvoreB;
    leCabecalhoArvoreB(&cabecalhoArvoreB, arvoreB);

    // Checa a integridade da Árvore B
    if (arquivoFoiCorrompido(cabecalhoArvoreB.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(linhasBinario);
        fclose(arvoreB);
        exit(0);
    }

    // Busca na Árvore B
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
 **/

// Função 13:
//  ->  Lê um arquivo de dados de Veículo, um índice de prefixos,
//      um número de elementos a serem inseridos e os elementos
//  ->  Exibe o binário na tela do arquivo de índice
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

    // Abre a Árvore B
    FILE *arvoreB = fopen(nomeArvoreB, "rb+");
    if (arvoreB == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(veiculosBinario);
        exit(0);
    }

    // Lê o cabeçalho da Árvore B
    CabecalhoArvoreB cabecalhoArvoreB;
    leCabecalhoArvoreB(&cabecalhoArvoreB, arvoreB);

    // Checa a integridade da Árvore B
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

        // Insere a chave na Árvore B
        insereArvoreB(chave, &cabecalhoArvoreB, arvoreB);
    }

    // Atualiza o cabeçalho do veículo
    cabecalhoVeiculo.status = '1';
    cabecalhoVeiculo.byteProxReg = ftell(veiculosBinario);
    fseek(veiculosBinario, 0, SEEK_SET);
    escreveCabecalhoVeiculoBinario(&cabecalhoVeiculo, veiculosBinario);

    // Atualiza o cabeçalho da Árvore B
    cabecalhoArvoreB.status = '1';
    escreveCabecalhoArvoreB(cabecalhoArvoreB, arvoreB);

    // Fecha os arquivos
    fclose(veiculosBinario);
    fclose(arvoreB);

    binarioNaTela(nomeArvoreB);
}

// Função 14:
//  ->  Lê um arquivo de dados de Linha, um índice de códigos de Linha,
//      um número de elementos a serem inseridos e os elementos
//  ->  Exibe o binário na tela do arquivo de índice
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

    // Abre a Árvore B
    FILE *arvoreB = fopen(nomeArvoreB, "rb+");
    if (arvoreB == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(linhasBinario);
        exit(0);
    }

    // Lê o cabeçalho da Árvore B
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

        // Insere a chave na Árvore B
        insereArvoreB(chave, &cabecalhoArvoreB, arvoreB);
    }

    // Atualiza o cabeçalho da linha
    cabecalhoLinha.status = '1';
    cabecalhoLinha.byteProxReg = ftell(linhasBinario);
    fseek(linhasBinario, 0, SEEK_SET);
    escreveCabecalhoLinhaBinario(&cabecalhoLinha, linhasBinario);

    // Atualiza o cabeçalho da Árvore B
    cabecalhoArvoreB.status = '1';
    escreveCabecalhoArvoreB(cabecalhoArvoreB, arvoreB);

    // Fecha os arquivos
    fclose(linhasBinario);
    fclose(arvoreB);

    binarioNaTela(nomeArvoreB);
}