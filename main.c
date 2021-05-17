// Pedro Lucas de Moliner de Castro - 11795784
// Matheus Henrique de Cerqueira Pinto - 11911104

#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "veiculo.h"


void createTableVeiculo();
void createTableLinha();
void selectFromVeiculo();
void selectFromLinha();
void selectFromWhereVeiculo();
void selectFromWhereLinha();
void insertIntoVeiculo();
void insertIntoLinha();


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
            fprintf(stderr, "Falha na identificacao da funcionalidade %u.", funcionalidade);
            exit(EXIT_FAILURE);
            break;
    }

    return EXIT_SUCCESS;
}

void createTableVeiculo() {
    char nomeCsv[255];
    char nomeBinario[255];

    // Lê nome dos arquivos
    if (scanf("%s %s", nomeCsv, nomeBinario) != 2) {
        fprintf(stderr, "Falha no processamento do arquivo.\n");
        exit(EXIT_FAILURE);
    }

    // Abre o CSV para leitura
    FILE *csv = fopen(nomeCsv, "r");
    if (csv == NULL) {
        fprintf(stderr, "Falha no processamento do arquivo.\n");
        exit(EXIT_FAILURE);
    }

    // Abre o binário para escrita binária
    FILE *binario = fopen(nomeBinario, "wb");
    if (binario == NULL) {
        fclose(csv);
        fprintf(stderr, "Falha no processamento do arquivo.\n");
        exit(EXIT_FAILURE);
    }

    // Inicializa o cabeçalho
    CabecalhoVeiculo cabecalhoVeiculo;
    leCabecalhoVeiculoCsv(&cabecalhoVeiculo, csv);
    escreveCabecalhoVeiculoBinario(&cabecalhoVeiculo, binario);

    // Lê e escreve registros
    Veiculo veiculo;
    while (leVeiculoCsv(&veiculo, csv) != EOF) {
        escreveVeiculoBinario(&veiculo, binario);

        // Atualiza o número de registros
        if (veiculo.removido == '0')
            cabecalhoVeiculo.nroRegRemovidos++;
        else
            cabecalhoVeiculo.nroRegistros++;
    }

    // Atualiza os campos de controle do cabeçalho
    cabecalhoVeiculo.byteProxReg = ftell(binario);
    cabecalhoVeiculo.status = '1';

    // Atualiza cabeçalho
    fseek(binario, 0, SEEK_SET);
    escreveCabecalhoVeiculoBinario(&cabecalhoVeiculo, binario);

    // Fecha os arquivos utilizados
    fclose(csv);
    fclose(binario);

    binarioNaTela(nomeBinario);
}

void createTableLinha() {

}

void selectFromVeiculo() {

}

void selectFromLinha() {

}

void selectFromWhereVeiculo() {

}

void selectFromWhereLinha() {

}

void insertIntoVeiculo() {

}

void insertIntoLinha() {

}
