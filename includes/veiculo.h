/**
 * @author Matheus Henrique de Cerqueira Pinto (11911104)
 * @author Pedro Lucas de Moliner de Castro (11795784)
 * @date 2021-05-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef VEICULO_H
#define VEICULO_H

#include <stdbool.h>
#include <stdio.h>

typedef struct {
    char status;
    long long int byteProxReg;
    int nroRegistros;
    int nroRegRemovidos;
    char descrevePrefixo[19];
    char descreveData[36];
    char descreveLugares[43];
    char descreveLinha[27];
    char descreveModelo[18];
    char descreveCategoria[21];
} CabecalhoVeiculo;

typedef struct {
    char removido;
    int tamanhoRegistro;
    char prefixo[6];
    char data[11];
    int quantidadeLugares;
    int codLinha;
    int tamanhoModelo;
    char modelo[100];
    int tamanhoCategoria;
    char categoria[100];
} Veiculo;



/** Cabeçalho do Veículo **/

// Lê o cabeçalho de um Veículo a partir de um CSV
void leCabecalhoVeiculoCsv(CabecalhoVeiculo *cabecalhoVeiculo, FILE *csv);

// Lê o cabeçalho de um Veículo a partir de um binário
void leCabecalhoVeiculoBinario(CabecalhoVeiculo *cabecalhoVeiculo, FILE *binario);

// Escreve o cabeçalho de um Veículo num binário
void escreveCabecalhoVeiculoBinario(CabecalhoVeiculo *cabecalhoVeiculo, FILE *binario);



/** Corpo do Veículo **/

// Lê um Veículo a partir de um CSV
int leVeiculoCsv(Veiculo *veiculo, FILE *csv);

// Lê um Veículo a partir de um binário e retorna se ele foi removido
bool leVeiculoBinario(Veiculo *veiculo, FILE *binario);

// Lê um Veículo a partir da entrada padrão
void leVeiculoEntrada(Veiculo *veiculo);

// Escreve um Veículo num binário
void escreveVeiculoBinario(Veiculo *veiculo, FILE *binario);

// Verifica se um Veículo corresponde ao campo e valor pesquisados
bool comparaVeiculo(Veiculo *veiculo, const char *campo, const char *valor);

// Imprime um Veículo
void imprimeVeiculo(CabecalhoVeiculo *cabecalhoVeiculo, Veiculo *veiculo);

#endif  // VEICULO_H
