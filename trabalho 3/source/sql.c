/**
 * @author Matheus Henrique de Cerqueira Pinto (11911104)
 * @author Pedro Lucas de Moliner de Castro (11795784)
 * @date 2021-07-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "arvoreb.h"
#include "linha.h"
#include "mensagem.h"
#include "util.h"
#include "veiculo.h"

void _criaBinarioVeiculoOrdenado(char *nomeOriginal, char *nomeOrdenado, char *campo);
void _criaBinarioLinhaOrdenado(char *nomeOriginal, char *nomeOrdenado, char *campo);

/**
 * 
 * Order By
 * 
 */

// Função 17:
//  -> Lê um arquivo de dados de Veículo desordenado e um arquivo destino
//  -> Cria um arquivo ordenado e exibe o binário na tela
void orderByVeiculo() {
    char nomeOriginal[255];
    char nomeOrdenado[255];
    char campo[20];

    // Leitura dos campos
    leStringsEntrada(3, nomeOriginal, nomeOrdenado, campo);

    // Criação do arquivo ordenado
    _criaBinarioVeiculoOrdenado(nomeOriginal, nomeOrdenado, campo);

    binarioNaTela(nomeOrdenado);
}

// Função 18:
//  -> Lê um arquivo de dados de Linha desordenado e um arquivo destino
//  -> Cria um arquivo ordenado e exibe o binário na tela
void orderByLinha() {
    char nomeOriginal[255];
    char nomeOrdenado[255];
    char campo[20];

    // Leitura dos campos
    leStringsEntrada(3, nomeOriginal, nomeOrdenado, campo);

    // Criação do arquivo ordenado
    _criaBinarioLinhaOrdenado(nomeOriginal, nomeOrdenado, campo);

    binarioNaTela(nomeOrdenado);
}

/**
 * 
 * Select From Join On
 * 
 */

