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

// Lê o cabeçalho de uma Linha a partir de um CSV
void leCabecalhoLinhaCsv(CabecalhoLinha *cabecalhoLinha, FILE *csv);

// Lê o cabeçalho de uma Linha a partir de um binário
void leCabecalhoLinhaBinario(CabecalhoLinha *cabecalhoLinha, FILE *binario);

// Escreve o cabeçalho de uma Linha num binário
void escreveCabecalhoLinhaBinario(CabecalhoLinha *cabecalhoLinha, FILE *binario);



/** Corpo da Linha **/

// Lê uma Linha a partir de um CSV
int leLinhaCsv(Linha *linha, FILE *csv);

// Lê uma Linha a partir de um binário e retorna se ela foi removida
bool leLinhaBinario(Linha *linha, FILE *binario);

// Lê uma Linha a partir da entrada padrão
void leLinhaEntrada(Linha *linha);

// Escreve uma Linha num binário
void escreveLinhaBinario(Linha *linha, FILE *binario);

// Verifica se uma Linha corresponde ao campo e valor pesquisados
bool comparaLinha(Linha *linha, const char *campo, const char *valor);

// Imprime uma Linha
void imprimeLinha(CabecalhoLinha *cabecalhoLinha, Linha *linha);

#endif  // LINHA_H
