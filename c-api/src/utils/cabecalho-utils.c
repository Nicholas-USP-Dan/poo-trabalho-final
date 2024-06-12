/**
 * @file cabecalho-utils.c
 * @brief Source file para as funcionalidades do cabeçalho
 * 
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 * 
 * @version 2.0
 * 
 */

#include <stdio.h>
#include <errno.h>

#include "cabecalho-utils.h"

int initialize_data_cabecalho(const unsigned char status, const int64_t topo, const int64_t prox_byte_offset, 
const int32_t nro_regarq, const int32_t nro_regrem, FILE *data_fptr){
    // Escrita dos campos no arquivo binário
    
    fwrite(&status, 1, 1, data_fptr);
    fwrite(&topo, 8, 1, data_fptr);
    fwrite(&prox_byte_offset, 8, 1, data_fptr);
    fwrite(&nro_regarq, 4, 1, data_fptr);
    fwrite(&nro_regrem, 4, 1, data_fptr);

    if(ftell(data_fptr) != HEADER_END_OFFSET){ // Verifica se todos os bytes foram escritos
        return -1;
    }

    return 0;
}

int check_status(FILE *data_fptr){
    char status;
    fread(&status, 1, 1, data_fptr);
    if(status != '1') errno = 77; // Atribui errno como EBADFD 77 File descriptor in bad state
    return status == '1';
}

int update_nro_reg(const int diff, FILE *data_fptr){
    int32_t nro_regarq;
    int32_t nro_regrem;

    fseek(data_fptr, NRO_REGARQ_OFFSET, SEEK_SET);

    if(fread(&nro_regarq, 4, 1, data_fptr) != 1 ||
    fread(&nro_regrem, 4, 1, data_fptr) != 1){
        return -1;
    }

    nro_regarq += diff;
    nro_regrem -= diff;
    if(nro_regrem < 0){
        nro_regrem = 0;
    }

    fseek(data_fptr, NRO_REGARQ_OFFSET, SEEK_SET);

    if(fwrite(&nro_regarq, 4, 1, data_fptr) != 1 ||
    fwrite(&nro_regrem, 4, 1, data_fptr) != 1){
        return -1;
    }

    return 0;
}