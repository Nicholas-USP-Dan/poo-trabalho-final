/**
 * @file removed-list.h
 * @brief Header file para a estrutura da lista de registros removidos
 * 
 * @details Junto à um array dinâmico, esta lista irá conter qual método de reaproveitamento utilizar, sendo eles 
 * FIRST_FIT, BEST_FIT e WORST_FIT; Dependendo do método de reaproveitamento e da operação sendo realizada,
 * é necessário garantir que a lista esteja ordenada antes da função ser chamada, isso é possível, chamando a função sort_rem_list()
 * antes de chamar a operação.
 *
 * @note A funcionalidade de adicionar está contida no dyn_array (junto com outras manipulações), assim sendo necessário
 * operar sobre o array contido na lista de removidos (extraído por REM_LIST.arr)
 * 
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 * 
 * @version 2.0
 * 
 */

#ifndef REMOVED_LIST_H
#define REMOVED_LIST_H

#include <stdio.h>
#include <stdint.h>

#include "../adts/dyn-array.h"

/**
 * @brief Estrutura de um elemento na lista de removidos
 * 
 */
struct _rem_el {
    int64_t offset; /**< Offset do registro removido */
    int32_t tam; /**< Tamanho do registro removido */
};

/**
 * @brief Tipo para o elemento removido
 * 
 */
typedef struct _rem_el REM_EL;

/**
 * @brief Enumeração para os métodos de reaproveitamento de espaço
 * 
 */
enum alloc_met {
    FIRST_FIT, /**< Pega o primeiro espaço com tamanho que receba o registro (sem ordenação) */
    BEST_FIT, /**< Pega o espaço que tenha o menor resto de espaço (ordenação crescente) */
    WORST_FIT /**< Pega o maior espaço para o registro (ordenação decrescente); bom para edições */
};

/**
 * @brief Estrutura para a lista de removidos
 * 
 */
struct _rem_list {
    DYN_ARRAY *arr; /**< Array dinâmico da lista; as inserções e remoções são realizadas diretamente no array */
    enum alloc_met met; /**< Método de reaproveitamento de espaço escolhido */
};

/**
 * @brief Tipo para a lista de registros removidos
 * 
 */ 
typedef struct _rem_list REM_LIST;

/**
 * @brief Inicializa uma nova lista de removidos
 * 
 * @param met Método de reaproveitamento de espaço a ser utilizado pela lista
 * @return [REM_LIST*] Returna o ponteiro da lista alocada
 */
REM_LIST* initialize_rem_list(const enum alloc_met met);

/**
 * @brief Limpa a memória alocada por uma lista de removidos
 * 
 * @param list Referência para o ponteiro da lista de removidos
 */
void clear_rem_list(REM_LIST **list);

/**
 * @brief Carrega a lista de removidos a partir de um arquivo de dados
 * 
 * @param data_fptr Arquivo de dados da lista
 * @param met Metodo de reaproveitamento de espaço
 * 
 * @return [REM_LIST*] Ponteiro para uma nova lista de removidos
*/
REM_LIST* load_rem_list(FILE *data_fptr, const enum alloc_met met);

/**
 * @brief Acha um espaço livre em uma lista de registros removidos de acordo com o método definido; 
 * caso não haja espaço disponível retorna -1
 * 
 * @param tam Tamanho do registro procurado
 * @param list Referência para o ponteiro da lista de remoção
 * 
 * @return [REM_EL] Retorna o elemento com o tamanho válido, caso não tenha espaço retorna o registro
 
*/
REM_EL find_space(const int32_t tam, REM_LIST **list);

/**
 * @brief Escreve a lista de registros removidos no arquivo de dados
 * 
 * @param list Referência para o ponteiro da lista de removidos
 * @param data_fptr Ponteiro para o arquivo de dados
 * 
 * @retval -1 Houve uma falha durante a escrita da lista.
 * @retval 0 Escrita realizada com sucesso.
 */
int write_rem_list(REM_LIST **list, FILE *data_fptr);

/**
 * @brief Algoritmo estável que ordena a lista de removidos
 * 
 * @param list Referência para o ponteiro da lista de removidos
 */
void sort_rem_list(REM_LIST **list);

#endif