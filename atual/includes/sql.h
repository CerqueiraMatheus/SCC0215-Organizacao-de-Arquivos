/**
 * @author Matheus Henrique de Cerqueira Pinto (11911104)
 * @author Pedro Lucas de Moliner de Castro (11795784)
 * @date 2021-05-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef SQL_H
#define SQL_H

/** Create Table **/
void createTableVeiculo();
void createTableLinha();

/** Select From **/
void selectFromVeiculo();
void selectFromLinha();

/** Select From Where **/
void selectFromWhereVeiculo();
void selectFromWhereLinha();

/** Insert Into **/
void insertIntoVeiculo();
void insertIntoLinha();

/** Create Index **/
void createIndexVeiculo();
void createIndexLinha();

/** Select From Where Index **/
void selectFromWhereIndexVeiculo();
void selectFromWhereIndexLinha();

/** Insert Into Index **/
void insertIntoIndexVeiculo();
void insertIntoIndexLinha();

/** Order By **/
void orderByVeiculo();
void orderByLinha();

/** Select From Join On **/
void selectFromJoinOnLoop();
void selectFromJoinOnIndex();
void selectFromJoinOnMerge();

#endif  // SQL_H