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
    leStringsEntrada(2, nomeCsv, nomeBinario);

    // Abre os arquivos
    FILE *csv = abreArquivo(nomeCsv, "r", 0);
    FILE *binario = abreArquivo(nomeBinario, "wb", 1, csv);

    // Inicializa o cabeçalho
    CabecalhoVeiculo cabecalho = leCabecalhoVeiculoCsv(csv);
    escreveCabecalhoVeiculoBinario(cabecalho, binario);

    // Percorre o CSV escrevendo os registros
    while (true) {
        bool ehEOF = false;
        Veiculo veiculo = leVeiculoCsv(csv, &ehEOF);

        if (ehEOF) {
            break;
        }

        escreveVeiculoBinario(veiculo, binario);
        if (registroFoiRemovido(veiculo.removido)) {
            cabecalho.nroRegRemovidos++;
        }
        else {
            cabecalho.nroRegistros++;
        }
    }

    // Atualiza o cabeçalho
    cabecalho.byteProxReg = ftell(binario);
    cabecalho.status = '1';
    escreveCabecalhoVeiculoBinario(cabecalho, binario);

    fclose(csv);
    fclose(binario);
    binarioNaTela(nomeBinario);
}

void createTableLinha() {
    char nomeCsv[255];
    char nomeBinario[255];

    // Recebe os nomes dos arquivos
    leStringsEntrada(2, nomeCsv, nomeBinario);

    // Abre os arquivos
    FILE *csv = abreArquivo(nomeCsv, "r", 0);
    FILE *binario = abreArquivo(nomeBinario, "wb", 1, csv);

    // Inicializa o cabeçalho
    CabecalhoLinha cabecalho = leCabecalhoLinhaCsv(csv);
    escreveCabecalhoLinhaBinario(cabecalho, binario);

    // Percorre o CSV escrevendo os registros
    while (true) {
        bool ehEOF = false;
        Linha linha = leLinhaCsv(csv, &ehEOF);
        
        if (ehEOF) {
            break;
        }

        escreveLinhaBinario(linha, binario);
        if (registroFoiRemovido(linha.removido)) {
            cabecalho.nroRegRemovidos++;
        }
        else {
            cabecalho.nroRegistros++;
        }
    }

    // Atualiza o cabeçalho
    cabecalho.byteProxReg = ftell(binario);
    cabecalho.status = '1';
    escreveCabecalhoLinhaBinario(cabecalho, binario);

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
    leStringsEntrada(1, nomeBinario);

    // Abre o binário
    FILE *binario = abreArquivo(nomeBinario, "rb+", 0);
    CabecalhoVeiculo cabecalho = leCabecalhoVeiculoBinario(binario);
    validaArquivo(cabecalho.status, 1, binario);
    atualizaStatusBinario('0', binario);

    // Checa a existência de registros não removidos
    if (cabecalho.nroRegistros == 0) {
        printf("%s\n", REGISTRO_INEXISTENTE);
        atualizaStatusBinario('1', binario);
        fclose(binario);
        exit(0);
    }

    int nroTotalRegistros = cabecalho.nroRegistros + cabecalho.nroRegRemovidos;

    // Percorre os registros
    for (int i = 0; i < nroTotalRegistros; i++) {
        Veiculo veiculo = leVeiculoBinario(binario);

        // Caso registro lido não removido
        if (!registroFoiRemovido(veiculo.removido)) {
            imprimeVeiculo(veiculo, cabecalho);
        }
    }

    atualizaStatusBinario('1', binario);
    fclose(binario);
}

