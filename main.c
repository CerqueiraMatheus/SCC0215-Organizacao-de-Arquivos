// Pedro Lucas de Moliner de Castro - 11795784
// Matheus Henrique de Cerqueira Pinto - 11911104

#include <stdio.h>
#include <stdlib.h>

#include "veiculo.h"


enum Funcionalidades {
    CREATE_TABLE_VEICULO = 1,
    CREATE_TABLE_LINHA,
    SELECT_FROM_VEICULO,
    SELECT_FROM_LINHA,
    SELECT_FROM_WHERE_VEICULO,
    SELECT_FROM_WHERE_LINHA,
    INSERT_INTO_VEICULO,
    INSERT_INTO_LINHA
};

int main() {
    enum Funcionalidades funcionalidade;
    scanf("%u", &funcionalidade);

    switch (funcionalidade) {
        case CREATE_TABLE_VEICULO: ;
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

            CabecalhoVeiculo cabecalhoVeiculo;
            leCabecalhoVeiculoCsv(&cabecalhoVeiculo, csv);

            fclose(csv);
            fclose(binario);

            break;
        case CREATE_TABLE_LINHA: ;
            break;
        case SELECT_FROM_VEICULO: ;
            break;
        case SELECT_FROM_LINHA: ;
            break;
        case SELECT_FROM_WHERE_VEICULO: ;
            break;
        case SELECT_FROM_WHERE_LINHA: ;
            break;
        case INSERT_INTO_VEICULO: ;
            break;
        case INSERT_INTO_LINHA: ;
            break;
        default: ;
            fprintf(stderr, "Falha na identificacao da funcionalidade %u.", funcionalidade);
            exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
