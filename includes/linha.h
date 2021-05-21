// Pedro Lucas de Moliner de Castro - 11795784
// Matheus Henrique de Cerqueira Pinto - 11911104

#ifndef LINHA_H
#define LINHA_H

#include <stdio.h>
#include <stdbool.h>


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


// Cabeçalho Linha
int leCabecalhoLinhaCsv(CabecalhoLinha *cabecalho, FILE *csv);
void leCabecalhoLinhaBinario(CabecalhoLinha *cabecalho, FILE *binario);
void escreveCabecalhoLinhaBinario(CabecalhoLinha *cabecalho, FILE *binario);

// Linha
int leLinhaCsv(Linha *linha, FILE *csv);
bool leLinhaBinario(Linha *linha, FILE *binario);
void escreveLinhaBinario(Linha *linha, FILE *binario);
void imprimeLinha(CabecalhoLinha cabecalhoLinha, Linha linha);

#endif  // LINHA_H
