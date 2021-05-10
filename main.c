// Pedro Lucas de Moliner de Castro - 11795784
// Matheus Henrique de Cerqueira Pinto - 11911104

#include <stdio.h>


enum Opcoes {
    CREATE_TABLE_VEICULO = 1,
    CREATE_TABLE_LINHA,
    SELECT_FROM_VEICULO,
    SELECT_FROM_LINHA,
    SELECT_FROM_WHERE_VEICULO,
    SELECT_FROM_WHERE_LINHA,
    INSERT_INTO_VEICULO,
    INSERT_INTO_LINHA
};

int main() {
    enum Opcoes opcao;
    scanf("%d", &opcao);

    switch (opcao) {
        case CREATE_TABLE_VEICULO:
            break;
        case CREATE_TABLE_LINHA:
            break;
        case SELECT_FROM_VEICULO:
            break;
        case SELECT_FROM_LINHA:
            break;
        case SELECT_FROM_WHERE_VEICULO:
            break;
        case SELECT_FROM_WHERE_LINHA:
            break;
        case INSERT_INTO_VEICULO:
            break;
        case INSERT_INTO_LINHA:
            break;
        default:
            break;
    }

    return 0;
}
