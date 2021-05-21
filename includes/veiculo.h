// Pedro Lucas de Moliner de Castro - 11795784
// Matheus Henrique de Cerqueira Pinto - 11911104

#ifndef VEICULO_H
#define VEICULO_H

#include <stdio.h>
#include <stdbool.h>


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


// Cabeçalho Veículo
int leCabecalhoVeiculoCsv(CabecalhoVeiculo *cabecalho, FILE *csv);
void escreveCabecalhoVeiculoBinario(CabecalhoVeiculo *cabecalho, FILE *binario);
void leCabecalhoVeiculoBinario(CabecalhoVeiculo *cabecalhoVeiculo, FILE *binario);

// Veículo
int leVeiculoCsv(Veiculo *veiculo, FILE *csv);
void escreveVeiculoBinario(Veiculo *veiculo, FILE *binario);
bool leVeiculoBinario(Veiculo *veiculo, FILE *binario);

void printVeiculo(CabecalhoVeiculo cabecalhoVeiculo, Veiculo veiculo);

#endif  // VEICULO_H
