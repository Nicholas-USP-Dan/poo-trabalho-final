/**
 * @file dyn-array.h
 * @brief Header file para a estrutura de um array dinâmico
 * 
 * @details A maioria das operações sobre este array pressupõem que a lista esteja ordenada, além disso não há
 * nenhuma função neste header que ordene este array, então fica por responsabilidade do programador ordenar o array 
 * por meio de um algoritmo qualquer para operar corretamente sobre o array.
 *
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 * 
 * @version 2.0
 * 
 */

#ifndef DYNAMIC_ARR_H
#define DYNAMIC_ARR_H

#include <stdint.h>

/**
 * @brief Estrutura de um elemento do array (genérico)
 */
struct _arr_el {
    void* el; /**< @brief Ponteiro para um espaço (dado arbitrário) alocado dinâmicamente. */
    int removed; /**< @brief Flag que verifica se o elemento está removido ou não. */
};

/**
 * @brief Tipo para o elemento do array (genérico)
 */
typedef struct _arr_el ARR_EL;

/**
 * @brief Tipo do array dinâmico
 */
typedef struct _dyn_array DYN_ARRAY;

/**
 * @brief Inicializa um novo array dinâmico.
 * 
 * @details Aloca-se um vetor de tamanho 1 no inicio
 * 
 * @param f_index Ponteiro de função que gera um index pelo qual ordenação possa ser feita, e assim busca binária
 * @return [DYN_ARRAY*] Retorna um ponteiro para um novo array dinâmico alocado em memória
 */
DYN_ARRAY* initialize_dynarr(int64_t (*f_index)(const void*));

/**
 * @brief Limpa memória gasta pelo array e pelos elementos dele
 * 
 * @details Como o array guarda ponteiros, é necessário limpar cada ponteiro individualmente
 * 
 * @param array Referência para o ponteiro do array dinâmico
 */
void clear_dynarr(DYN_ARRAY **array);

/**
 * @brief Retorna o número de elementos ocupados da lista (incluindo elementos removidos).
 * 
 * @param array Referência para o ponteiro do array dinâmico
 * @return [int64_t] Número de elementos ocupados na lista.
 */
int64_t get_len_dynarr(DYN_ARRAY **array);

/**
 * @brief Retorna o ponteiro do array (puro) utilizado por um array dinâmico.
 * 
 * @param array Referência para o ponteiro do array dinâmico
 * @return [ARR_EL*] Ponteiro para o array puro usado no array dinâmico
 */
ARR_EL *get_raw_dyarr(DYN_ARRAY **array);

/**
 * @brief Pega um valor no array em uma posição.
 * 
 * @param i Posição a ser recuperada
 * @param array Referência para o ponteiro do array dinâmico
 * 
 * @return [void*] Ponteiro da data guardada pelo array na posição dada
 * @retval NULL Retorna nulo caso a posição seja inválida ou se o elemento esteja removido
 */
void* get_dynarr(int64_t i, DYN_ARRAY **array);

/**
 * @brief Procura por um valor no array pelo índice
 *
 * @details Essa busca é binária (então supõe-se que a lista esteja ordenada)
 * 
 * @param index Índice do valor a ser procurado
 * @param array Referência para o ponteiro do array dinâmico
 * 
 * @return [int64_t] Posição do valor encontrado
 * @retval -1 Índice não existe no array
 */
int64_t find_pos_dynarr(int64_t index, DYN_ARRAY **array);

/**
 * @brief Insere um novo elemento (não removido) no final do array
 * 
 * @param el Dado a ser adicionado
 * @param array Referência para o ponteiro do array dinâmico
 * 
 * @retval -1 Houve uma falha interna durante a operação. (Aumento da memória do array não foi possível)
 * @retval 0 A operação foi realizada com sucesso.
 */
int insert_back_dynarr(void* el, DYN_ARRAY **array);

/**
 * @brief Insere um novo elemento (não removido) e ordena ele
 *
 * @details Primeiramente o elemento é inserido no final, para depois colocá-lo na posição correta
 * 
 * @param el Dado a ser adicionado
 * @param array Referência para o ponteiro do array dinâmico
 *
 * @retval -1 Houve uma falha interna durante a operação. (Aumento da memória do array não foi possível)
 * @retval 0 A operação foi realizada com sucesso.
 */
int insert_ord_dynarr(void* el, DYN_ARRAY **array);

/**
 * @brief Remove um elemento no array pela posição dele
 * 
 * @details O elemento não é removido fisicamente, ele é somente atribuído com flag de remoção, isso permite economizar 
 * processamento ao não requerir shifts
 * 
 * @param i Posição do valor a ser removido
 * @param array Referência para o ponteiro do array dinâmico
 * 
 * @retval -1 Houve uma falha interna durante a operação.
 * @retval 0 A operação foi realizada com sucesso.
 */
int remove_dynarr(int64_t i, DYN_ARRAY **array);

#endif