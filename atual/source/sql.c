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
#include "arvoreb.h"
#include "linha.h"
#include "util.h"
#include "sql.h"


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
        printf("%s\n", FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Abre os arquivos
    FILE *csv = abreArquivo(nomeCsv, "r", 0);
    FILE *binario = abreArquivo(nomeBinario, "wb", 1, csv);

    // Inicializa o cabeçalho
    CabecalhoVeiculo cabecalhoVeiculo = leCabecalhoVeiculoCsv(csv);
    escreveCabecalhoVeiculoBinario(cabecalhoVeiculo, binario);

    // Percorre o CSV escrevendo os registros
    while (true) {
        bool ehEOF = false;
        Veiculo veiculo = leVeiculoCsv(csv, &ehEOF);

        if (ehEOF) {
            break;
        }

        escreveVeiculoBinario(veiculo, binario);
        if (registroFoiRemovido(veiculo.removido)) {
            cabecalhoVeiculo.nroRegRemovidos++;
        }
        else {
            cabecalhoVeiculo.nroRegistros++;
        }
    }

    // Atualiza o cabeçalho
    cabecalhoVeiculo.byteProxReg = ftell(binario);
    cabecalhoVeiculo.status = '1';
    escreveCabecalhoVeiculoBinario(cabecalhoVeiculo, binario);

    fclose(csv);
    fclose(binario);
    binarioNaTela(nomeBinario);
}

