/**
 * @file insert-data.c
 * @brief Implementação da funcionalidade 2 & 3 como definido no projeto
 * 
 * O tipo de jogador é utilizado para representar uma busca filtrada (a estrutura contém todos os campos de um registro);
 * Como definido nas especificações do projeto, o id de cada jogador é único, assim, em buscas que filtram o id de um jogador,
 * É possível sair da busca assim que achar-se um registro com o mesmo id que o filtrado
 * 
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 * 
 * @version 2.0
 * 
 */

#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>

#include "adts/dyn-array.h"
#include "data-file.h"

#include "index-file.h"
#include "utils/data-utils.h"
#include "utils/campo-utils.h"
#include "utils/cabecalho-utils.h"
#include "utils/removed-list.h"

int insert_data(FILE *data_fptr, const JOGADOR j, int *quant_ins, REM_LIST **rem_list, DYN_ARRAY **index_arr){
    int ret = -1;

    // Settar status do cabeçalho pra inconsistente (0)
    fseek(data_fptr, STATUS_OFFSET, SEEK_SET);
    // char status = '0';
    // fwrite(&status, 1, 1, data_fptr);
    set_campoc('0', data_fptr);

    // Ler o campo "proxByteOffset"
    fseek(data_fptr, PROXBYTE_OFFSET, SEEK_SET);
    int64_t proxbyte_offset = get_campo64(data_fptr);

    int32_t reg_size = get_reg_size(j);
    REM_EL rem_el = find_space(reg_size, rem_list);

    int64_t offset = rem_el.offset != -1 ? rem_el.offset : proxbyte_offset;

    if(rem_el.offset == -1){
        fseek(data_fptr, proxbyte_offset, SEEK_SET);

        // Escrevendo o novo registro
        append_reg(j, data_fptr);

        // Atualizar o valor do proxbyte_offset
        proxbyte_offset = ftell(data_fptr);
        fseek(data_fptr, PROXBYTE_OFFSET, SEEK_SET);
        set_campo64(proxbyte_offset, data_fptr);
    }
    else { // Inserir no espaço encontrado
        // Ir para o offset encontrado, verificar o tam, escrever o novo registro e escrever $ no lixo

        // Ir para o offset encontrado, pulando o char de removido
        fseek(data_fptr, rem_el.offset+1, SEEK_SET);
        int32_t rem_size = get_campo32(data_fptr);

        // Escrevendo o novo registro
        fseek(data_fptr, rem_el.offset, SEEK_SET);
        append_reg(j, data_fptr);

        int32_t aux_size = reg_size;
        while(aux_size < rem_size){
            char trash = '$';
            set_campoc(trash, data_fptr);
            aux_size++;
        }
        
        // Alterando o tamanho do registro para manter o antigo (do registro removido)
        fseek(data_fptr, rem_el.offset+1, SEEK_SET);
        // fwrite(&rem_size, 4, 1, data_fptr);
        set_campo32(rem_size, data_fptr);
    }

    INDEX_REG *reg = (INDEX_REG*)malloc(sizeof(INDEX_REG));
    reg->index = j.id;
    reg->offset = offset;
    insert_ord_dynarr(reg, index_arr);

    (*quant_ins)++;

    ret = 0;
    
    return ret;
}