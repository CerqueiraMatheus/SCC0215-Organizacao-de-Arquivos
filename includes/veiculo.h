// Pedro Lucas de Moliner de Castro - 11795784
// Matheus Henrique de Cerqueira Pinto - 11911104

#ifndef VEICULO_H
#define VEICULO_H

#include <stdbool.h>


typedef struct {
    bool status;
    long byteProxReg;
    int nroRegistros;
    int nroRegRemovidos;
    char descrevePrefixo[18];
    char descreveData[35];
    char descreveLugares[42];
    char descreveLinha[17];
    char descreveModelo[20];
    char descreveCategoria[26];
} CabecalhoVeiculo;

typedef struct {
    bool removido;
    int tamanhoRegistro;
    char prefixo[5];
    char data[10];
    int quantidadeLugares;
    int codLinha;
    int tamanhoModelo;
    char *modelo;
    int tamanhoCategoria;
    char *categoria;
} Veiculo;

#endif // VEICULO_H
