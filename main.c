// Pedro Lucas de Moliner de Castro - 11795784
// Matheus Henrique de Cerqueira Pinto - 11911104

#include <stdio.h>
#include <stdlib.h>

#include "util.h"
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

            // Lê nome arquivos
            if (scanf("%s %s", nomeCsv, nomeBinario) != 2) {
                fprintf(stderr, "Falha no processamento do arquivo.\n");
                exit(EXIT_FAILURE);
            }

            // Abre CSV para leitura
            FILE *csv = fopen(nomeCsv, "r");
            if (csv == NULL) {
                fprintf(stderr, "Falha no processamento do arquivo.\n");
                exit(EXIT_FAILURE);
            }

            // Abre binário para escrita
            FILE *binario = fopen(nomeBinario, "wb");
            if (binario == NULL) {
                fclose(csv);
                fprintf(stderr, "Falha no processamento do arquivo.\n");
                exit(EXIT_FAILURE);
            }

            // Inicializa cabeçalho
            CabecalhoVeiculo cabecalhoVeiculo;
            leCabecalhoVeiculoCsv(&cabecalhoVeiculo, csv);
            escreveCabecalhoVeiculoBinario(&cabecalhoVeiculo, binario);

            // Lê e escreve registros
            Veiculo veiculo;
            while (leVeiculoCsv(&veiculo, csv) != EOF) {
                escreveVeiculoBinario(&veiculo, binario);

                // Atualiza número de registros
                if (veiculo.removido == '0')
                    cabecalhoVeiculo.nroRegRemovidos++;
                else
                    cabecalhoVeiculo.nroRegistros++;
            }

            // Atualiza campos de controle do cabeçalho
            cabecalhoVeiculo.byteProxReg = ftell(binario);
            cabecalhoVeiculo.status = '1';

            // Atualiza cabeçalho
            fseek(binario, 0, SEEK_SET);
            escreveCabecalhoVeiculoBinario(&cabecalhoVeiculo, binario);

            // Fecha arquivos utilizados
            fclose(csv);
            fclose(binario);

            binarioNaTela(nomeBinario);

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
