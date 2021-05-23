// Pedro Lucas de Moliner de Castro - 11795784
// Matheus Henrique de Cerqueira Pinto - 11911104

#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include <stdio.h>

// Leitura
char *leStringArquivo(FILE *arquivo);

// Binário
void leStringBinario(char *string, int tamanho, FILE *binario);
void escreveStringNuloBinario(int tamanho, FILE *binario);

// Conversão
char *stringParaCampoString(char *string);
int stringParaCampoInteiro(char *string);
char *removeAspasString(char *string);

// Funções fornecidas:
void binarioNaTela(char *nomeArquivoBinario);
void scan_quote_string(char *str);

// Checagem
bool arquivoFoiCorrompido(char status);
bool registroFoiRemovido(char removido);

// Impressão
void imprimeCampoInteiro(int campo);
void imprimeCampoString(char *campo, int tamanho);
void imprimeData(char *data);
void imprimeAceitaCartao(char *aceitaCartao);

#endif  // UTIL_H
