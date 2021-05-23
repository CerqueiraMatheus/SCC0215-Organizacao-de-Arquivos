// Pedro Lucas de Moliner de Castro - 11795784
// Matheus Henrique de Cerqueira Pinto - 11911104

#ifndef LINHA_H
#define LINHA_H

#include <stdbool.h>
#include <stdio.h>

#define STR_COD "codLinha"
#define STR_CARTAO "aceitaCartao"
#define STR_NOME "nomeLinha"
#define STR_COR "corLinha"

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
void leCabecalhoLinhaCsv(CabecalhoLinha *cabecalhoLinha, FILE *csv);
void leCabecalhoLinhaBinario(CabecalhoLinha *cabecalhoLinha, FILE *binario);
void escreveCabecalhoLinhaBinario(CabecalhoLinha *cabecalhoLinha, FILE *binario);

// Linha
int leLinhaCsv(Linha *linha, FILE *csv);
bool leLinhaBinario(Linha *linha, FILE *binario);
void leLinhaEntrada(Linha *linha);
void escreveLinhaBinario(Linha *linha, FILE *binario);
void imprimeLinha(CabecalhoLinha cabecalhoLinha, Linha linha);
bool comparaLinha(Linha linha, char *nomeCampo, char *valor);

#endif  // LINHA_H
