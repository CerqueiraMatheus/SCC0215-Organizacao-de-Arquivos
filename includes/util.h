// Pedro Lucas de Moliner de Castro - 11795784
// Matheus Henrique de Cerqueira Pinto - 11911104

#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include <stdio.h>


// CSV
void leStringCsv(char *string, FILE *csv);
int leInteiroCsv(FILE *csv);

// Binário
void leStringBinario(char *string, int tamanho, FILE *binario);
void escreveStringNuloBinario(int tamanho, FILE *binario);

// Entrada
int leInteiroEntrada();

// Conversão
char *stringParaCampoString(char *string);
int stringParaCampoInteiro(char *string);
char *removeAspasString(char *string);

// Checagem
bool arquivoFoiCorrompido(char status);
bool registroFoiRemovido(char removido);

// Impressão
void imprimeCampoInteiro(int campo);
void imprimeCampoString(char *campo, int tamanho);
void imprimeData(char *data);
void imprimeAceitaCartao(char *aceitaCartao);

// Comparação
bool comparaRegistroCampoString(char *nomeCampo, char *nomeCampoPesquisado, char *valor, char *valorPesquisado);
bool comparaRegistroCampoInt(char *nomeCampo, char *nomeCampoPesquisado, int valor, char *valorPesquisado);

// Funções fornecidas:
void binarioNaTela(char *nomeArquivoBinario);
void scan_quote_string(char *str);

#endif  // UTIL_H
