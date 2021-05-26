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

// Atribui valor a uma string a partir de um CSV
void leStringCsv(char *string, FILE *csv);

// Retorna um inteiro partir de um CSV
int leInteiroCsv(FILE *csv);



/** Manipulação de binário **/

// Atribui valor a uma string a partir de um binário
void leStringBinario(char *string, int tamanho, FILE *binario);

// Adiciona uma string nula a um binário
void escreveStringNuloBinario(int tamanho, FILE *binario);

// Altera o status de um binário ('0' -> invalido, '1' - > válido)
void atualizaStatusBinario(char status, FILE *binario);



/** Leitura de entradas **/

// Retorna um inteiro a partir da entrada padrão
int leInteiroEntrada();



/** Conversões **/

// Retorna uma string convertida em inteiro (-1 caso string = "")
int stringParaInteiro(const char *string);



/** Checagem de estados **/

// Verifica se o status do arquivo equivale a '0'
bool arquivoFoiCorrompido(char status);

// Verifica se o status de remoção arquivo equivale a '0'
bool registroFoiRemovido(char removido);



/** Comparação de valores **/

// Compara um campo e um valor string a um campo e um valor pesquisados
bool comparaCampoString(const char *campo, const char *campoRegistro, const char *valor, const char *valorRegistro);

// Compara um campo e um valor inteiro a um campo e um valor pesquisados
bool comparaCampoInteiro(const char *campo, const char *campoRegistro, int valor, int valorRegistro);



/** Impressão de resultados **/

// Imprime um campo string (trata casos nulos)
void imprimeCampoString(const char *campo, int tamanho);

// Imprime uma data por extenso (trata casos nulos)
void imprimeData(const char *data);

// Imprime o aceite de cartão por extenso (trata casos nulos)
void imprimeAceitaCartao(const char *aceitaCartao);

// Imprime um campo inteiro (trata casos iguais a -1)
void imprimeCampoInteiro(int campo);



/** Funções fornecidas **/

void binarioNaTela(char *nomeArquivoBinario);
void scan_quote_string(char *str);

#endif  // UTIL_H
