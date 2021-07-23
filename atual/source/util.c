/**
 * @author Matheus Henrique de Cerqueira Pinto (11911104)
 * @author Pedro Lucas de Moliner de Castro (11795784)
 * @date 2021-05-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "mensagem.h"
#include "util.h"


/**
 * 
 * Arquivos
 * 
 */

FILE *abreArquivo(const char *nome, const char *modo, int nroFechamentos, ...) {
    FILE *arquivo = fopen(nome, modo);

    if (arquivo == NULL) {
        va_list fechamentos;
        va_start(fechamentos, nroFechamentos);

        for (int i = 0; i < nroFechamentos; i++) {
            FILE *fechamento = va_arg(fechamentos, FILE *);
            fclose(fechamento);
        }

        va_end(fechamentos);

        printf("%s\n", FALHA_PROCESSAMENTO);
        exit(0);
    }

    return arquivo;
}

void validaArquivo(char status, int nroFechamentos, ...) {
    if (arquivoFoiCorrompido(status)) {
        va_list fechamentos;
        va_start(fechamentos, nroFechamentos);

        for (int i = 0; i < nroFechamentos; i++) {
            FILE *fechamento = va_arg(fechamentos, FILE *);
            fclose(fechamento);
        }

        va_end(fechamentos);

        printf("%s\n", FALHA_PROCESSAMENTO);
        exit(0);
    }
}


/**
 * 
 * CSV
 * 
 */

// Atribui valor a uma string a partir de um CSV
void leStringCsv(char *string, FILE *csv) {
    fscanf(csv, "%[^,\n]%*c", string);

    if (strcmp(string, "NULO") == 0) {
        strcpy(string, "");
    }
}

// Retorna um inteiro partir de um CSV
int leInteiroCsv(FILE *csv) {
    char inteiro[20];
    fscanf(csv, "%[^,\n]%*c", inteiro);

    return strcmp(inteiro, "NULO") == 0 ? -1 : atoi(inteiro);
}


/**
 * 
 * Binário
 * 
 */

// Atribui valor a uma string a partir de um binário
void leStringBinario(char *string, int tamanho, FILE *binario) {
    fread(string, sizeof(char), tamanho, binario);
    string[tamanho] = '\0';
}

// Adiciona lixo a um binário
void escreveLixoBinario(int numero, FILE *binario) {
    for (int i = 0; i < numero; i++) {
        fwrite("@", sizeof(char), 1, binario);
    }
}

// Adiciona uma string nula a um binário
void escreveStringNuloBinario(int tamanho, FILE *binario) {
    fwrite("\0", sizeof(char), 1, binario);
    escreveLixoBinario(tamanho - 1, binario);
}

// Altera o status de um binário ('0' -> invalido, '1' - > válido)
void atualizaStatusBinario(char status, FILE *binario) {
    long long int posicao = ftell(binario);

    fseek(binario, 0, SEEK_SET);
    fwrite(&status, sizeof(char), 1, binario);

    fseek(binario, posicao, SEEK_SET);
}


/**
 * 
 * Entrada
 * 
 */

// Retorna um inteiro a partir da entrada padrão
int leInteiroEntrada() {
    char inteiro[20];
    scanf(" %s", inteiro);

    return strcmp(inteiro, "NULO") == 0 ? -1 : atoi(inteiro);
}

void leStringsEntrada(int nroStrings, ...) {
    va_list strings;
    va_start(strings, nroStrings);

    char formato[3 * nroStrings + 1];
    formato[0] = '\0';
    for (int i = 0; i < nroStrings; i++) {
        strcat(formato, "%s ");
    }

    if (vscanf(formato, strings) != nroStrings) {
        printf("%s\n", FALHA_PROCESSAMENTO);
        exit(0);
    }

    va_end(strings);
}


/**
 * 
 * Conversão
 * 
 */

// Retorna uma string convertida em inteiro (-1 caso string = "")
int stringParaInteiro(const char *string) {
    return strcmp(string, "") == 0 ? -1 : atoi(string);
}


/**
 * 
 * Checagem de estados
 * 
 */

// Verifica se o status do arquivo equivale a '0'
bool arquivoFoiCorrompido(char status) {
    return status == '0';
}

