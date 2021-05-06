// Pedro Lucas de Moliner de Castro - 11795784
// Matheus Henrique de Cerqueira Pinto - 11911104

#include <stdbool.h>


struct _CabecalhoLinha {
    bool status;
    long byteProxReg;
    int nroRegistros;
    int nroRegRemovidos;
    char descreveCodigo[15];
    char descreveCartao[13];
    char descreveNome[13];
    char descreveLinha[24];
};

struct _Linha {
    bool removido;
    int tamanhoRegistro;
    int codLinha;
    char aceitaCartao;
    int tamanhoNome;
    char *nomeLinha;
    int tamanhoCor;
    char *corLinha;
};
