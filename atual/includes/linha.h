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



/** Cabeçalho da Linha **/
void leCabecalhoLinhaCsv(CabecalhoLinha *cabecalhoLinha, FILE *csv);
CabecalhoLinha leCabecalhoLinhaBinario(FILE *binario);
void escreveCabecalhoLinhaBinario(CabecalhoLinha cabecalhoLinha, FILE *binario);

/** Corpo da Linha **/
int leLinhaCsv(Linha *linha, FILE *csv);
Linha leLinhaBinario(FILE *binario);
Linha leLinhaEntrada();
void escreveLinhaBinario(Linha linha, FILE *binario);
void imprimeLinha(Linha linha, CabecalhoLinha cabecalho);
bool comparaLinha(Linha *linha, const char *campo, const char *valor);

#endif  // LINHA_H
