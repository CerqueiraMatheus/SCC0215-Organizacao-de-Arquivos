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


/** Binário **/
void posicionaBinarioProximoRegistroLinha(FILE *binario, CabecalhoLinha cabecalho);
void posicionaBinarioPrimeiroRegistroLinha(FILE *binario);

/** Cabeçalho da Linha **/
CabecalhoLinha leCabecalhoLinhaCsv(FILE *csv);
CabecalhoLinha leCabecalhoLinhaBinario(FILE *binario);
CabecalhoLinha criaCabecalhoLinhaOrdenado(CabecalhoLinha original);
void escreveCabecalhoLinhaBinario(CabecalhoLinha cabecalho, FILE *binario);

/** Linha **/
Linha leLinhaCsv(FILE *csv, bool *ehEOF);
Linha leLinhaBinario(FILE *binario);
Linha leLinhaEntrada();
void escreveLinhaBinario(Linha linha, FILE *binario);
void imprimeLinha(Linha linha, CabecalhoLinha cabecalho);
bool comparaCampoLinha(Linha *linha, const char *campo, const char *valor);

/** Linhas **/
void leLinhasBinario(Linha *linhas, int numero, FILE *binario);
void escreveLinhasBinario(Linha *linhas, int numero, FILE *binario);
void ordenaLinhas(Linha *linhas, int numero);


#endif  // LINHA_H