void createTableLinha() {
    char nomeCsv[255];
    char nomeBinario[255];

    // Recebe os nomes dos arquivos
    if (scanf("%s %s", nomeCsv, nomeBinario) != 2) {
        printf("%s\n", FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Abre os arquivos
    FILE *csv = abreArquivo(nomeCsv, "r", 0);
    FILE *binario = abreArquivo(nomeBinario, "wb", 1, csv);

    // Inicializa o cabeçalho
    CabecalhoLinha cabecalhoLinha = leCabecalhoLinhaCsv(csv);
    escreveCabecalhoLinhaBinario(cabecalhoLinha, binario);

    // Percorre o CSV escrevendo os registros
    while (true) {
        bool ehEOF = false;
        Linha linha = leLinhaCsv(csv, &ehEOF);
        
        if (ehEOF) {
            break;
        }

        escreveLinhaBinario(linha, binario);
        if (registroFoiRemovido(linha.removido)) {
            cabecalhoLinha.nroRegRemovidos++;
        }
        else {
            cabecalhoLinha.nroRegistros++;
        }
    }

    // Atualiza o cabeçalho
    cabecalhoLinha.byteProxReg = ftell(binario);
    cabecalhoLinha.status = '1';
    escreveCabecalhoLinhaBinario(cabecalhoLinha, binario);

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
        printf("%s\n", FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Abre o binário
    FILE *binario = abreArquivo(nomeBinario, "rb+", 0);
    CabecalhoVeiculo cabecalhoVeiculo = leCabecalhoVeiculoBinario(binario);
    validaArquivo(cabecalhoVeiculo.status, 1, binario);
    atualizaStatusBinario('0', binario);

    // Checa a existência de registros não removidos
    if (cabecalhoVeiculo.nroRegistros == 0) {
        printf("%s\n", REGISTRO_INEXISTENTE);
        atualizaStatusBinario('1', binario);
        fclose(binario);
        exit(0);
    }

    int nroTotalRegistros = cabecalhoVeiculo.nroRegistros + cabecalhoVeiculo.nroRegRemovidos;

    // Percorre os registros
    for (int i = 0; i < nroTotalRegistros; i++) {
        Veiculo veiculo = leVeiculoBinario(binario);

        // Caso registro lido não removido
        if (!registroFoiRemovido(veiculo.removido)) {
            imprimeVeiculo(veiculo, cabecalhoVeiculo);
        }
    }

    atualizaStatusBinario('1', binario);
    fclose(binario);
}

void selectFromLinha() {
    char nomeBinario[255];

    // Recebe o nome do arquivo
    if (scanf("%s", nomeBinario) != 1) {
        printf("%s\n", FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Abre o binário
    FILE *binario = abreArquivo(nomeBinario, "rb+", 0);
    CabecalhoLinha cabecalhoLinha = leCabecalhoLinhaBinario(binario);
    validaArquivo(cabecalhoLinha.status, 1, binario);
    atualizaStatusBinario('0', binario);

    // Checa a existência de registros não removidos
    if (cabecalhoLinha.nroRegistros == 0) {
        printf("%s\n", REGISTRO_INEXISTENTE);
        atualizaStatusBinario('1', binario);
        fclose(binario);
        exit(0);
    }

    int nroTotalRegistros = cabecalhoLinha.nroRegistros + cabecalhoLinha.nroRegRemovidos;

    // Percorre os registros
    for (int i = 0; i < nroTotalRegistros; i++) {
        Linha linha = leLinhaBinario(binario);

        // Caso registro lido não removido
        if (!registroFoiRemovido(linha.removido)) {
            imprimeLinha(linha, cabecalhoLinha);
        }
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
        printf("%s\n", FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Recebe o valor
    char valor[100];
    scan_quote_string(valor);

    // Abre o binário
    FILE *binario = abreArquivo(nomeBinario, "rb+", 0);
    CabecalhoVeiculo cabecalhoVeiculo = leCabecalhoVeiculoBinario(binario);
    validaArquivo(cabecalhoVeiculo.status, 1, binario);
    atualizaStatusBinario('0', binario);

    // Checa a existência de registros não removidos
    if (cabecalhoVeiculo.nroRegistros == 0) {
        printf("%s\n", REGISTRO_INEXISTENTE);
        atualizaStatusBinario('1', binario);
        fclose(binario);
        exit(0);
    }

    int nroTotalRegistros = cabecalhoVeiculo.nroRegistros + cabecalhoVeiculo.nroRegRemovidos;

    // Flag para indicar se houve ao menos um registro encontrado
    bool houveCorrespondencia = false;

    // Percorre os registros
    for (int i = 0; i < nroTotalRegistros; i++) {
        Veiculo veiculo = leVeiculoBinario(binario);

        // Caso registro lido não removido
        if (!registroFoiRemovido(veiculo.removido)) {
            // Checa valor desejado
            if (comparaCampoVeiculo(&veiculo, campo, valor)) {
                imprimeVeiculo(veiculo, cabecalhoVeiculo);
                houveCorrespondencia = true;
            }
        }
    }

    if (!houveCorrespondencia) {
        printf("%s\n", REGISTRO_INEXISTENTE);
    }

    atualizaStatusBinario('1', binario);
    fclose(binario);
}

void selectFromWhereLinha() {
    char nomeBinario[255];
    char campo[20];

    // Recebe os nomes do arquivo e do campo
    if (scanf("%s %s", nomeBinario, campo) != 2) {
        printf("%s\n", FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Recebe o valor
    char valor[100];
    scan_quote_string(valor);

    // Abre o binário
    FILE *binario = abreArquivo(nomeBinario, "rb+", 0);
    CabecalhoLinha cabecalhoLinha = leCabecalhoLinhaBinario(binario);
    validaArquivo(cabecalhoLinha.status, 1, binario);
    atualizaStatusBinario('0', binario);

    // Checa a existência de registros não removidos
    if (cabecalhoLinha.nroRegistros == 0) {
        printf("%s\n", REGISTRO_INEXISTENTE);
        atualizaStatusBinario('1', binario);
        fclose(binario);
        exit(0);
    }

    int nroTotalRegistros = cabecalhoLinha.nroRegistros + cabecalhoLinha.nroRegRemovidos;

    // Flag para indicar se houve ao menos um registro encontrado
    bool houveCorrespondencia = false;

    // Percorre os registros
    for (int i = 0; i < nroTotalRegistros; i++) {
        Linha linha = leLinhaBinario(binario);

        // Caso registro lido não removido
        if (!registroFoiRemovido(linha.removido)) {
            // Checa valor desejado
            if (comparaCampoLinha(&linha, campo, valor)) {
                imprimeLinha(linha, cabecalhoLinha);
                houveCorrespondencia = true;
            }
        }
    }

    if (!houveCorrespondencia) {
        printf("%s\n", REGISTRO_INEXISTENTE);
    }

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
        printf("%s\n", FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Abre o binário
    FILE *binario = abreArquivo(nomeBinario, "rb+", 0);
    CabecalhoVeiculo cabecalhoVeiculo = leCabecalhoVeiculoBinario(binario);
    validaArquivo(cabecalhoVeiculo.status, 1, binario);
    atualizaStatusBinario('0', binario);

    // Recebe o número de inserções e posiciona o arquivo
    int insercoes;
    scanf("%d", &insercoes);
    posicionaBinarioProximoRegistroVeiculo(binario, cabecalhoVeiculo);

    // Percorre a entrada escrevendo os registros
    for (int i = 0; i < insercoes; i++) {
        Veiculo veiculo = leVeiculoEntrada();
        escreveVeiculoBinario(veiculo, binario);
        cabecalhoVeiculo.nroRegistros++;
    }

    // Atualiza o cabeçalho
    cabecalhoVeiculo.byteProxReg = ftell(binario);
    cabecalhoVeiculo.status = '1';
    escreveCabecalhoVeiculoBinario(cabecalhoVeiculo, binario);

    fclose(binario);
    binarioNaTela(nomeBinario);
}

void insertIntoLinha() {
    char nomeBinario[255];

    // Recebe o nome do arquivo
    if (scanf("%s", nomeBinario) != 1) {
        printf("%s\n", FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Abre o binário
    FILE *binario = abreArquivo(nomeBinario, "rb+", 0);
    CabecalhoLinha cabecalhoLinha = leCabecalhoLinhaBinario(binario);
    validaArquivo(cabecalhoLinha.status, 1, binario);
    atualizaStatusBinario('0', binario);

    // Recebe o número de inserções e posiciona o arquivo
    int insercoes;
    scanf("%d", &insercoes);
    posicionaBinarioProximoRegistroLinha(binario, cabecalhoLinha);

    // Percorre a entrada escrevendo os registros
    for (int i = 0; i < insercoes; i++) {
        Linha linha = leLinhaEntrada();
        escreveLinhaBinario(linha, binario);
        cabecalhoLinha.nroRegistros++;
    }

    // Atualiza o cabeçalho
    cabecalhoLinha.byteProxReg = ftell(binario);
    cabecalhoLinha.status = '1';
    escreveCabecalhoLinhaBinario(cabecalhoLinha, binario);

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
        printf("%s\n", FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Abre o binário veículo
    FILE *veiculosBinario = abreArquivo(nomeVeiculosBinario, "rb", 0);
    CabecalhoVeiculo cabecalhoVeiculo = leCabecalhoVeiculoBinario(veiculosBinario);
    validaArquivo(cabecalhoVeiculo.status, 1, veiculosBinario);

    // Checa a existência de registros não removidos
    if (cabecalhoVeiculo.nroRegistros == 0) {
        printf("%s\n", FALHA_PROCESSAMENTO);
        fclose(veiculosBinario);
        exit(0);
    }

    // Abre a árvore-B
    FILE *arvoreB = abreArquivo(nomeArvoreB, "wb+", 1, veiculosBinario);

    // Cria e inicializa o cabeçalho da árvore-B
    CabecalhoArvoreB cabecalhoArvoreB = criaCabecalhoArvoreB();
    escreveCabecalhoArvoreB(cabecalhoArvoreB, arvoreB);

    int nroTotalRegistros = cabecalhoVeiculo.nroRegistros + cabecalhoVeiculo.nroRegRemovidos;

    // Percorre os registros
    for (int i = 0; i < nroTotalRegistros; i++) {
        long long int offset = ftell(veiculosBinario);
        Veiculo veiculo = leVeiculoBinario(veiculosBinario);

        // Caso registro não removido
        if (!registroFoiRemovido(veiculo.removido)) {
            // Cria a chave a ser inserida
            ChaveArvoreB chave;
            chave.C = convertePrefixo(veiculo.prefixo);
            chave.PR = offset;

            // Insere a chave na árvore-B
            insereArvoreB(chave, &cabecalhoArvoreB, arvoreB);
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
        printf("%s\n", FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Abre o binário linha
    FILE *linhasBinario = abreArquivo(nomeLinhasBinario, "rb", 0);
    CabecalhoLinha cabecalhoLinha = leCabecalhoLinhaBinario(linhasBinario);
    validaArquivo(cabecalhoLinha.status, 1, linhasBinario);

    // Checa a existência de registros não removidos
    if (cabecalhoLinha.nroRegistros == 0) {
        printf("%s\n", FALHA_PROCESSAMENTO);
        fclose(linhasBinario);
        exit(0);
    }

    // Abre a árvore-B
    FILE *arvoreB = abreArquivo(nomeArvoreB, "wb+", 1, linhasBinario);

    // Cria e inicializa o cabeçalho da árvore-B
    CabecalhoArvoreB cabecalhoArvoreB = criaCabecalhoArvoreB();
    escreveCabecalhoArvoreB(cabecalhoArvoreB, arvoreB);

    int nroTotalRegistros = cabecalhoLinha.nroRegistros + cabecalhoLinha.nroRegRemovidos;

    // Percorre os registros
    for (int i = 0; i < nroTotalRegistros; i++) {
        long long int offset = ftell(linhasBinario);
        Linha linha = leLinhaBinario(linhasBinario);

        // Caso registro não removido
        if (!registroFoiRemovido(linha.removido)) {
            // Cria a chave a ser inserida
            ChaveArvoreB chave;
            chave.C = linha.codLinha;
            chave.PR = offset;

            // Insere a chave na árvore-B
            insereArvoreB(chave, &cabecalhoArvoreB, arvoreB);
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
        printf("%s\n", FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Checa se o campo é chave primária
    if (strcmp(campo, "prefixo") != 0) {
        printf("%s\n", FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Recebe o prefixo
    char prefixo[6];
    scan_quote_string(prefixo);

    // Abre o binário veículo
    FILE *veiculosBinario = abreArquivo(nomeVeiculosBinario, "rb", 0);
    CabecalhoVeiculo cabecalhoVeiculo = leCabecalhoVeiculoBinario(veiculosBinario);
    validaArquivo(cabecalhoVeiculo.status, 1, veiculosBinario);

    // Abre a árvore-B
    FILE *arvoreB = abreArquivo(nomeArvoreB, "rb", 1, veiculosBinario);
    CabecalhoArvoreB cabecalhoArvoreB = leCabecalhoArvoreB(arvoreB);
    validaArquivo(cabecalhoArvoreB.status, 2, veiculosBinario, arvoreB);

    // Busca na árvore-B
    long long int offset = buscaArvoreB(convertePrefixo(prefixo), cabecalhoArvoreB.noRaiz, arvoreB);

    // Caso encontrado
    if (offset != NAO_ENCONTRADO) {
        // Lê o veículo especificado
        fseek(veiculosBinario, offset, SEEK_SET);
        Veiculo veiculo = leVeiculoBinario(veiculosBinario);

        // Caso não removido
        if (!registroFoiRemovido(veiculo.removido)) {
            imprimeVeiculo(veiculo, cabecalhoVeiculo);
        }
        // Caso removido
        else {
            printf("%s\n", REGISTRO_INEXISTENTE);
        }
    }
    // Caso não encontrado
    else {
        printf("%s\n", REGISTRO_INEXISTENTE);
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
        printf("%s\n", FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Checa se o campo é chave primária
    if (strcmp(campo, "codLinha") != 0) {
        printf("%s\n", FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Recebe o código da linha
    int codLinha;
    scanf("%d", &codLinha);

    // Abre o binário linha
    FILE *linhasBinario = abreArquivo(nomeLinhasBinario, "rb", 0);
    CabecalhoLinha cabecalhoLinha = leCabecalhoLinhaBinario(linhasBinario);
    validaArquivo(cabecalhoLinha.status, 1, linhasBinario);

    // Abre a árvore-B
    FILE *arvoreB = abreArquivo(nomeArvoreB, "rb", 1, linhasBinario);
    CabecalhoArvoreB cabecalhoArvoreB = leCabecalhoArvoreB(arvoreB);
    validaArquivo(cabecalhoArvoreB.status, 2, linhasBinario, arvoreB);

    // Busca na árvore-B
    long long int offset = buscaArvoreB(codLinha, cabecalhoArvoreB.noRaiz, arvoreB);

    // Caso encontrado
    if (offset != NAO_ENCONTRADO) {
        // Lê a linha especificada
        fseek(linhasBinario, offset, SEEK_SET);
        Linha linha = leLinhaBinario(linhasBinario);

        // Caso não removido
        if (!registroFoiRemovido(linha.removido)) {
            imprimeLinha(linha, cabecalhoLinha);
        }
        // Caso removido
        else {
            printf("%s\n", REGISTRO_INEXISTENTE);
        }
    }
    // Caso não encontrado
    else {
        printf("%s\n", REGISTRO_INEXISTENTE);
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
        printf("%s\n", FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Abre o binário veículo
    FILE *veiculosBinario = abreArquivo(nomeVeiculosBinario, "rb+", 0);
    CabecalhoVeiculo cabecalhoVeiculo = leCabecalhoVeiculoBinario(veiculosBinario);
    validaArquivo(cabecalhoVeiculo.status, 1, veiculosBinario);
    atualizaStatusBinario('0', veiculosBinario);

    // Abre a árvore-B
    FILE *arvoreB = abreArquivo(nomeArvoreB, "rb+", 1, veiculosBinario);
    CabecalhoArvoreB cabecalhoArvoreB = leCabecalhoArvoreB(arvoreB);
    validaArquivo(cabecalhoArvoreB.status, 2, veiculosBinario, arvoreB);
    atualizaStatusBinario('0', arvoreB);

    // Recebe o número de inserções e posiciona o arquivo
    int insercoes;
    scanf("%d", &insercoes);
    posicionaBinarioProximoRegistroVeiculo(veiculosBinario, cabecalhoVeiculo);

    // Percorre a entrada escrevendo os registros
    for (int i = 0; i < insercoes; i++) {
        Veiculo veiculo = leVeiculoEntrada();

        // Cria a chave a ser inserida
        ChaveArvoreB chave;
        chave.C = convertePrefixo(veiculo.prefixo);
        chave.PR = ftell(veiculosBinario);

        // Escreve o veículo no binário veículo
        escreveVeiculoBinario(veiculo, veiculosBinario);
        cabecalhoVeiculo.nroRegistros++;

        // Insere a chave na árvore-B
        insereArvoreB(chave, &cabecalhoArvoreB, arvoreB);
    }

    // Atualiza o cabeçalho do veículo
    cabecalhoVeiculo.status = '1';
    cabecalhoVeiculo.byteProxReg = ftell(veiculosBinario);
    escreveCabecalhoVeiculoBinario(cabecalhoVeiculo, veiculosBinario);

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
        printf("%s\n", FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Abre o binário linha
    FILE *linhasBinario = abreArquivo(nomeLinhasBinario, "rb+", 0);
    CabecalhoLinha cabecalhoLinha = leCabecalhoLinhaBinario(linhasBinario);
    validaArquivo(cabecalhoLinha.status, 1, linhasBinario);
    atualizaStatusBinario('0', linhasBinario);

    // Abre a árvore-B
    FILE *arvoreB = abreArquivo(nomeArvoreB, "rb+", 1, linhasBinario);
    CabecalhoArvoreB cabecalhoArvoreB = leCabecalhoArvoreB(arvoreB);
    validaArquivo(cabecalhoArvoreB.status, 2, linhasBinario, arvoreB);
    atualizaStatusBinario('0', arvoreB);

    // Recebe o número de inserções e posiciona o arquivo
    int insercoes;
    scanf("%d", &insercoes);
    posicionaBinarioProximoRegistroLinha(linhasBinario, cabecalhoLinha);

    // Percorre a entrada escrevendo os registros
    for (int i = 0; i < insercoes; i++) {
        Linha linha = leLinhaEntrada();

        // Cria a chave a ser inserida
        ChaveArvoreB chave;
        chave.C = linha.codLinha;
        chave.PR = ftell(linhasBinario);

        // Escreve a linha no binário linha
        escreveLinhaBinario(linha, linhasBinario);
        cabecalhoLinha.nroRegistros++;

        // Insere a chave na árvore-B
        insereArvoreB(chave, &cabecalhoArvoreB, arvoreB);
    }

    // Atualiza o cabeçalho da linha
    cabecalhoLinha.status = '1';
    cabecalhoLinha.byteProxReg = ftell(linhasBinario);
    escreveCabecalhoLinhaBinario(cabecalhoLinha, linhasBinario);

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
        printf("%s\n", FALHA_PROCESSAMENTO);
        exit(0);
    }

    if (!ehCampoOrdenavel(campo)) {
        printf("%s\n", FALHA_PROCESSAMENTO);
        exit(0);
    }

    FILE *arquivoOriginal = abreArquivo(nomeArquivoOriginal, "rb", 0);
    CabecalhoVeiculo cabecalhoOriginal = leCabecalhoVeiculoBinario(arquivoOriginal);
    validaArquivo(cabecalhoOriginal.status, 1, arquivoOriginal);

    FILE *arquivoOrdenado = abreArquivo(nomeArquivoOrdenado, "wb", 1, arquivoOriginal);

    CabecalhoVeiculo cabecalhoOrdenado = criaCabecalhoVeiculoNovo(cabecalhoOriginal);
    escreveCabecalhoVeiculoBinario(cabecalhoOrdenado, arquivoOrdenado);

    int nroTotalRegistros = cabecalhoOriginal.nroRegistros + cabecalhoOriginal.nroRegRemovidos;

    Veiculo veiculos[cabecalhoOrdenado.nroRegistros];
    leVeiculosBinario(veiculos, nroTotalRegistros, arquivoOriginal);
    ordenaVeiculos(veiculos, cabecalhoOrdenado.nroRegistros);

    escreveVeiculosBinario(veiculos, cabecalhoOrdenado.nroRegistros, arquivoOrdenado);

    cabecalhoOrdenado.status = '1';
    cabecalhoOrdenado.byteProxReg = ftell(arquivoOrdenado);
    escreveCabecalhoVeiculoBinario(cabecalhoOrdenado, arquivoOrdenado);

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
    char nomeArquivoVeiculo[255];
    char nomeArquivoLinha[255];
    char nomeCampoVeiculo[20];
    char nomeCampoLinha[20];

    // Leitura dos campos
    if (scanf("%s %s %s %s",
              nomeArquivoVeiculo, nomeArquivoLinha, nomeCampoVeiculo, nomeCampoLinha) != 4) {
        printf("%s\n", FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Leitura do arquivo de veículos
    FILE *binarioVeiculo = abreArquivo(nomeArquivoVeiculo, "rb", 0);
    CabecalhoVeiculo cabecalhoVeiculo = leCabecalhoVeiculoBinario(binarioVeiculo);
    validaArquivo(cabecalhoVeiculo.status, 1, binarioVeiculo);

    // Leitura do arquivo de linhas
    FILE *binarioLinha = abreArquivo(nomeArquivoLinha, "rb", 1, binarioVeiculo);
    CabecalhoLinha cabecalhoLinha = leCabecalhoLinhaBinario(binarioLinha);
    validaArquivo(cabecalhoLinha.status, 2, binarioVeiculo, binarioLinha);

    // Checa se os campos são válidos
    if (strcmp(nomeCampoLinha, "codLinha") != 0 ||
        strcmp(nomeCampoLinha, nomeCampoVeiculo) != 0) {
        printf("%s\n", FALHA_PROCESSAMENTO);
        fclose(binarioVeiculo);
        fclose(binarioLinha);
        exit(0);
    }

    // Caso não haja veículo ou linha
    if (cabecalhoLinha.nroRegistros == 0 || cabecalhoVeiculo.nroRegistros == 0) {
        printf("%s\n", REGISTRO_INEXISTENTE);
        fclose(binarioVeiculo);
        fclose(binarioLinha);
        exit(0);
    }

    // Contabiliza o total de registros
    int registrosVeiculo = cabecalhoVeiculo.nroRegistros + cabecalhoVeiculo.nroRegRemovidos;
    int registrosLinha = cabecalhoLinha.nroRegistros + cabecalhoLinha.nroRegRemovidos;

    // Operador para encontro
    bool encontrado = false;

    // Percorre os veículos
    for (int i = 0; i < registrosVeiculo; i++) {
        Veiculo veiculo = leVeiculoBinario(binarioVeiculo);

        // Caso seja um veículo válido
        if (!registroFoiRemovido(veiculo.removido)) {
            // Percorre as linhas
            for (int j = 0; j < registrosLinha; j++) {
                Linha linha = leLinhaBinario(binarioLinha);

                // Caso seja uma linha válida
                if (!registroFoiRemovido(linha.removido)) {
                    // Caso haja o "match"
                    if (veiculo.codLinha == linha.codLinha) {
                        encontrado = true;
                        imprimeVeiculo(veiculo, cabecalhoVeiculo);
                        imprimeLinha(linha, cabecalhoLinha);
                        printf("\n");
                    }
                }
            }

            // Reinicia o arquivo da linha
            posicionaBinarioPrimeiroRegistroLinha(binarioLinha);
        }
    }

    // Caso não tenha ocorrido match
    if (!encontrado) {
        printf("%s\n", REGISTRO_INEXISTENTE);
    }

    // Fecha os arquivos
    fclose(binarioVeiculo);
    fclose(binarioLinha);
}

void selectFromJoinOnIndex() {
    char nomeArquivoVeiculo[255];
    char nomeArquivoLinha[255];
    char nomeCampoVeiculo[20];
    char nomeCampoLinha[20];
    char nomeIndiceLinha[255];

    // Leitura dos campos
    if (scanf("%s %s %s %s %s",
              nomeArquivoVeiculo, nomeArquivoLinha, nomeCampoVeiculo,
              nomeCampoLinha, nomeIndiceLinha) != 5) {
        printf("%s\n", FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Leitura do arquivo de veículos
    FILE *binarioVeiculo = abreArquivo(nomeArquivoVeiculo, "rb", 0);
    CabecalhoVeiculo cabecalhoVeiculo = leCabecalhoVeiculoBinario(binarioVeiculo);
    validaArquivo(cabecalhoVeiculo.status, 1, binarioVeiculo);

    // Leitura do arquivo de linhas
    FILE *binarioLinha = abreArquivo(nomeArquivoLinha, "rb", 1, binarioVeiculo);
    CabecalhoLinha cabecalhoLinha = leCabecalhoLinhaBinario(binarioLinha);
    validaArquivo(cabecalhoLinha.status, 2, binarioVeiculo, binarioLinha);

    // Leitura do arquivo de índice de linhas
    FILE *binarioIndiceLinha = abreArquivo(nomeIndiceLinha, "rb", 2, binarioVeiculo, binarioLinha);
    CabecalhoArvoreB cabecalhoIndiceLinha = leCabecalhoArvoreB(binarioIndiceLinha);
    validaArquivo(cabecalhoIndiceLinha.status, 3, binarioVeiculo, binarioLinha, binarioIndiceLinha);

    // Checa se os campos são válidos
    if (strcmp(nomeCampoLinha, "codLinha") != 0 ||
        strcmp(nomeCampoLinha, nomeCampoVeiculo) != 0) {
        printf("%s\n", FALHA_PROCESSAMENTO);
        fclose(binarioVeiculo);
        fclose(binarioLinha);
        fclose(binarioIndiceLinha);
        exit(0);
    }

    // Caso não haja veículo
    if (cabecalhoVeiculo.nroRegistros == 0) {
        printf("%s\n", REGISTRO_INEXISTENTE);
        fclose(binarioVeiculo);
        fclose(binarioLinha);
        fclose(binarioIndiceLinha);
        exit(0);
    }

    // Contabiliza o total de registros
    int registrosVeiculo = cabecalhoVeiculo.nroRegistros + cabecalhoVeiculo.nroRegRemovidos;

    // Operador para encontro
    bool encontrado = false;

    // Percorre os veículos
    for (int i = 0; i < registrosVeiculo; i++) {
        Veiculo veiculo = leVeiculoBinario(binarioVeiculo);

        // Se o veículo não foi excluído
        if (!registroFoiRemovido(veiculo.removido)) {

            // Caso a busca retorne um offset válido
            long long int offset;
            if ((offset = buscaArvoreB(veiculo.codLinha,
                                       cabecalhoIndiceLinha.noRaiz,
                                       binarioIndiceLinha)) != NAO_ENCONTRADO) {
                                           
                // Executa a leitura da linha
                fseek(binarioLinha, offset, SEEK_SET);
                Linha linha = leLinhaBinario(binarioLinha);

                // Caso a linha seja válida
                if (!registroFoiRemovido(linha.removido)) {
                    encontrado = true;
                    imprimeVeiculo(veiculo, cabecalhoVeiculo);
                    imprimeLinha(linha, cabecalhoLinha);
                    printf("\n");
                }
            }
        }
    }

    // Caso não tenha ocorrido match
    if (!encontrado) {
        printf("%s\n", REGISTRO_INEXISTENTE);
    }

    // Fecha os arquivos
    fclose(binarioVeiculo);
    fclose(binarioLinha);
    fclose(binarioIndiceLinha);
}

void selectFromJoinOnMerge() {

}
