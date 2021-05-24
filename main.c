// Pedro Lucas de Moliner de Castro - 11795784
// Matheus Henrique de Cerqueira Pinto - 11911104

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "linha.h"
#include "veiculo.h"


void createTableVeiculo();
void createTableLinha();
void selectFromVeiculo();
void selectFromLinha();
void selectFromWhereVeiculo();
void selectFromWhereLinha();
void insertIntoVeiculo();
void insertIntoLinha();


const char *MENSAGEM_FALHA_PROCESSAMENTO = "Falha no processamento do arquivo.";
const char *MENSAGEM_REGISTRO_INEXISTENTE = "Registro inexistente.";


int main() {
    int funcionalidade;
    scanf("%d", &funcionalidade);

    switch (funcionalidade) {
        case 1:
            createTableVeiculo();
            break;
        case 2:
            createTableLinha();
            break;
        case 3:
            selectFromVeiculo();
            break;
        case 4:
            selectFromLinha();
            break;
        case 5:
            selectFromWhereVeiculo();
            break;
        case 6:
            selectFromWhereLinha();
            break;
        case 7:
            insertIntoVeiculo();
            break;
        case 8:
            insertIntoLinha();
            break;
        default:
            printf("Falha na identificacao da funcionalidade %d.", funcionalidade);
            exit(0);
            break;
    }

    return 0;
}

