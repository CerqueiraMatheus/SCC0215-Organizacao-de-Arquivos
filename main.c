// Pedro Lucas de Moliner de Castro - 11795784
// Matheus Henrique de Cerqueira Pinto - 11911104

#include <stdio.h>
#include <stdlib.h>

#include "linha.h"
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

    if (scanf("%s %s", nomeCsv, nomeBinario) != 2) {
        fprintf(stderr, "Falha no processamento do arquivo.\n");
        exit(EXIT_FAILURE);
    }

    FILE *csv = fopen(nomeCsv, "r");
    if (csv == NULL) {
        fprintf(stderr, "Falha no processamento do arquivo.\n");
        exit(EXIT_FAILURE);
    }

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

    // Percorre o CSV escrevendo os registros
    Veiculo veiculo;
    while (leVeiculoCsv(&veiculo, csv) != EOF) {
        escreveVeiculoBinario(&veiculo, binario);

        if (veiculo.removido == '0')
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
        fprintf(stderr, "Falha no processamento do arquivo.\n");
        exit(EXIT_FAILURE);
    }

    FILE *csv = fopen(nomeCsv, "r");
    if (csv == NULL) {
        fprintf(stderr, "Falha no processamento do arquivo.\n");
        exit(EXIT_FAILURE);
    }

    FILE *binario = fopen(nomeBinario, "wb");
    if (binario == NULL) {
        fclose(csv);
        fprintf(stderr, "Falha no processamento do arquivo.\n");
        exit(EXIT_FAILURE);
    }

    // Inicializa o cabeçalho
    CabecalhoLinha cabecalhoLinha;
    leCabecalhoLinhaCsv(&cabecalhoLinha, csv);
    escreveCabecalhoLinhaBinario(&cabecalhoLinha, binario);

    // Percorre o CSV escrevendo os registros
    Linha linha;
    while (leLinhaCsv(&linha, csv) != EOF) {
        escreveLinhaBinario(&linha, binario);

        if (linha.removido == '0')
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
        fprintf(stderr, "Falha no processamento do arquivo.\n");
        exit(EXIT_FAILURE);
    }

    // Leitura do arquivo
    FILE *binario = fopen(nomeBinario, "rb");
    if (binario == NULL) {
        fprintf(stderr, "Falha no processamento do arquivo.\n");
        exit(EXIT_FAILURE);
    }

    // Leitura do cabeçalho do binário
    CabecalhoVeiculo cabecalhoVeiculo;
    leCabecalhoVeiculoBinario(&cabecalhoVeiculo, binario);

    // Leitura do corpo do binário
    Veiculo veiculo;

    // Contabiliza todos os registros
    int nroTotalRegistros = cabecalhoVeiculo.nroRegistros + cabecalhoVeiculo.nroRegRemovidos;
    
    // Percorre até o fim do número de registros
    for (; nroTotalRegistros > 0; nroTotalRegistros--) {
        
        // Caso seja lido um registro não excluído
        if (leVeiculoBinario(&veiculo, binario) != false) {
            printf("Prefixo do veiculo: %s\n", veiculo.prefixo);

            printf("Modelo do veiculo: ");
            printfTrataNuloVariavel(veiculo.modelo, veiculo.tamanhoModelo);

            printf("Categoria do veiculo: ");
            printfTrataNuloVariavel(veiculo.categoria, veiculo.tamanhoCategoria);

            printf("Data de entrada do veiculo na frota: ");
            printTrataNuloDataExtenso(veiculo.data);

            printf("Quantidade de lugares sentados disponiveis: ");
            printfTrataNuloInt(veiculo.quantidadeLugares);

            printf("\n");
        } 
        
        // Se for excluído, pula o corpo do registro
        else {
            fseek(binario, veiculo.tamanhoRegistro, SEEK_CUR);
        }
    }

    fclose(binario);
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
