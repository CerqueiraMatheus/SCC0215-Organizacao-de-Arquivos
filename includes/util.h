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

// Impressão
void imprimeCampoInteiro(int campo);
void imprimeCampoString(char *campo, int tamanho);
void imprimeData(char *data);
void imprimeAceitaCartao(char *aceitaCartao);

// Checagem
bool foiRemovido(char c);

#endif  // UTIL_H
