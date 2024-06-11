/**
 * @file removed-list.c
 * @brief Source file para a estrutura da lista de registros removidos
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

#include "removed-list.h"

#include "campo-utils.h"
#include "cabecalho-utils.h"

/**
 * @brief Função para pegar o tamanho de REM_EL (critério de ordenação do array)
 * 
 * @param a Ponteiro para um dado
 * @return [int64_t] Retorna o tamanho do registro removido
 */
static int64_t get_tam(const void *a){
    return ((REM_EL*)a)->tam;
}

REM_LIST* initialize_rem_list(const enum alloc_met met){
    REM_LIST *list = malloc(sizeof(REM_LIST));
    list->met = met;

    list->arr = initialize_dynarr(&get_tam);

    return list;
}

void clear_rem_list(REM_LIST **list){
    clear_dynarr(&(*list)->arr);
    free(*list);
    (*list) = NULL;
}

REM_LIST* load_rem_list(FILE *data_fptr, const enum alloc_met met){
    REM_LIST *list = initialize_rem_list(met);

    // Lê o campo TOPO
    fseek(data_fptr, TOPO_OFFSET, SEEK_SET);
    int64_t offset = get_campo64(data_fptr);

    // Continua o loop enquanto a lista conter elementos válidos
    while(offset != -1){
        fseek(data_fptr, offset+1, SEEK_SET);

        // Cria uma instância para o registro removido encontrado
        REM_EL *el = malloc(sizeof(REM_EL));
        el->offset = offset;
        el->tam = get_campo32(data_fptr);
        
        // Insere o registro na lista (mudar isso para no final ordenar a lista)
        insert_back_dynarr(el, &list->arr);

        // Ir para o próximo elemento da fila
        offset = get_campo64(data_fptr);
    }

    return list;
}

REM_EL find_space(const int32_t tam, REM_LIST **list){
    REM_EL el = {-1,0}, *el_aux = NULL;

    switch ((*list)->met) {
        case FIRST_FIT: // Array não precisa estar ordenado
        case BEST_FIT: // Array deve estar ordenado
            // Iterar pela lista até achar um elemento válido
            for(int i = 0; i < get_len_dynarr(&(*list)->arr); i++){
                el_aux = (REM_EL*)get_dynarr(i, &(*list)->arr);
                if(!el_aux) continue;

                if(tam <= el_aux->tam){
                    el = *el_aux;
                    remove_dynarr(i, &(*list)->arr);
                    break;
                }
            }
            break;
        case WORST_FIT:
            // Pega o maior valor (array ordenado decrescente) e verifica se o tamanho é válido
            while(!el_aux){
                el_aux = (REM_EL*)get_dynarr(0, &(*list)->arr);
            }
            if(tam <= el_aux->tam){
                el = *el_aux;
                remove_dynarr(0, &(*list)->arr);
            }
            break;
        default:
            break;
    }

    return el;
}

int write_rem_list(REM_LIST **list, FILE *data_fptr){
    fseek(data_fptr, TOPO_OFFSET, SEEK_SET);

    for(int i = 0; i < get_len_dynarr(&(*list)->arr); i++){
        REM_EL *el = get_dynarr(i, &(*list)->arr);
        if(el){
            if(set_campo64(el->offset, data_fptr) == -1){
                return -1;
            }

            // Seek para o registro e pular para o campo prox
            fseek(data_fptr, el->offset+1+4, SEEK_SET);
        }
    }

    if(set_campo64(-1, data_fptr)){
        return -1;
    }

    return 0;
}

// static int compare_rem_el(const void* a, const void* b){
//     return ((REM_EL*)((ARR_EL*)a)->el)->tam - ((REM_EL*)((ARR_EL*)b)->el)->tam;
// }

void sort_rem_list(REM_LIST **list){
    // O algorimo tem que ser estável para os casos testes
    // qsort(get_raw_dyarr(&(*list)->arr), get_len_dynarr(&(*list)->arr), sizeof(ARR_EL), &compare_rem_el);
}