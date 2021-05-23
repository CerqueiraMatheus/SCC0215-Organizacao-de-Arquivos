// Pedro Lucas de Moliner de Castro - 11795784
// Matheus Henrique de Cerqueira Pinto - 11911104

#include "util.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *MENSAGEM_CAMPO_NULO = "campo com valor nulo";

// CSV

char *leStringCsv(FILE *csv) {
    char *string = NULL;
    size_t tamanho = 0;
    size_t comprimento = getline(&string, &tamanho, csv);

    if (comprimento == -1) {
        free(string);
        return NULL;
    } else {
        string[comprimento - 1] = '\0';
        return string;
    }
}

// Binário

void leStringBinario(char *string, int tamanho, FILE *binario) {
    fread(string, sizeof(char), tamanho, binario);
    string[tamanho] = '\0';
}

void escreveStringNuloBinario(int tamanho, FILE *binario) {
    fwrite("\0", sizeof(char), 1, binario);

    // Adiciona o lixo
    for (int i = 0; i < tamanho - 1; i++)
        fwrite("@", sizeof(char), 1, binario);
}

// Conversão

const char *stringParaCampoString(const char *string) {
    return strcmp(string, "NULO") == 0 ? "" : string;
}

int stringParaCampoInteiro(const char *string) {
    return strcmp(string, "NULO") == 0 ? -1 : atoi(string);
}

// Checagem

bool arquivoFoiCorrompido(char status) {
    return status == '0';
}

bool registroFoiRemovido(char removido) {
    return removido == '0';
}

// Impressão

void imprimeCampoInteiro(int campo) {
    if (campo == -1)
        printf("%s\n", MENSAGEM_CAMPO_NULO);
    else
        printf("%d\n", campo);
}

void imprimeCampoString(char *campo, int tamanho) {
    if (tamanho == 0)
        printf("%s\n", MENSAGEM_CAMPO_NULO);
    else
        printf("%s\n", campo);
}

void imprimeData(char *data) {
    if (data[0] == '\0') {
        printf("%s\n", MENSAGEM_CAMPO_NULO);
        return;
    }

    const char *MESES[12] = {
        "janeiro",
        "fevereiro",
        "março",
        "abril",
        "maio",
        "junho",
        "julho",
        "agosto",
        "setembro",
        "outubro",
        "novembro",
        "dezembro"};

    int dia, mes, ano;
    sscanf(data, "%d-%d-%d", &ano, &mes, &dia);

    printf("%02d de %s de %d\n", dia, MESES[mes - 1], ano);
}

void imprimeAceitaCartao(char *aceitaCartao) {
    if (aceitaCartao[0] == '\0') {
        printf("%s\n", MENSAGEM_CAMPO_NULO);
        return;
    }

    switch (aceitaCartao[0]) {
        case 'S':
            printf("PAGAMENTO SOMENTE COM CARTAO SEM PRESENCA DE COBRADOR\n");
            break;

        case 'N':
            printf("PAGAMENTO EM CARTAO E DINHEIRO\n");
            break;

        case 'F':
            printf("PAGAMENTO EM CARTAO SOMENTE NO FINAL DE SEMANA\n");
            break;
    }
}

void removeAspasString(char *string) {
    // Caso não haja aspas
    if (string[0] != '\"') return;

    // Caso haja, retira as aspas
    memmove(string, string + 1, strlen(string));
    string[strlen(string) - 1] = '\0';
}

// Funções fornecidas:

/* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
*  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */
void binarioNaTela(char *nomeArquivoBinario) {
    unsigned long i, cs;
    unsigned char *mb;
    size_t fl;
    FILE *fs;
    if (nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
        fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
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

/*
*	Use essa função para ler um campo string delimitado entre aspas (").
*	Chame ela na hora que for ler tal campo. Por exemplo:
*
*	A entrada está da seguinte forma:
*		nomeDoCampo "MARIA DA SILVA"
*
*	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
*		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
*		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
*/
void scan_quote_string(char *str) {
    char R;

    while ((R = getchar()) != EOF && isspace(R))
        ;  // ignorar espaços, \r, \n...

    if (R == 'N' || R == 'n') {  // campo NULO
        getchar();
        getchar();
        getchar();        // ignorar o "ULO" de NULO.
        strcpy(str, "");  // copia string vazia
    } else if (R == '\"') {
        if (scanf("%[^\"]", str) != 1) {  // ler até o fechamento das aspas
            strcpy(str, "");
        }
        getchar();          // ignorar aspas fechando
    } else if (R != EOF) {  // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
        str[0] = R;
        scanf("%s", &str[1]);
    } else {  // EOF
        strcpy(str, "");
    }
}