// Verifica se o status de remoção arquivo equivale a '0'
bool registroFoiRemovido(char removido) {
    return removido == '0';
}


/**
 * 
 * Checagem de campos
 * 
 */

bool ehCampoOrdenavel(const char *campo) {
    return strcmp(campo, "codLinha") == 0;
}


/**
 * 
 * Impressão
 * 
 */

// Imprime um campo string (trata casos nulos)
void imprimeCampoString(const char *campo, int tamanho) {
    if (tamanho == 0) {
        printf("%s\n", CAMPO_NULO);
    }
    else {
        printf("%s\n", campo);
    }
}

// Imprime um campo inteiro (trata casos iguais a -1)
void imprimeCampoInteiro(int campo) {
    if (campo == -1) {
        printf("%s\n", CAMPO_NULO);
    }
    else {
        printf("%d\n", campo);
    }
}


/**
 * 
 * Comparação
 * 
 */

// Compara um campo e um valor string a um campo e um valor pesquisados
bool comparaCampoString(const char *campo, const char *campoRegistro, const char *valor, const char *valorRegistro) {
    return strcmp(campo, campoRegistro) == 0 && strcmp(valor, valorRegistro) == 0;
}

// Compara um campo e um valor inteiro a um campo e um valor pesquisados
bool comparaCampoInteiro(const char *campo, const char *campoRegistro, int valor, int valorRegistro) {
    return strcmp(campo, campoRegistro) == 0 && valor == valorRegistro;
}


/**
 * 
 * Funções fornecidas
 * 
 */

void binarioNaTela(char *nomeArquivoBinario) {
    unsigned long i, cs;
    unsigned char *mb;
    size_t fl;
    FILE *fs;
    if (nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
        fprintf(stderr,
                "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
        return;
    }
    fseek(fs, 0, SEEK_END);
    fl = ftell(fs);
    fseek(fs, 0, SEEK_SET);
    mb = (unsigned char *)malloc(fl);
    fread(mb, 1, fl, fs);

    cs = 0;
    for (i = 0; i < fl; i++) {
        cs += (unsigned long)mb[i];
    }
    printf("%lf\n", (cs / (double)100));
    free(mb);
    fclose(fs);
}

void scan_quote_string(char *str) {
    char R;

    while ((R = getchar()) != EOF && isspace(R))
        ;

    if (R == 'N' || R == 'n') {
        getchar();
        getchar();
        getchar();
        strcpy(str, "");
    } else if (R == '\"') {
        if (scanf("%[^\"]", str) != 1) {
            strcpy(str, "");
        }
        getchar();
    } else if (R != EOF) {
        str[0] = R;
        scanf("%s", &str[1]);
    } else {
        strcpy(str, "");
    }
}

int convertePrefixo(char *str) {
    /* O registro que tem essa string como chave foi removido */
    if (str[0] == '*')
        return -1;

    /* Começamos com o primeiro digito na ordem de 36^0 = 1 */
    int power = 1;

    /* Faz a conversão char por char para chegar ao resultado */
    int result = 0;
    for (int i = 0; i < 5; i++) {
        /* 
            Interpreta o char atual como se fosse um digito
            em base 36. Os digitos da base 36 são:
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D,
            E, F, G, H, I, J, K, L, M, N, O, P, Q, R,
            S, T, U, V, W, X, Y, Z
        */
        int cur_digit;
        /* Checa pelos digitos normais e os converte para números */
        if (str[i] >= '0' && str[i] <= '9')
            cur_digit = str[i] - '0';
        /* Checa pelas letras e as converte para números */
        else if (str[i] >= 'A' && str[i] <= 'Z')
            cur_digit = 10 + str[i] - 'A';

        /*
            Multiplica o digito atual pelo ordem da posição atual
            e adiciona no resultado
            Primeira posição:   36^0 = 1
            Segunda posição:    36^1 = 36
            Terceira posição:   36^2 = 1.296
            Quarta posição:     36^3 = 46.656
            Quinta posição:     36^4 = 1.679.616
        */
        result += cur_digit * power;

        /* Aumenta a ordem atual */
        power *= 36;
    }

    return result;
}
