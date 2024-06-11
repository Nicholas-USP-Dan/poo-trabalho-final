/**
 * @file index-file.c
 * @brief Source file para as funcionalidades relacionadas ao arquivo de índice.
 * 
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 * 
 * @version 2.0
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>

#include "index-file.h"

#include "adts/dyn-array.h"

#include "utils/data-utils.h"
#include "utils/campo-utils.h"
#include "utils/cabecalho-utils.h"

/**
 * @brief Função para comparar dois registros de índice (INDEX_REG);
 * Ela é usada para ordenar a lista de índices
 * 
 * @param a Primeiro ponteiro
 * @param b Segundo ponteiro
 * @return [int] Retorna positivo se a > b, ou negativo se a < b
 */
static int index_compare(const void* a, const void* b){
    return ((INDEX_REG*)((ARR_EL*)a)->el)->index - ((INDEX_REG*)((ARR_EL*)b)->el)->index;
}

int64_t get_index(const void *index_reg){
    return (int64_t)((INDEX_REG*)index_reg)->index;
}

DYN_ARRAY* generate_index(FILE *data_fptr){
    DYN_ARRAY *index_arr = initialize_dynarr(&get_index);

    // Pular o cabecalho
    fseek(data_fptr, HEADER_END_OFFSET, SEEK_SET);

    // Lê registros até ler todos os registros válidos
    while(1){
        int64_t reg_offset = ftell(data_fptr);
        unsigned char rem = get_campoc(data_fptr);

        if(feof(data_fptr)){
            break;
        }

        int32_t reg_size = get_campo32(data_fptr);

        // Registro esta removido, mover para o proximo
        if(rem == '1'){
            fseek(data_fptr, reg_size-5, SEEK_CUR);
            continue;
        }

        // Pular tamanhoRegistro e Prox
        fseek(data_fptr, 8, SEEK_CUR);

        JOGADOR j = read_jogador_data(data_fptr);

        // Cria o registro do índice e insere ele na lista de índices
        INDEX_REG *reg_temp = malloc(sizeof(INDEX_REG));
        reg_temp->index = j.id;
        reg_temp->offset = reg_offset;
        insert_back_dynarr(reg_temp, &index_arr);

        free_jogador(&j);

        fseek(data_fptr, reg_offset + reg_size, SEEK_SET);
    }

    // Ordenação da lista de índices
    qsort(get_raw_dyarr(&index_arr), get_len_dynarr(&index_arr), sizeof(ARR_EL), &index_compare);

    return index_arr;
}

int write_index(DYN_ARRAY **index_arr, FILE *index_fptr){
    // Volta para o começo e pula o campo status
    fseek(index_fptr, 1, SEEK_SET);

    // Passa por todos os itens da lista de índices e escreve eles no arquivo
    for(int i = 0; i < get_len_dynarr(index_arr); i++){
        INDEX_REG *reg = (INDEX_REG*)get_dynarr(i, index_arr);
        if(!reg) continue; // Verifica se o item não está removido
        
        // Atribui o registro no arquivo
        if(set_campo32(reg->index, index_fptr) == -1 ||
        set_campo64(reg->offset, index_fptr) == -1){
            return -1;
        }
    }

    // Escrever o status do arquivo de índice como estável
    fseek(index_fptr, 0, SEEK_SET);
    if(set_campoc('1', index_fptr) == -1){
        return -1;
    }

    return 0;
}

DYN_ARRAY* load_index(FILE *index_fptr){
    DYN_ARRAY *array = initialize_dynarr(&get_index);

    // Lê registros até o fim do arquivo
    while(1){
        // Inicializa e atribui um item de índice
        INDEX_REG *reg = (INDEX_REG*)malloc(sizeof(INDEX_REG));
        reg->index = get_campo32(index_fptr);
        reg->offset = get_campo64(index_fptr);

        if(feof(index_fptr)){
            free(reg);
            break;
        }

        // Insere o registro na lista de índices
        insert_back_dynarr(reg, &array);
    }

    return array;
}