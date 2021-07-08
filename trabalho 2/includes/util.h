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

// Manipulação de binário
void leStringBinario(char *string, int tamanho, FILE *binario);
void escreveLixoBinario(int numero, FILE *binario);
void escreveStringNuloBinario(int tamanho, FILE *binario);
void atualizaStatusBinario(char status, FILE *binario);

// Leitura de entradas
int leInteiroEntrada();

// Checagem de estados
bool arquivoFoiCorrompido(char status);
bool registroFoiRemovido(char removido);

// Impressão de resultados
void imprimeCampoString(const char *campo, int tamanho);
void imprimeData(const char *data);
void imprimeAceitaCartao(const char *aceitaCartao);
void imprimeCampoInteiro(int campo);

// Funções fornecidas
void binarioNaTela(char *nomeArquivoBinario);
void scan_quote_string(char *str);
int convertePrefixo(char *str);

#endif  // UTIL_H
