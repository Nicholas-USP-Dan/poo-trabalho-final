/**
 * @file cabecalho-utils.h
 * @brief Header file para funcionalidades do cabeçalho.
 * 
 * Este arquivo contem as declaracoes de funcoes dedicadas a manipulacao do cabecalho do arquivo de dados como especificado
 * no trabalho introdutório da disciplina de Organizacao de Arquivos.
 * 
 * ESTRUTURA DO CABEÇALHO:
 * 
   @verbatim
    0                 1                 9                          17                    21                    25
    | status (1 byte) |  topo (8 bytes) | proxByteOffset (8 bytes) | nroRegArq (4 bytes) | nroRegRem (4 bytes) |
   @endverbatim
 * 
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 * 
 * @version 2.0
 * 
 */

#ifndef CABCL_UTILS_H
#define CABCL_UTILS_H

#include <stdint.h>
#include <stdio.h>

// Lista de offsets dos campos do cabecalho (campos estaticos)

#define STATUS_OFFSET 0
#define TOPO_OFFSET 1
#define PROXBYTE_OFFSET 9
#define NRO_REGARQ_OFFSET 17
#define NRO_REGREM_OFFSET 21
#define HEADER_END_OFFSET 25

/**
 * @brief Inicializa um novo cabecalho em um arquivo de dados binario.
 * 
 * @details Como a função é composta unicamente pela função fwrite, é possível ler o 
 * stderr para a identificação de erro.
 * 
 * @param status Valor a ser inserido no campo status.
 * @param topo Valor a ser inserido no campo topo.
 * @param prox_byte_offset Valor a ser inserido no campo proxByteOffset.
 * @param nro_regarq Valor a ser inserido no campo nroRegArq.
 * @param nro_regrem Valor a ser inserido no campo nroRegRem.
 * @param data_fptr Ponteiro do arquivo binário de dados.
 * 
 * @retval 0 Operação realizada com sucesso.
 * @retval -1 Houve uma falha durante a execução da funcionalidade.
 */
int initialize_data_cabecalho(unsigned char status, int64_t topo, int64_t prox_byte_offset, 
int32_t nro_regarq, int32_t nro_regrem, FILE *data_fptr);

/**
 * @brief Funcao que verifica o status de um arquivo binario
 * 
 * @param data_fptr Ponteiro do arquivo binário de dados.
 * 
 * @retval 1 Status do arquivo é válido (status = '1')
 * @retval 0 Status do arquivo não é válido
 * 
 * @note A função não reposiciona o ponteiro para o offset do status, então o ponteiro 
 * deve estar na posição correta (STATUS_OFFSET).
 */
int check_status(FILE *data_fptr);

/**
 * @brief Atualiza os campos nro_regarq e nro_regrem
 * 
 * @param diff Diferença da quantidade final pela inicial ((>0): mais registros; 
 * (<0): menos registros)
 * @param data_fptr Ponteiro do arquivo binário de dados
 * 
 * @retval 0 Operação realizada com sucesso.
 * @retval -1 Houve uma falha durante a execução da funcionalidade.
 */
int update_nro_reg(int diff, FILE *data_fptr);

#endif