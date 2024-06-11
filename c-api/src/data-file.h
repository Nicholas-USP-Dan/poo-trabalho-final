/**
 * @file data-file.h
 * @brief Header file para as funcionalidades relacionadas aos arquivos binarios de dado.
 * 
 * @note Cada implementação está separada em um arquivo diferente para diminuir a linhas de código de cada arquivo.
 * 
 * @note Quando as funções das funcionalidades do projeto retornam -1, isso significa que houve um erro durante
 * a execução de uma função nativa do stdio, logo é possível extrair o erro obtido através da variável errno
 * 
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 * 
 * @version 2.0
 * 
 */

#ifndef DATA_FILE_H
#define DATA_FILE_H

#include <stdio.h>

#include "adts/dyn-array.h"
#include "utils/removed-list.h"

/**
 * @brief Estrutura que contém os dados de um jogador, representa um registro no arquivo de dados.
 * 
 * @note As strings contidas dentro da estrutura terminam com '\0' para compatibilidade do processamento pelas funções
 * da biblioteca string.h
 */
struct _jogador {
    int32_t id; /**< id do jogador (32 bits ou 4 bytes) */
    int32_t idade; /**< idade do jogador (32 bits ou 4 bytes) */
    char *nome; /**< nome do jogador (string terminada em char nulo de tamanho variável)*/
    char *nac; /**< nacionalidade do jogador (string terminada em char nulo de tamanho variável)*/
    char *clube; /**< clube do jogador (string terminada em char nulo de tamanho variável)*/
};

/**
 * @brief Tipo da estrutura representando os dados de um jogador
 */
typedef struct _jogador JOGADOR;

/**
 * @brief Cria um arquivo binario de dados a partir de um arquivo csv
 * 
 * @param csv_fptr Ponteiro para o arquivo de entrada csv
 * @param data_fptr Ponteiro para o arquivo binário de saída
 * 
 * @retval -1 Houve uma falha interna durante a operação.
 * @retval 0 A operação foi realizada com sucesso.
 */
int create_data_file(FILE *csv_fptr, FILE *data_fptr);

/**
 * @brief Filtra os registros válidos (não removidos) de um arquivo binário de dados
 * e imprime eles
 * 
 * @details A função lê uma linha de busca com vários campos e realiza a busca conforme os critérios.
 * Formato de uma linha de busca:
   @verbatim
   M nomeCampo1 valorCampo1 ... nomeCampoM valorCampoM (M pares de nome e valor)
   @endverbatim 
 * 
 * @param data_fptr Ponteiro para o arquivo de dados
 * @param where Critério de filtragem
 * 
 * @note Caso a busca contenha um critério de id, quando a função encontrar um jogador com este id, ele termina a busca.
 * 
 * @retval -1 Houve uma falha interna durante a operação.
 * @retval 0 A operação foi realizada com sucesso.
 */
int select_data(FILE *data_fptr, JOGADOR where);

/**
 * @brief Remove registros no arquivo binário de dados que cumprem um critério.
 * 
 * @param data_fptr Ponteiro para o arquivo binário de saída.
 * @param index_filename Nome do arquivo de índice.
 * @param [out] quant_rem Referência de uma variável para guardar a quantidade de registros removidos com a
 * operação
 * @param rem_list Referência para o ponteiro da lista de registros removidos.
 * @param index_arr Referência para o ponteiro da lista de índices.
 * 
 * @retval -1 Houve uma falha interna durante a operação.
 * @retval 0 A operação foi realizada com sucesso.
 */
int delete_data(FILE *data_fptr, const JOGADOR where, int *quant_rem, REM_LIST **rem_list, DYN_ARRAY **index_arr);

/**
 * @brief Insere um registro em um arquivo binário de dados.
 * 
 * @details Essa função utiliza-se do método Best Fit para encontrar espaços para ocupar; No método Best Fit
 * procura-se um espaço que tenha o menor espaço sobrando ao ser preenchido com o novo registro
 * 
 * @param data_fptr Ponteiro para o arquivo binário de saída
 * @param index_fptr Ponteiro para o arquivo de índice.
 * 
 * @retval -1 Houve uma falha interna durante a operação.
 * @retval 0 A operação foi realizada com sucesso.
 */
int insert_data(FILE *data_fptr, const JOGADOR where, int *quant_ins, REM_LIST **rem_list, DYN_ARRAY **index_arr);

#endif