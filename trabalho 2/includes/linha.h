/**
 * @author Matheus Henrique de Cerqueira Pinto (11911104)
 * @author Pedro Lucas de Moliner de Castro (11795784)
 * @date 2021-05-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef LINHA_H
#define LINHA_H

#include <stdbool.h>
#include <stdio.h>

// Struct para o cabeçalho da Linha
typedef struct {
    char status;
    long long int byteProxReg;
    int nroRegistros;
    int nroRegRemovidos;
    char descreveCodigo[16];
    char descreveCartao[14];
    char descreveNome[14];
    char descreveLinha[25];
} CabecalhoLinha;

// Struct para Linha
typedef struct {
    char removido;
    int tamanhoRegistro;
    int codLinha;
    char aceitaCartao[2];
    int tamanhoNome;
    char nomeLinha[100];
    int tamanhoCor;
    char corLinha[100];
} Linha;

// Funções para o cabeçalho da Linha
void leCabecalhoLinhaBinario(CabecalhoLinha *cabecalhoLinha, FILE *binario);
void escreveCabecalhoLinhaBinario(CabecalhoLinha *cabecalhoLinha, FILE *binario);

// Funções para Linha
bool leLinhaBinario(Linha *linha, FILE *binario);
void leLinhaEntrada(Linha *linha);
void escreveLinhaBinario(Linha *linha, FILE *binario);
void imprimeLinha(CabecalhoLinha *cabecalhoLinha, Linha *linha);

#endif  // LINHA_H
