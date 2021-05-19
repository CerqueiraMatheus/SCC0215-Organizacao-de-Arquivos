// Pedro Lucas de Moliner de Castro - 11795784
// Matheus Henrique de Cerqueira Pinto - 11911104

#include "util.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Leitura e Escrita

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

void escreveStringNuloBinario(int tamanho, FILE *binario) {
    fwrite("\0", sizeof(char), 1, binario);

    // Adiciona o lixo
    for (int i = 0; i < tamanho - 1; i++)
        fwrite("@", sizeof(char), 1, binario);
}

// Conversão

const char *stringParaCampoString(const char *string) {
    if (strcmp(string, "NULO") == 0)
        return "";
    else
        return string;
}

int stringParaCampoInteiro(const char *string) {
    if (strcmp(string, "NULO") == 0)
        return -1;
    else
        return atoi(string);
}

bool foiRemovido(char c) {
    return (c == '0') ? true : false;
}

void printfTrataNuloString(char *string) {
    if (*string)
        printf("%s\n", string);
    else
        printf("campo com valor nulo\n");
}

void printfTrataNuloInt(int n) {
    if (n != -1)
        printf("%d\n", n);
    else
        printf("campo com valor nulo\n");
}

void printfTrataNuloVariavel(char *string, int tamanho) {
    if (tamanho != 0)
        printf("%s\n", string);
    else
        printf("campo com valor nulo\n");
}

void printTrataNuloDataExtenso(char *string) {
    if (!*string) {
        printf("campo com valor nulo\n");
        return;
    }

    char *meses[12] = {"janeiro",
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
    sscanf(string, "%d-%d-%d", &ano, &mes, &dia);
    printf("%02d de %s de %d\n", dia, meses[mes - 1], ano);
}

void printTrataNuloPagamentoExtenso(char *string) {
    if (!*string) {
        printf("campo com valor nulo\n");
        return;
    }

    switch (string[0]) {
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