void createTableVeiculo() {
    char nomeCsv[255];
    char nomeBinario[255];

    if (scanf("%s %s", nomeCsv, nomeBinario) != 2) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    FILE *csv = fopen(nomeCsv, "r");
    if (csv == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

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

    if (scanf("%s %s", nomeCsv, nomeBinario) != 2) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    FILE *csv = fopen(nomeCsv, "r");
    if (csv == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

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

void selectFromVeiculo() {
    char nomeBinario[255];

    // Leitura do nome do arquivo
    if (scanf("%s", nomeBinario) != 1) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Leitura do arquivo
    FILE *binario = fopen(nomeBinario, "rb");
    if (binario == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Leitura do cabeçalho do binário
    CabecalhoVeiculo cabecalhoVeiculo;
    leCabecalhoVeiculoBinario(&cabecalhoVeiculo, binario);

    // Caso esteja corrompido
    if (arquivoFoiCorrompido(cabecalhoVeiculo.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(binario);
        exit(0);
    }

    // Caso não haja registros
    if (cabecalhoVeiculo.nroRegistros == 0) {
        printf("%s\n", MENSAGEM_REGISTRO_INEXISTENTE);
        fclose(binario);
        exit(0);
    }

    // Leitura do corpo do binário
    Veiculo veiculo;

    // Contabiliza todos os registros
    int nroTotalRegistros = cabecalhoVeiculo.nroRegistros + cabecalhoVeiculo.nroRegRemovidos;

    // Percorre até o fim do número de registros
    for (int i = 0; i < nroTotalRegistros; i++) {
        // Caso seja lido um registro não excluído
        if (leVeiculoBinario(&veiculo, binario) == true)
            imprimeVeiculo(&cabecalhoVeiculo, &veiculo);

        // Se for excluído, pula o corpo do registro
        else
            fseek(binario, veiculo.tamanhoRegistro, SEEK_CUR);
    }

    fclose(binario);
}

void selectFromLinha() {
    char nomeBinario[255];

    // Leitura do nome do arquivo
    if (scanf("%s", nomeBinario) != 1) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Leitura do arquivo
    FILE *binario = fopen(nomeBinario, "rb");
    if (binario == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Leitura do cabeçalho do binário
    CabecalhoLinha cabecalhoLinha;
    leCabecalhoLinhaBinario(&cabecalhoLinha, binario);

    // Caso esteja corrompido
    if (arquivoFoiCorrompido(cabecalhoLinha.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(binario);
        exit(0);
    }

    // Caso não haja registros
    if (cabecalhoLinha.nroRegistros == 0) {
        printf("%s\n", MENSAGEM_REGISTRO_INEXISTENTE);
        fclose(binario);
        exit(0);
    }

    // Leitura do corpo do binário
    Linha linha;

    // Contabiliza todos os registros
    int nroTotalRegistros = cabecalhoLinha.nroRegistros + cabecalhoLinha.nroRegRemovidos;

    // Percorre até o fim do número de registros
    for (int i = 0; i < nroTotalRegistros; i++) {
        // Caso seja lido um registro não excluído
        if (leLinhaBinario(&linha, binario) == true)
            imprimeLinha(&cabecalhoLinha, &linha);

        // Se for excluído, pula o corpo do registro
        else
            fseek(binario, linha.tamanhoRegistro, SEEK_CUR);
    }

    fclose(binario);
}

void selectFromWhereVeiculo() {
    char nomeBinario[255];
    char campo[20];

    // Leitura do nome do arquivo
    if (scanf("%s %s", nomeBinario, campo) != 2) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    char valor[100];
    scan_quote_string(valor);

    // Leitura do arquivo
    FILE *binario = fopen(nomeBinario, "rb");
    if (binario == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Leitura do cabeçalho do binário
    CabecalhoVeiculo cabecalhoVeiculo;
    leCabecalhoVeiculoBinario(&cabecalhoVeiculo, binario);

    // Caso esteja corrompido
    if (arquivoFoiCorrompido(cabecalhoVeiculo.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(binario);
        exit(0);
    }

    // Caso não haja registros
    if (cabecalhoVeiculo.nroRegistros == 0) {
        printf("%s\n", MENSAGEM_REGISTRO_INEXISTENTE);
        fclose(binario);
        exit(0);
    }

    Veiculo veiculo;

    // Contabiliza todos os registros
    int nroTotalRegistros = cabecalhoVeiculo.nroRegistros + cabecalhoVeiculo.nroRegRemovidos;

    // Flag para checar se houve ao menos uma correspondência
    bool houveCorrespondencia = false;

    // Percorre até o fim do número de registros
    for (int i = 0; i < nroTotalRegistros; i++) {
        // Caso seja lido um registro não excluído
        if (leVeiculoBinario(&veiculo, binario) == true) {
            // Checa se o veículo tem o valor fornecido no campo desejado
            if (comparaVeiculo(&veiculo, campo, valor)) {
                imprimeVeiculo(&cabecalhoVeiculo, &veiculo);
                houveCorrespondencia = true;
            }
        }

        // Se for excluído, pula o corpo do registro
        else
            fseek(binario, veiculo.tamanhoRegistro, SEEK_CUR);
    }

    if (!houveCorrespondencia)
        printf("%s\n", MENSAGEM_REGISTRO_INEXISTENTE);

    fclose(binario);
}

void selectFromWhereLinha() {
    char nomeBinario[255];
    char campo[20];

    // Leitura do nome do arquivo
    if (scanf("%s %s", nomeBinario, campo) != 2) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    char valor[100];
    scan_quote_string(valor);

    // Leitura do arquivo
    FILE *binario = fopen(nomeBinario, "rb");
    if (binario == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Leitura do cabeçalho do binário
    CabecalhoLinha cabecalhoLinha;
    leCabecalhoLinhaBinario(&cabecalhoLinha, binario);

    // Caso esteja corrompido
    if (arquivoFoiCorrompido(cabecalhoLinha.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(binario);
        exit(0);
    }

    // Caso não haja registros
    if (cabecalhoLinha.nroRegistros == 0) {
        printf("%s\n", MENSAGEM_REGISTRO_INEXISTENTE);
        fclose(binario);
        exit(0);
    }

    Linha linha;

    // Contabiliza todos os registros
    int nroTotalRegistros = cabecalhoLinha.nroRegistros + cabecalhoLinha.nroRegRemovidos;

    // Flag para checar se houve ao menos uma correspondência
    bool houveCorrespondencia = false;

    // Percorre até o fim do número de registros
    for (int i = 0; i < nroTotalRegistros; i++) {
        // Caso seja lido um registro não excluído
        if (leLinhaBinario(&linha, binario) == true) {
            // Checa se a linha tem o valor fornecido no campo desejado
            if (comparaLinha(&linha, campo, valor)) {
                imprimeLinha(&cabecalhoLinha, &linha);
                houveCorrespondencia = true;
            }
        }

        // Se for excluído, pula o corpo do registro
        else
            fseek(binario, linha.tamanhoRegistro, SEEK_CUR);
    }

    if (!houveCorrespondencia)
        printf("%s\n", MENSAGEM_REGISTRO_INEXISTENTE);

    fclose(binario);
}

void insertIntoVeiculo() {
    char nomeBinario[255];

    if (scanf("%s", nomeBinario) != 1) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    FILE *binario = fopen(nomeBinario, "rb+");
    if (binario == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Inicializa o cabeçalho
    CabecalhoVeiculo cabecalhoVeiculo;
    leCabecalhoVeiculoBinario(&cabecalhoVeiculo, binario);

    // Caso esteja corrompido
    if (arquivoFoiCorrompido(cabecalhoVeiculo.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(binario);
        exit(0);
    }

    fseek(binario, cabecalhoVeiculo.byteProxReg, SEEK_SET);

    int insercoes;
    scanf("%d", &insercoes);

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

    if (scanf("%s", nomeBinario) != 1) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    FILE *binario = fopen(nomeBinario, "rb+");
    if (binario == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Inicializa o cabeçalho
    CabecalhoLinha cabecalhoLinha;
    leCabecalhoLinhaBinario(&cabecalhoLinha, binario);

    // Caso esteja corrompido
    if (arquivoFoiCorrompido(cabecalhoLinha.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(binario);
        exit(0);
    }

    fseek(binario, cabecalhoLinha.byteProxReg, SEEK_SET);

    int insercoes;
    scanf("%d", &insercoes);

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
