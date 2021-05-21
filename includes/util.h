// Pedro Lucas de Moliner de Castro - 11795784
// Matheus Henrique de Cerqueira Pinto - 11911104

#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdbool.h>


// Leitura e Escrita
char *leStringCsv(FILE *csv);
void escreveStringNuloBinario(int tamanho, FILE *binario);

// Conversão
const char *stringParaCampoString(const char *string);
int stringParaCampoInteiro(const char *string);

// Funções fornecidas:
void binarioNaTela(char *nomeArquivoBinario);
void scan_quote_string(char *str);

bool foiRemovido(char c);
void printTrataNuloString(char *string);
void printTrataNuloInt(int n);
void printTrataNuloVariavel(char *string, int tamanho);
void printTrataNuloDataExtenso(char *string);
void printTrataNuloPagamentoExtenso(char *string);

#endif  // UTIL_H
