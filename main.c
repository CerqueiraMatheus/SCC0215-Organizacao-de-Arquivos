// Pedro Lucas de Moliner de Castro - 11795784
// Matheus Henrique de Cerqueira Pinto - 11911104

#include <stdio.h>


enum Opcoes {
    CRIAR_TABELA_VEICULO = 1,
    CRIAR_TABELA_LINHA,
    LER_TABELA_VEICULO,
    LER_TABELA_LINHA,
    FILTRAR_TABELA_VEICULO,
    FILTRAR_TABELA_LINHA,
    INSERIR_VEICULO,
    INSERIR_LINHA
};

int main() {
    enum Opcoes opcao;
    scanf("%d", &opcao);

    switch (opcao) {
        case /* constant-expression */:
            /* code */
            break;

        default:
            break;
    }

    return 0;
}
