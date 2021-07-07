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

/** Create Table Index **/
void createTableIndexVeiculo();
void createTableIndexLinha();

/** Create Where Index **/
void selectWhereIndexVeiculo();
void selectWhereIndexLinha();

/** Insert Into Index **/
void insertIntoIndexVeiculo();
void insertIntoIndexLinha();

#endif  // SQL_H