// Função 15:
//  -> Lê um arquivo de dados de Veículo e Linha
//  -> Exibe os "matches" entre os códigos de linha de Veículo e Linha
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
    if (cabecalhoVeiculo.nroRegistros == 0 || cabecalhoLinha.nroRegistros == 0) {
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

// Função 16:
//  -> Lê um arquivo de dados de Veículo e Linha e um arquivo de índices de Linha
//  -> Exibe os "matches" entre os códigos de linha de Veículo e Linha
void selectFromJoinOnIndex() {
    char nomeBinarioVeiculo[255];
    char nomeBinarioLinha[255];
    char campoVeiculo[20];
    char campoLinha[20];
    char nomeArvoreB[255];

    // Leitura dos campos
    leStringsEntrada(5, nomeBinarioVeiculo, nomeBinarioLinha, campoVeiculo,
                     campoLinha, nomeArvoreB);

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
    CabecalhoArvoreB cabecalhoArvoreB = leCabecalhoArvoreB(arvoreB);
    validaArquivo(cabecalhoArvoreB.status, 3, binarioVeiculo, binarioLinha, arvoreB);

    // Caso não haja veículo ou linha
    if (cabecalhoVeiculo.nroRegistros == 0 || cabecalhoLinha.nroRegistros == 0) {
        printf("%s\n", REGISTRO_INEXISTENTE);
        fclose(binarioVeiculo);
        fclose(binarioLinha);
        fclose(arvoreB);
        exit(0);
    }

    // Contabiliza o total de registros
    int nroTotalRegistrosVeiculo =
        cabecalhoVeiculo.nroRegistros + cabecalhoVeiculo.nroRegRemovidos;

    // Operador para encontro
    bool encontrado = false;

    // Percorre os veículos
    for (int i = 0; i < nroTotalRegistrosVeiculo; i++) {
        Veiculo veiculo = leVeiculoBinario(binarioVeiculo);

        // Se o veículo não foi excluído
        if (!registroFoiRemovido(veiculo.removido)) {
            long long int offset = buscaArvoreB(veiculo.codLinha,
                                                cabecalhoArvoreB.noRaiz, arvoreB);

            // Caso a busca retorne um offset válido
            if (offset != NAO_ENCONTRADO) {
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

// Função 19:
//  -> Lê um arquivo de dados de Veículo e Linha
//  -> Cria arquivos ordenados de dados
//     Exibe os "matches" entre os códigos de linha de Veículo e Linha
void selectFromJoinOnMerge() {
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

    // Criação dos arquivos ordenados
    _criaBinarioVeiculoOrdenado(nomeBinarioVeiculo, "veiculoOrdenadoMerge.bin", campoVeiculo);
    _criaBinarioLinhaOrdenado(nomeBinarioLinha, "linhaOrdenadoMerge.bin", campoLinha);

    // Leitura do arquivo de veículos ordenado
    FILE *binarioVeiculo = abreArquivo("veiculoOrdenadoMerge.bin", "rb", 0);
    CabecalhoVeiculo cabecalhoVeiculo = leCabecalhoVeiculoBinario(binarioVeiculo);
    validaArquivo(cabecalhoVeiculo.status, 1, binarioVeiculo);

    // Leitura do arquivo de linhas ordenado
    FILE *binarioLinha = abreArquivo("linhaOrdenadoMerge.bin", "rb", 1, binarioVeiculo);
    CabecalhoLinha cabecalhoLinha = leCabecalhoLinhaBinario(binarioLinha);
    validaArquivo(cabecalhoLinha.status, 2, binarioVeiculo, binarioLinha);

    // Caso não haja veículo ou linha
    if (cabecalhoVeiculo.nroRegistros == 0 || cabecalhoLinha.nroRegistros == 0) {
        printf("%s\n", REGISTRO_INEXISTENTE);
        fclose(binarioVeiculo);
        fclose(binarioLinha);
        exit(0);
    }

    bool encontrado = false;

    // Leitura do primeiro veículo
    Veiculo veiculo = leVeiculoBinario(binarioVeiculo);
    int indiceVeiculo = 0;

    // Leitura da primeira linha
    Linha linha = leLinhaBinario(binarioLinha);
    int indiceLinha = 0;

    // Executa o merge dos arquivos
    while (indiceVeiculo < cabecalhoVeiculo.nroRegistros &&
           indiceLinha < cabecalhoLinha.nroRegistros) {
        // Caso haja o "match"
        if (veiculo.codLinha == linha.codLinha) {
            // Imprime o resultado
            encontrado = true;
            imprimeVeiculo(veiculo, cabecalhoVeiculo);
            imprimeLinha(linha, cabecalhoLinha);
            printf("\n");

            // Avança para o próximo veículo
            if (++indiceVeiculo < cabecalhoVeiculo.nroRegistros) {
                veiculo = leVeiculoBinario(binarioVeiculo);
            }
        } else if (veiculo.codLinha < linha.codLinha) {
            // Avança para o próximo veículo
            if (++indiceVeiculo < cabecalhoVeiculo.nroRegistros) {
                veiculo = leVeiculoBinario(binarioVeiculo);
            }
        } else {
            // Avança para a próxima linha
            if (++indiceLinha < cabecalhoLinha.nroRegistros) {
                linha = leLinhaBinario(binarioLinha);
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
}

/**
 * 
 * Auxiliares
 * 
 */

void _criaBinarioVeiculoOrdenado(char *nomeOriginal, char *nomeOrdenado, char *campo) {
    // Valida o campo
    if (!ehCampoOrdenavel(campo)) {
        printf("%s\n", FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Abre o arquivo original para leitura
    FILE *original = abreArquivo(nomeOriginal, "rb", 0);
    CabecalhoVeiculo cabecalhoOriginal = leCabecalhoVeiculoBinario(original);
    validaArquivo(cabecalhoOriginal.status, 1, original);

    // Abre o arquivo novo para escrita
    FILE *ordenado = abreArquivo(nomeOrdenado, "wb", 1, original);
    CabecalhoVeiculo cabecalhoOrdenado = criaCabecalhoVeiculoOrdenado(cabecalhoOriginal);
    escreveCabecalhoVeiculoBinario(cabecalhoOrdenado, ordenado);

    if (cabecalhoOriginal.nroRegistros > 0) {
        int nroTotalRegistros = cabecalhoOriginal.nroRegistros + cabecalhoOriginal.nroRegRemovidos;

        // Lê os veículos para a RAM
        Veiculo veiculos[cabecalhoOriginal.nroRegistros];
        leVeiculosBinario(veiculos, nroTotalRegistros, original);

        // Escreve os veículos ordenados
        ordenaVeiculos(veiculos, cabecalhoOriginal.nroRegistros);
        escreveVeiculosBinario(veiculos, cabecalhoOriginal.nroRegistros, ordenado);
    }

    // Atualiza o cabeçalho
    cabecalhoOrdenado.status = '1';
    cabecalhoOrdenado.byteProxReg = ftell(ordenado);
    cabecalhoOrdenado.nroRegistros = cabecalhoOriginal.nroRegistros;
    escreveCabecalhoVeiculoBinario(cabecalhoOrdenado, ordenado);

    // Fecha os arquivos
    fclose(original);
    fclose(ordenado);
}

void _criaBinarioLinhaOrdenado(char *nomeOriginal, char *nomeOrdenado, char *campo) {
    // Valida o campo
    if (!ehCampoOrdenavel(campo)) {
        printf("%s\n", FALHA_PROCESSAMENTO);
        exit(0);
    }

    // Abre o arquivo original para leitura
    FILE *original = abreArquivo(nomeOriginal, "rb", 0);
    CabecalhoLinha cabecalhoOriginal = leCabecalhoLinhaBinario(original);
    validaArquivo(cabecalhoOriginal.status, 1, original);

    // Abre o arquivo novo para escrita
    FILE *ordenado = abreArquivo(nomeOrdenado, "wb", 1, original);
    CabecalhoLinha cabecalhoOrdenado = criaCabecalhoLinhaOrdenado(cabecalhoOriginal);
    escreveCabecalhoLinhaBinario(cabecalhoOrdenado, ordenado);

    if (cabecalhoOriginal.nroRegistros > 0) {
        int nroTotalRegistros = cabecalhoOriginal.nroRegistros + cabecalhoOriginal.nroRegRemovidos;

        // Lê as linhas para a RAM
        Linha linhas[cabecalhoOriginal.nroRegistros];
        leLinhasBinario(linhas, nroTotalRegistros, original);

        // Escreve as linhas ordenados
        ordenaLinhas(linhas, cabecalhoOriginal.nroRegistros);
        escreveLinhasBinario(linhas, cabecalhoOriginal.nroRegistros, ordenado);
    }

    // Atualiza o cabeçalho
    cabecalhoOrdenado.status = '1';
    cabecalhoOrdenado.byteProxReg = ftell(ordenado);
    cabecalhoOrdenado.nroRegistros = cabecalhoOriginal.nroRegistros;
    escreveCabecalhoLinhaBinario(cabecalhoOrdenado, ordenado);

    // Fecha os arquivos
    fclose(original);
    fclose(ordenado);
}
