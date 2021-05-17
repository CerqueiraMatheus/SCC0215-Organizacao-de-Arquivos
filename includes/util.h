// Pedro Lucas de Moliner de Castro - 11795784
// Matheus Henrique de Cerqueira Pinto - 11911104

#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>


char *leStringCsv(FILE *csv);
const char *trataNuloString(const char *string);
int trataNuloInteiro(const char *string);
void escreveLixoBinario(int numero, FILE *binario);

// Funções fornecidas:
void binarioNaTela(char *nomeArquivoBinario);
void scan_quote_string(char *str);

#endif // UTIL_H
