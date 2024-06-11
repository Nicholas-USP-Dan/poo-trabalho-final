/**
 * @file index-file.h
 * @brief Header file para as funcionalidades relacionadas ao arquivo de índice.
 * 
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 * 
 * @version 2.0
 * 
 */

#ifndef INDEX_FILE_H
#define INDEX_FILE_H

#include <stdio.h>

#include "adts/dyn-array.h"

/**
 * @brief Estrutura que contém os dados de um registro de índice
 */
struct _index_reg {
    int32_t index;
    int64_t offset;
};

/**
 * @brief Tipo da estrutura representando os dados de um registro de índice
 * 
 */
typedef struct _index_reg INDEX_REG;

/**
 * @brief Função para que o dyn_array possa adquirir o índice do registro;
 * É utilizado somente para a criação do array
 * 
 * @param index_reg 
 * @return [int64_t] 
 */
int64_t get_index(const void *index_reg);

/**
 * @brief Função que a partir de um arquivo de dados gera um array de índice
 * 
 * @param data_fptr Ponteiro para o arquivo de dados a ser lido
 * @return [DYN_ARRAY*] Retorna o ponteiro para o array alocado do índice
 */
DYN_ARRAY* generate_index(FILE *data_fptr);

/**
 * @brief Escreve a lista de índices em um arquivo de índices
 * 
 * @param index_arr Referência para o ponteiro da lista de índices
 * @param index_fptr Ponteiro para o arquivo de índices a ser escrito
 * 
 * @retval -1 Houve uma falha durante a escrita do arquivo de índice
 * @retval 0 Escrita realizada com sucesso.
 */
int write_index(DYN_ARRAY **index_arr, FILE *index_fptr);

/**
 * @brief Carrega uma lista de índices a partir de um arquivo de índices
 * 
 * @param index_fptr Ponteiro para o arquivo de índices a ser escrito
 * @return [DYN_ARRAY*] Retorna o ponteiro para a lista de índices lido
 */
DYN_ARRAY* load_index(FILE *index_fptr);

#endif