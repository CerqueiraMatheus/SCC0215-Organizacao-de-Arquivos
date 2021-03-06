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
void leCabecalhoVeiculoCsv(CabecalhoVeiculo *cabecalhoVeiculo, FILE *csv);
void leCabecalhoVeiculoBinario(CabecalhoVeiculo *cabecalhoVeiculo, FILE *binario);
void escreveCabecalhoVeiculoBinario(CabecalhoVeiculo *cabecalhoVeiculo, FILE *binario);

/** Corpo do Veículo **/
int leVeiculoCsv(Veiculo *veiculo, FILE *csv);
bool leVeiculoBinario(Veiculo *veiculo, FILE *binario);
void leVeiculoEntrada(Veiculo *veiculo);
void escreveVeiculoBinario(Veiculo *veiculo, FILE *binario);
bool comparaVeiculo(Veiculo *veiculo, const char *campo, const char *valor);
void imprimeVeiculo(CabecalhoVeiculo *cabecalhoVeiculo, Veiculo *veiculo);

#endif  // VEICULO_H
