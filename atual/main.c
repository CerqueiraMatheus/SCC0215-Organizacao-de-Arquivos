/**
 * @author Matheus Henrique de Cerqueira Pinto (11911104)
 * @author Pedro Lucas de Moliner de Castro (11795784)
 * @date 2021-05-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdlib.h>
#include <stdio.h>

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
        case 15:
            selectFromJoinOnLoop();
            break;
        case 16:
            selectFromJoinOnIndex();
            break;
        case 17:
            orderByVeiculo();
            break;
        case 18:
            orderByLinha();
            break;
        case 19:
            selectFromJoinOnMerge();
            break;
        default:
            printf("Falha na identificacao da funcionalidade %d.", funcionalidade);
            exit(0);
            break;
    }

    return 0;
}