void selectFromLinha() {
    char nomeBinario[255];

    // Recebe o nome do arquivo
    leStringsEntrada(1, nomeBinario);

    // Abre o binário
    FILE *binario = abreArquivo(nomeBinario, "rb+", 0);
    CabecalhoLinha cabecalho = leCabecalhoLinhaBinario(binario);
    validaArquivo(cabecalho.status, 1, binario);
    atualizaStatusBinario('0', binario);

    // Checa a existência de registros não removidos
    if (cabecalho.nroRegistros == 0) {
        printf("%s\n", REGISTRO_INEXISTENTE);
        atualizaStatusBinario('1', binario);
        fclose(binario);
        exit(0);
    }

    int nroTotalRegistros = cabecalho.nroRegistros + cabecalho.nroRegRemovidos;

    // Percorre os registros
    for (int i = 0; i < nroTotalRegistros; i++) {
        Linha linha = leLinhaBinario(binario);

        // Caso registro lido não removido
        if (!registroFoiRemovido(linha.removido)) {
            imprimeLinha(linha, cabecalho);
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
    leStringsEntrada(2, nomeBinario, campo);

    // Recebe o valor
    char valor[100];
    scan_quote_string(valor);

    // Abre o binário
    FILE *binario = abreArquivo(nomeBinario, "rb+", 0);
    CabecalhoVeiculo cabecalho = leCabecalhoVeiculoBinario(binario);
    validaArquivo(cabecalho.status, 1, binario);
    atualizaStatusBinario('0', binario);

    // Checa a existência de registros não removidos
    if (cabecalho.nroRegistros == 0) {
        printf("%s\n", REGISTRO_INEXISTENTE);
        atualizaStatusBinario('1', binario);
        fclose(binario);
        exit(0);
    }

    int nroTotalRegistros = cabecalho.nroRegistros + cabecalho.nroRegRemovidos;

    // Flag para indicar se houve ao menos um registro encontrado
    bool encontrado = false;

    // Percorre os registros
    for (int i = 0; i < nroTotalRegistros; i++) {
        Veiculo veiculo = leVeiculoBinario(binario);

        // Caso registro lido não removido
        if (!registroFoiRemovido(veiculo.removido)) {
            // Checa valor desejado
            if (comparaCampoVeiculo(&veiculo, campo, valor)) {
                imprimeVeiculo(veiculo, cabecalho);
                encontrado = true;
            }
        }
    }

    if (!encontrado) {
        printf("%s\n", REGISTRO_INEXISTENTE);
    }

    atualizaStatusBinario('1', binario);
    fclose(binario);
}

void selectFromWhereLinha() {
    char nomeBinario[255];
    char campo[20];

    // Recebe os nomes do arquivo e do campo
    leStringsEntrada(2, nomeBinario, campo);

    // Recebe o valor
    char valor[100];
    scan_quote_string(valor);

    // Abre o binário
    FILE *binario = abreArquivo(nomeBinario, "rb+", 0);
    CabecalhoLinha cabecalho = leCabecalhoLinhaBinario(binario);
    validaArquivo(cabecalho.status, 1, binario);
    atualizaStatusBinario('0', binario);

    // Checa a existência de registros não removidos
    if (cabecalho.nroRegistros == 0) {
        printf("%s\n", REGISTRO_INEXISTENTE);
        atualizaStatusBinario('1', binario);
        fclose(binario);
        exit(0);
    }

    int nroTotalRegistros = cabecalho.nroRegistros + cabecalho.nroRegRemovidos;

    // Flag para indicar se houve ao menos um registro encontrado
    bool encontrado = false;

    // Percorre os registros
    for (int i = 0; i < nroTotalRegistros; i++) {
        Linha linha = leLinhaBinario(binario);

        // Caso registro lido não removido
        if (!registroFoiRemovido(linha.removido)) {
            // Checa valor desejado
            if (comparaCampoLinha(&linha, campo, valor)) {
                imprimeLinha(linha, cabecalho);
                encontrado = true;
            }
        }
    }

    if (!encontrado) {
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
    leStringsEntrada(1, nomeBinario);

    // Abre o binário
    FILE *binario = abreArquivo(nomeBinario, "rb+", 0);
    CabecalhoVeiculo cabecalho = leCabecalhoVeiculoBinario(binario);
    validaArquivo(cabecalho.status, 1, binario);
    atualizaStatusBinario('0', binario);

    // Recebe o número de inserções e posiciona o arquivo
    int insercoes;
    scanf("%d", &insercoes);
    posicionaBinarioProximoRegistroVeiculo(binario, cabecalho);

    // Percorre a entrada escrevendo os registros
    for (int i = 0; i < insercoes; i++) {
        Veiculo veiculo = leVeiculoEntrada();
        escreveVeiculoBinario(veiculo, binario);
        cabecalho.nroRegistros++;
    }

    // Atualiza o cabeçalho
    cabecalho.byteProxReg = ftell(binario);
    cabecalho.status = '1';
    escreveCabecalhoVeiculoBinario(cabecalho, binario);

    fclose(binario);
    binarioNaTela(nomeBinario);
}

void insertIntoLinha() {
    char nomeBinario[255];

    // Recebe o nome do arquivo
    leStringsEntrada(1, nomeBinario);

    // Abre o binário
    FILE *binario = abreArquivo(nomeBinario, "rb+", 0);
    CabecalhoLinha cabecalho = leCabecalhoLinhaBinario(binario);
    validaArquivo(cabecalho.status, 1, binario);
    atualizaStatusBinario('0', binario);

    // Recebe o número de inserções e posiciona o arquivo
    int insercoes;
    scanf("%d", &insercoes);
    posicionaBinarioProximoRegistroLinha(binario, cabecalho);

    // Percorre a entrada escrevendo os registros
    for (int i = 0; i < insercoes; i++) {
        Linha linha = leLinhaEntrada();
        escreveLinhaBinario(linha, binario);
        cabecalho.nroRegistros++;
    }

    // Atualiza o cabeçalho
    cabecalho.byteProxReg = ftell(binario);
    cabecalho.status = '1';
    escreveCabecalhoLinhaBinario(cabecalho, binario);

    fclose(binario);
    binarioNaTela(nomeBinario);
}


/**
 *
 * Create Index
 * 
 */

void createIndexVeiculo() {
    char nomeBinario[255];
    char nomeArvoreB[255];

    // Recebe o nome dos arquivos
    leStringsEntrada(2, nomeBinario, nomeArvoreB);

    // Abre o binário veículo
    FILE *binario = abreArquivo(nomeBinario, "rb", 0);
    CabecalhoVeiculo cabecalhoVeiculo = leCabecalhoVeiculoBinario(binario);
    validaArquivo(cabecalhoVeiculo.status, 1, binario);

    // Checa a existência de registros não removidos
    if (cabecalhoVeiculo.nroRegistros == 0) {
        printf("%s\n", FALHA_PROCESSAMENTO);
        fclose(binario);
        exit(0);
    }

    // Abre a árvore-B
    FILE *arvoreB = abreArquivo(nomeArvoreB, "wb+", 1, binario);
    CabecalhoArvoreB cabecalhoArvoreB = criaCabecalhoArvoreB();
    escreveCabecalhoArvoreB(cabecalhoArvoreB, arvoreB);

    int nroTotalRegistros = cabecalhoVeiculo.nroRegistros + cabecalhoVeiculo.nroRegRemovidos;

    // Percorre os registros
    for (int i = 0; i < nroTotalRegistros; i++) {
        long long int offset = ftell(binario);
        Veiculo veiculo = leVeiculoBinario(binario);

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
    fclose(binario);
    fclose(arvoreB);

    binarioNaTela(nomeArvoreB);
}

void createIndexLinha() {
    char nomeBinario[255];
    char nomeArvoreB[255];

    // Recebe o nome dos arquivos
    leStringsEntrada(2, nomeBinario, nomeArvoreB);

    // Abre o binário linha
    FILE *binario = abreArquivo(nomeBinario, "rb", 0);
    CabecalhoLinha cabecalhoLinha = leCabecalhoLinhaBinario(binario);
    validaArquivo(cabecalhoLinha.status, 1, binario);

    // Checa a existência de registros não removidos
    if (cabecalhoLinha.nroRegistros == 0) {
        printf("%s\n", FALHA_PROCESSAMENTO);
        fclose(binario);
        exit(0);
    }

    // Abre a árvore-B
    FILE *arvoreB = abreArquivo(nomeArvoreB, "wb+", 1, binario);
    CabecalhoArvoreB cabecalhoArvoreB = criaCabecalhoArvoreB();
    escreveCabecalhoArvoreB(cabecalhoArvoreB, arvoreB);

    int nroTotalRegistros = cabecalhoLinha.nroRegistros + cabecalhoLinha.nroRegRemovidos;

    // Percorre os registros
    for (int i = 0; i < nroTotalRegistros; i++) {
        long long int offset = ftell(binario);
        Linha linha = leLinhaBinario(binario);

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
    fclose(binario);
    fclose(arvoreB);

    binarioNaTela(nomeArvoreB);
}


/**
 *
 * Select From Where Index
 * 
 */

void selectFromWhereIndexVeiculo() {
    char nomeBinario[255];
    char nomeArvoreB[255];
    char campo[20];

    // Recebe os nomes dos arquivos e do campo
    leStringsEntrada(3, nomeBinario, nomeArvoreB, campo);

    // Checa se o campo é chave primária
    if (strcmp(campo, "prefixo") != 0) {
        printf("%s\n", FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Recebe o prefixo
    char prefixo[6];
    scan_quote_string(prefixo);

    // Abre o binário veículo
    FILE *binario = abreArquivo(nomeBinario, "rb", 0);
    CabecalhoVeiculo cabecalhoVeiculo = leCabecalhoVeiculoBinario(binario);
    validaArquivo(cabecalhoVeiculo.status, 1, binario);

    // Abre a árvore-B
    FILE *arvoreB = abreArquivo(nomeArvoreB, "rb", 1, binario);
    CabecalhoArvoreB cabecalhoArvoreB = leCabecalhoArvoreB(arvoreB);
    validaArquivo(cabecalhoArvoreB.status, 2, binario, arvoreB);

    // Busca na árvore-B
    long long int offset = buscaArvoreB(convertePrefixo(prefixo), cabecalhoArvoreB.noRaiz, arvoreB);

    // Caso encontrado
    if (offset != NAO_ENCONTRADO) {
        // Lê o veículo especificado
        fseek(binario, offset, SEEK_SET);
        Veiculo veiculo = leVeiculoBinario(binario);

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
    fclose(binario);
    fclose(arvoreB);
}

void selectFromWhereIndexLinha() {
    char nomeBinario[255];
    char nomeArvoreB[255];
    char campo[20];

    // Recebe os nomes dos arquivos e do campo
    leStringsEntrada(3, nomeBinario, nomeArvoreB, campo);

    // Checa se o campo é chave primária
    if (strcmp(campo, "codLinha") != 0) {
        printf("%s\n", FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Recebe o código da linha
    int codLinha;
    scanf("%d", &codLinha);

    // Abre o binário linha
    FILE *binario = abreArquivo(nomeBinario, "rb", 0);
    CabecalhoLinha cabecalhoLinha = leCabecalhoLinhaBinario(binario);
    validaArquivo(cabecalhoLinha.status, 1, binario);

    // Abre a árvore-B
    FILE *arvoreB = abreArquivo(nomeArvoreB, "rb", 1, binario);
    CabecalhoArvoreB cabecalhoArvoreB = leCabecalhoArvoreB(arvoreB);
    validaArquivo(cabecalhoArvoreB.status, 2, binario, arvoreB);

    // Busca na árvore-B
    long long int offset = buscaArvoreB(codLinha, cabecalhoArvoreB.noRaiz, arvoreB);

    // Caso encontrado
    if (offset != NAO_ENCONTRADO) {
        // Lê a linha especificada
        fseek(binario, offset, SEEK_SET);
        Linha linha = leLinhaBinario(binario);

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
    fclose(binario);
    fclose(arvoreB);
}


/**
 *
 * Insert Into Index
 * 
 */

void insertIntoIndexVeiculo() {
    char nomeBinario[255];
    char nomeArvoreB[255];

    // Recebe os nomes dos arquivos
    leStringsEntrada(2, nomeBinario, nomeArvoreB);

    // Abre o binário veículo
    FILE *binario = abreArquivo(nomeBinario, "rb+", 0);
    CabecalhoVeiculo cabecalhoVeiculo = leCabecalhoVeiculoBinario(binario);
    validaArquivo(cabecalhoVeiculo.status, 1, binario);
    atualizaStatusBinario('0', binario);

    // Abre a árvore-B
    FILE *arvoreB = abreArquivo(nomeArvoreB, "rb+", 1, binario);
    CabecalhoArvoreB cabecalhoArvoreB = leCabecalhoArvoreB(arvoreB);
    validaArquivo(cabecalhoArvoreB.status, 2, binario, arvoreB);
    atualizaStatusBinario('0', arvoreB);

    // Recebe o número de inserções e posiciona o arquivo
    int insercoes;
    scanf("%d", &insercoes);
    posicionaBinarioProximoRegistroVeiculo(binario, cabecalhoVeiculo);

    // Percorre a entrada escrevendo os registros
    for (int i = 0; i < insercoes; i++) {
        Veiculo veiculo = leVeiculoEntrada();

        // Cria a chave a ser inserida
        ChaveArvoreB chave;
        chave.C = convertePrefixo(veiculo.prefixo);
        chave.PR = ftell(binario);

        // Escreve o veículo no binário veículo
        escreveVeiculoBinario(veiculo, binario);
        cabecalhoVeiculo.nroRegistros++;

        // Insere a chave na árvore-B
        insereArvoreB(chave, &cabecalhoArvoreB, arvoreB);
    }

    // Atualiza o cabeçalho do veículo
    cabecalhoVeiculo.status = '1';
    cabecalhoVeiculo.byteProxReg = ftell(binario);
    escreveCabecalhoVeiculoBinario(cabecalhoVeiculo, binario);

    // Atualiza o cabeçalho da árvore-B
    cabecalhoArvoreB.status = '1';
    escreveCabecalhoArvoreB(cabecalhoArvoreB, arvoreB);

    // Fecha os arquivos
    fclose(binario);
    fclose(arvoreB);

    binarioNaTela(nomeArvoreB);
}

void insertIntoIndexLinha() {
    char nomeBinario[255];
    char nomeArvoreB[255];

    // Recebe os nome do arquivos
    leStringsEntrada(2, nomeBinario, nomeArvoreB);

    // Abre o binário linha
    FILE *binario = abreArquivo(nomeBinario, "rb+", 0);
    CabecalhoLinha cabecalhoLinha = leCabecalhoLinhaBinario(binario);
    validaArquivo(cabecalhoLinha.status, 1, binario);
    atualizaStatusBinario('0', binario);

    // Abre a árvore-B
    FILE *arvoreB = abreArquivo(nomeArvoreB, "rb+", 1, binario);
    CabecalhoArvoreB cabecalhoArvoreB = leCabecalhoArvoreB(arvoreB);
    validaArquivo(cabecalhoArvoreB.status, 2, binario, arvoreB);
    atualizaStatusBinario('0', arvoreB);

    // Recebe o número de inserções e posiciona o arquivo
    int insercoes;
    scanf("%d", &insercoes);
    posicionaBinarioProximoRegistroLinha(binario, cabecalhoLinha);

    // Percorre a entrada escrevendo os registros
    for (int i = 0; i < insercoes; i++) {
        Linha linha = leLinhaEntrada();

        // Cria a chave a ser inserida
        ChaveArvoreB chave;
        chave.C = linha.codLinha;
        chave.PR = ftell(binario);

        // Escreve a linha no binário linha
        escreveLinhaBinario(linha, binario);
        cabecalhoLinha.nroRegistros++;

        // Insere a chave na árvore-B
        insereArvoreB(chave, &cabecalhoArvoreB, arvoreB);
    }

    // Atualiza o cabeçalho da linha
    cabecalhoLinha.status = '1';
    cabecalhoLinha.byteProxReg = ftell(binario);
    escreveCabecalhoLinhaBinario(cabecalhoLinha, binario);

    // Atualiza o cabeçalho da árvore-B
    cabecalhoArvoreB.status = '1';
    escreveCabecalhoArvoreB(cabecalhoArvoreB, arvoreB);

    // Fecha os arquivos
    fclose(binario);
    fclose(arvoreB);

    binarioNaTela(nomeArvoreB);
}


/**
 * 
 * Order By
 * 
 */

void orderByVeiculo() {
    char nomeOriginal[255];
    char nomeOrdenado[255];
    char campo[20];

    leStringsEntrada(3, nomeOriginal, nomeOrdenado, campo);

    if (!ehCampoOrdenavel(campo)) {
        printf("%s\n", FALHA_PROCESSAMENTO);
        exit(0);
    }

    FILE *original = abreArquivo(nomeOriginal, "rb", 0);
    CabecalhoVeiculo cabecalhoOriginal = leCabecalhoVeiculoBinario(original);
    validaArquivo(cabecalhoOriginal.status, 1, original);

    FILE *ordenado = abreArquivo(nomeOrdenado, "wb", 1, original);
    CabecalhoVeiculo cabecalhoOrdenado = criaCabecalhoVeiculoOrdenado(cabecalhoOriginal);
    escreveCabecalhoVeiculoBinario(cabecalhoOrdenado, ordenado);

    if (cabecalhoOriginal.nroRegistros > 0) {
        int nroTotalRegistros = cabecalhoOriginal.nroRegistros + cabecalhoOriginal.nroRegRemovidos;

        Veiculo veiculos[cabecalhoOriginal.nroRegistros];
        leVeiculosBinario(veiculos, nroTotalRegistros, original);

        ordenaVeiculos(veiculos, cabecalhoOriginal.nroRegistros);
        escreveVeiculosBinario(veiculos, cabecalhoOriginal.nroRegistros, ordenado);
    }

    cabecalhoOrdenado.status = '1';
    cabecalhoOrdenado.byteProxReg = ftell(ordenado);
    cabecalhoOrdenado.nroRegistros = cabecalhoOriginal.nroRegistros;
    escreveCabecalhoVeiculoBinario(cabecalhoOrdenado, ordenado);

    fclose(original);
    fclose(ordenado);

    binarioNaTela(nomeOrdenado);
}

void orderByLinha() {

}


/**
 * 
 * Select From Join On
 * 
 */

void selectFromJoinOnLoop() {
    char nomeBinarioVeiculo[255];
    char nomeBinarioLinha[255];
    char campoVeiculo[20];
    char campoLinha[20];

    // Leitura dos campos
    leStringsEntrada(4, nomeBinarioVeiculo, nomeBinarioLinha, campoVeiculo, campoLinha);

    // Checa se os campos são válidos
    if (!ehCampoJuntavel(campoVeiculo) || !ehCampoJuntavel(campoLinha)) {
        printf("%s\n", FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Leitura do arquivo de veículos
    FILE *binarioVeiculo = abreArquivo(nomeBinarioVeiculo, "rb", 0);
    CabecalhoVeiculo cabecalhoVeiculo = leCabecalhoVeiculoBinario(binarioVeiculo);
    validaArquivo(cabecalhoVeiculo.status, 1, binarioVeiculo);

    // Leitura do arquivo de linhas
    FILE *binarioLinha = abreArquivo(nomeBinarioLinha, "rb", 1, binarioVeiculo);
    CabecalhoLinha cabecalhoLinha = leCabecalhoLinhaBinario(binarioLinha);
    validaArquivo(cabecalhoLinha.status, 2, binarioVeiculo, binarioLinha);

    // Caso não haja veículo ou linha
    if (cabecalhoLinha.nroRegistros == 0 || cabecalhoVeiculo.nroRegistros == 0) {
        printf("%s\n", REGISTRO_INEXISTENTE);
        fclose(binarioVeiculo);
        fclose(binarioLinha);
        exit(0);
    }

    // Contabiliza o total de registros
    int nroTotalRegistrosVeiculo = cabecalhoVeiculo.nroRegistros + cabecalhoVeiculo.nroRegRemovidos;
    int nroTotalRegistrosLinha = cabecalhoLinha.nroRegistros + cabecalhoLinha.nroRegRemovidos;

    // Operador para encontro
    bool encontrado = false;

    // Percorre os veículos
    for (int i = 0; i < nroTotalRegistrosVeiculo; i++) {
        Veiculo veiculo = leVeiculoBinario(binarioVeiculo);

        // Caso seja um veículo válido
        if (!registroFoiRemovido(veiculo.removido)) {
            // Percorre as linhas
            for (int j = 0; j < nroTotalRegistrosLinha; j++) {
                Linha linha = leLinhaBinario(binarioLinha);

                // Caso seja uma linha válida
                if (!registroFoiRemovido(linha.removido)) {
                    // Caso haja o "match"
                    if (veiculo.codLinha == linha.codLinha) {
                        encontrado = true;
                        imprimeVeiculo(veiculo, cabecalhoVeiculo);
                        imprimeLinha(linha, cabecalhoLinha);
                        printf("\n");
                        break;
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
    char nomeBinarioVeiculo[255];
    char nomeBinarioLinha[255];
    char campoVeiculo[20];
    char campoLinha[20];
    char nomeArvoreB[255];

    // Leitura dos campos
    leStringsEntrada(5, nomeBinarioVeiculo, nomeBinarioLinha, campoVeiculo, campoLinha, nomeArvoreB);

    // Checa se os campos são válidos
    if (!ehCampoJuntavel(campoVeiculo) || !ehCampoJuntavel(campoLinha)) {
        printf("%s\n", FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Leitura do arquivo de veículos
    FILE *binarioVeiculo = abreArquivo(nomeBinarioVeiculo, "rb", 0);
    CabecalhoVeiculo cabecalhoVeiculo = leCabecalhoVeiculoBinario(binarioVeiculo);
    validaArquivo(cabecalhoVeiculo.status, 1, binarioVeiculo);

    // Leitura do arquivo de linhas
    FILE *binarioLinha = abreArquivo(nomeBinarioLinha, "rb", 1, binarioVeiculo);
    CabecalhoLinha cabecalhoLinha = leCabecalhoLinhaBinario(binarioLinha);
    validaArquivo(cabecalhoLinha.status, 2, binarioVeiculo, binarioLinha);

    // Leitura do arquivo de índice de linhas
    FILE *arvoreB = abreArquivo(nomeArvoreB, "rb", 2, binarioVeiculo, binarioLinha);
    CabecalhoArvoreB cabecalhoIndiceLinha = leCabecalhoArvoreB(arvoreB);
    validaArquivo(cabecalhoIndiceLinha.status, 3, binarioVeiculo, binarioLinha, arvoreB);

    // Caso não haja veículo
    if (cabecalhoVeiculo.nroRegistros == 0) {
        printf("%s\n", REGISTRO_INEXISTENTE);
        fclose(binarioVeiculo);
        fclose(binarioLinha);
        fclose(arvoreB);
        exit(0);
    }

    // Contabiliza o total de registros
    int nroTotalRegistrosVeiculo = cabecalhoVeiculo.nroRegistros + cabecalhoVeiculo.nroRegRemovidos;

    // Operador para encontro
    bool encontrado = false;

    // Percorre os veículos
    for (int i = 0; i < nroTotalRegistrosVeiculo; i++) {
        Veiculo veiculo = leVeiculoBinario(binarioVeiculo);

        // Se o veículo não foi excluído
        if (!registroFoiRemovido(veiculo.removido)) {

            // Caso a busca retorne um offset válido
            long long int offset;
            if ((offset = buscaArvoreB(veiculo.codLinha,
                                       cabecalhoIndiceLinha.noRaiz,
                                       arvoreB)) != NAO_ENCONTRADO) {
                                           
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
    fclose(arvoreB);
}

void selectFromJoinOnMerge() {

}
