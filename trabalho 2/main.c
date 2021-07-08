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
        case 9:
            createIndexVeiculo();
            break;
        case 10:
            createIndexLinha();
            break;
        case 11:
            selectFromWhereIndexVeiculo();
            break;
        case 12:
            selectFromWhereIndexLinha();
            break;
        case 13:
            insertIntoIndexVeiculo();
            break;
        case 14:
            insertIntoIndexLinha();
            break;

        default:
            printf("Falha na identificacao da funcionalidade %d.", funcionalidade);
            exit(0);
            break;
    }

    return 0;
}