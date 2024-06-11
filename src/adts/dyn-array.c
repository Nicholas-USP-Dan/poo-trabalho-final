/**
 * @file dyn-array.c
 * @brief Source file para a estrutura de um array dinâmico
 * 
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 * 
 * @version 2.0
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "dyn-array.h"

/**
 * @brief Estrutura de um array dinâmico
 */
struct _dyn_array {
    int64_t last; /**< @brief Índice do último elemento do array*/
    int64_t max_size; /**< @brief Tamanho máximo de arr, aumenta em potências de 2*/
    ARR_EL *arr; /**< @brief Representação da heap como vetor*/
    int64_t (*f_index)(const void*); /**< @brief Função que retorna o índice de ordenação do array */
};

DYN_ARRAY* initialize_dynarr(int64_t (*f_index)(const void*)){
    DYN_ARRAY *array = malloc(sizeof(DYN_ARRAY));

    array->last = -1; // Como não há elementos ainda, a última posição é -1
    array->max_size = 1;
    array->arr = malloc(sizeof(ARR_EL));
    array->f_index = f_index;
    return array;
}

void clear_dynarr(DYN_ARRAY **array){
    // Limpa cada elemento do array
    for(int i = 0; i <= (*array)->last; i++){
        free((*array)->arr[i].el);
    }

    free((*array)->arr);
    free((*array));
    (*array) = NULL;
}

int64_t get_len_dynarr(DYN_ARRAY **array){
    return (*array)->last+1;
}

ARR_EL *get_raw_dyarr(DYN_ARRAY **array){
    return (*array)->arr;
}

void* get_dynarr(int64_t i, DYN_ARRAY **array){
    if(i > (*array)->last){
        return NULL;
    }

    // Verifica se o elemento na posição não está removido
    return !((*array)->arr[i].removed) ? (*array)->arr[i].el : NULL;
}

int64_t find_pos_dynarr(int64_t index, DYN_ARRAY **array){
    // Algoritmo de busca binária simples
    int64_t beg = 0;
    int64_t end = (*array)->last;

    while(beg <= end){
        int64_t mid = beg + (end-beg)/2;

        int64_t mid_index = (*(*array)->f_index)((*array)->arr[mid].el);

        if(mid_index == index && !(*array)->arr[mid].removed){
            return mid;
        }
        else if(index > mid_index){
            beg = mid+1;
        }
        else{
            end = mid-1;
        }
    }

    return -1;
}

int insert_back_dynarr(void* el, DYN_ARRAY **array){
    // Verifica a necessidade de expandir o array
    if((*array)->last >= (*array)->max_size-1){
        (*array)->max_size = ((*array)->max_size*2);
        
        // Verifica se o realloc foi feito com sucesso
        if(!((*array)->arr = realloc((*array)->arr, sizeof(ARR_EL) * (*array)->max_size))) {
            return -1;
        }
    }

    // Insere o elemento na primeira posição não ocupada
    (*array)->arr[++(*array)->last] = (ARR_EL){
        .el = el,
        .removed = 0
    };

    return 0;
}

int insert_ord_dynarr(void* el, DYN_ARRAY **array){
    if(insert_back_dynarr(el, array) == -1) {
        return -1;
    }

    int64_t i = (*array)->last;

    ARR_EL el_aux = (*array)->arr[i];

    for(; i > 0 && (*(*array)->f_index)((*array)->arr[i-1].el) > (*(*array)->f_index)(el_aux.el); i--){
        (*array)->arr[i] = (*array)->arr[i-1];
    }

    (*array)->arr[i] = el_aux;

    return 0;
}


int remove_dynarr(int64_t i, DYN_ARRAY **array){
    (*array)->arr[i].removed = 1;
    return 0;
}