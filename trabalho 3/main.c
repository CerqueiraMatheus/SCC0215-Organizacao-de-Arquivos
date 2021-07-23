/**
 * @author Matheus Henrique de Cerqueira Pinto (11911104)
 * @author Pedro Lucas de Moliner de Castro (11795784)
 * @date 2021-07-22
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
