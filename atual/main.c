/**
 * @author Matheus Henrique de Cerqueira Pinto (11911104)
 * @author Pedro Lucas de Moliner de Castro (11795784)
 * @date 2021-05-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <stdlib.h>

#include "sql.h"

int main() {
    int funcionalidade;
    scanf("%d", &funcionalidade);

    switch (funcionalidade) {
        case 1:
            createTableVeiculo();
            break;
        case 2:
            createTableLinha();
            break;
        case 3:
            selectFromVeiculo();
            break;
        case 4:
            selectFromLinha();
            break;
        case 5:
            selectFromWhereVeiculo();
            break;
        case 6:
            selectFromWhereLinha();
            break;
        case 7:
            insertIntoVeiculo();
            break;
        case 8:
            insertIntoLinha();
            break;
        case 9:
            createTableIndexVeiculo();
            break;

        default:
            printf("Falha na identificacao da funcionalidade %d.", funcionalidade);
            exit(0);
            break;
    }

    return 0;
}