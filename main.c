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

    if (scanf("%s", nomeBinario) != 1) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    FILE *binario = fopen(nomeBinario, "rb");
    if (binario == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    CabecalhoVeiculo cabecalhoVeiculo;
    leCabecalhoVeiculoBinario(&cabecalhoVeiculo, binario);

    if (arquivoFoiCorrompido(cabecalhoVeiculo.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(binario);
        exit(0);
    }

    if (cabecalhoVeiculo.nroRegistros == 0) {
        printf("%s\n", MENSAGEM_REGISTRO_INEXISTENTE);
        fclose(binario);
        exit(0);
    }

    Veiculo veiculo;
    int nroTotalRegistros = cabecalhoVeiculo.nroRegistros + cabecalhoVeiculo.nroRegRemovidos;

    for (int i = 0; i < nroTotalRegistros; i++) {
        // Caso registro lido não removido
        if (leVeiculoBinario(&veiculo, binario) == true)
            imprimeVeiculo(&cabecalhoVeiculo, &veiculo);

        // Caso removido, pula corpo
        else
            fseek(binario, veiculo.tamanhoRegistro, SEEK_CUR);
    }

    fclose(binario);
}

void selectFromLinha() {
    char nomeBinario[255];

    if (scanf("%s", nomeBinario) != 1) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    FILE *binario = fopen(nomeBinario, "rb");
    if (binario == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    CabecalhoLinha cabecalhoLinha;
    leCabecalhoLinhaBinario(&cabecalhoLinha, binario);

    if (arquivoFoiCorrompido(cabecalhoLinha.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(binario);
        exit(0);
    }

    if (cabecalhoLinha.nroRegistros == 0) {
        printf("%s\n", MENSAGEM_REGISTRO_INEXISTENTE);
        fclose(binario);
        exit(0);
    }

    Linha linha;
    int nroTotalRegistros = cabecalhoLinha.nroRegistros + cabecalhoLinha.nroRegRemovidos;

    for (int i = 0; i < nroTotalRegistros; i++) {
        // Caso registro lido não removido
        if (leLinhaBinario(&linha, binario) == true)
            imprimeLinha(&cabecalhoLinha, &linha);

        // Caso removido, pula corpo
        else
            fseek(binario, linha.tamanhoRegistro, SEEK_CUR);
    }

    fclose(binario);
}

void selectFromWhereVeiculo() {
    char nomeBinario[255];
    char campo[20];

    if (scanf("%s %s", nomeBinario, campo) != 2) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    char valor[100];
    scan_quote_string(valor);

    FILE *binario = fopen(nomeBinario, "rb");
    if (binario == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    CabecalhoVeiculo cabecalhoVeiculo;
    leCabecalhoVeiculoBinario(&cabecalhoVeiculo, binario);

    if (arquivoFoiCorrompido(cabecalhoVeiculo.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(binario);
        exit(0);
    }

    if (cabecalhoVeiculo.nroRegistros == 0) {
        printf("%s\n", MENSAGEM_REGISTRO_INEXISTENTE);
        fclose(binario);
        exit(0);
    }

    Veiculo veiculo;
    int nroTotalRegistros = cabecalhoVeiculo.nroRegistros + cabecalhoVeiculo.nroRegRemovidos;
    bool houveCorrespondencia = false;

    for (int i = 0; i < nroTotalRegistros; i++) {
        // Caso registro lido não removido
        if (leVeiculoBinario(&veiculo, binario) == true) {
            // Checa valor desejado
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

    if (scanf("%s %s", nomeBinario, campo) != 2) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    char valor[100];
    scan_quote_string(valor);

    FILE *binario = fopen(nomeBinario, "rb");
    if (binario == NULL) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        exit(0);
    }

    CabecalhoLinha cabecalhoLinha;
    leCabecalhoLinhaBinario(&cabecalhoLinha, binario);

    if (arquivoFoiCorrompido(cabecalhoLinha.status)) {
        printf("%s\n", MENSAGEM_FALHA_PROCESSAMENTO);
        fclose(binario);
        exit(0);
    }

    if (cabecalhoLinha.nroRegistros == 0) {
        printf("%s\n", MENSAGEM_REGISTRO_INEXISTENTE);
        fclose(binario);
        exit(0);
    }

    Linha linha;
    int nroTotalRegistros = cabecalhoLinha.nroRegistros + cabecalhoLinha.nroRegRemovidos;
    bool houveCorrespondencia = false;

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

    CabecalhoVeiculo cabecalhoVeiculo;
    leCabecalhoVeiculoBinario(&cabecalhoVeiculo, binario);

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

    CabecalhoLinha cabecalhoLinha;
    leCabecalhoLinhaBinario(&cabecalhoLinha, binario);

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
