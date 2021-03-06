/**
 * @author Matheus Henrique de Cerqueira Pinto (11911104)
 * @author Pedro Lucas de Moliner de Castro (11795784)
 * @date 2021-05-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include <stdio.h>

/**  Manipulação de CSV **/
void leStringCsv(char *string, FILE *csv);
int leInteiroCsv(FILE *csv);

/** Manipulação de binário **/
void leStringBinario(char *string, int tamanho, FILE *binario);
void escreveStringNuloBinario(int tamanho, FILE *binario);
void atualizaStatusBinario(char status, FILE *binario);

/** Leitura de entradas **/
int leInteiroEntrada();

/** Conversões **/
int stringParaInteiro(const char *string);

/** Checagem de estados **/
bool arquivoFoiCorrompido(char status);
bool registroFoiRemovido(char removido);

/** Comparação de valores **/
bool comparaCampoString(const char *campo, const char *campoRegistro, const char *valor, const char *valorRegistro);
bool comparaCampoInteiro(const char *campo, const char *campoRegistro, int valor, int valorRegistro);

/** Impressão de resultados **/
void imprimeCampoString(const char *campo, int tamanho);
void imprimeData(const char *data);
void imprimeAceitaCartao(const char *aceitaCartao);
void imprimeCampoInteiro(int campo);

/** Funções fornecidas **/
void binarioNaTela(char *nomeArquivoBinario);
void scan_quote_string(char *str);

#endif  // UTIL_H
