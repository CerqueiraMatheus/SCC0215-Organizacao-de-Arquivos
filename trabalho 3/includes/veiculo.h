/**
 * @author Matheus Henrique de Cerqueira Pinto (11911104)
 * @author Pedro Lucas de Moliner de Castro (11795784)
 * @date 2021-07-22
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
CabecalhoVeiculo leCabecalhoVeiculoBinario(FILE *binario);
CabecalhoVeiculo criaCabecalhoVeiculoOrdenado(CabecalhoVeiculo original);
void escreveCabecalhoVeiculoBinario(CabecalhoVeiculo cabecalho, FILE *binario);

/** Veículo **/
Veiculo leVeiculoBinario(FILE *binario);
void escreveVeiculoBinario(Veiculo veiculo, FILE *binario);
void imprimeVeiculo(Veiculo veiculo, CabecalhoVeiculo cabecalho);

/** Veículos **/
void leVeiculosBinario(Veiculo *veiculos, int numero, FILE *binario);
void escreveVeiculosBinario(Veiculo *veiculos, int numero, FILE *binario);
void ordenaVeiculos(Veiculo *veiculos, int numero);

#endif  // VEICULO_H
