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

// Struct para o cabeçalho do Veículo
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

// Struct para o corpo do Veículo
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

// Funções para o cabeçalho do Veículo
void leCabecalhoVeiculoBinario(CabecalhoVeiculo *cabecalhoVeiculo, FILE *binario);
void escreveCabecalhoVeiculoBinario(CabecalhoVeiculo *cabecalhoVeiculo, FILE *binario);

// Funções para Veículo
bool leVeiculoBinario(Veiculo *veiculo, FILE *binario);
void leVeiculoEntrada(Veiculo *veiculo);
void escreveVeiculoBinario(Veiculo *veiculo, FILE *binario);
void imprimeVeiculo(CabecalhoVeiculo *cabecalhoVeiculo, Veiculo *veiculo);

#endif  // VEICULO_H
