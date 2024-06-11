/**
 * @file create-data-file.c
 * @brief Source file para a criação de um arquivo de dados a partir de um arquivo csv
 * 
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 * 
 * @version 2.0
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>

#include "data-file.h"

#include "utils/data-utils.h"
#include "utils/campo-utils.h"
#include "utils/cabecalho-utils.h"

/**
 * @brief Remove o caracter newline ('\n') ao fim de uma string
 */
#define TRIM_NEWLINE(str) str[strcspn(str, "\n")] = 0

/**
 * @brief Implementação semelhante da função strtok(), porém com leitura de campos vazios.
 * 
 * @details A implementação é feita através da função strchr().
 * 
 * @param start_ptr Ponteiro que aponta para o inicio da token
 * @param delim Caractere delimitador das tokens
 * @return [char*] Retorna a token obtida (string alocada dinâmicamente)
 */
static char* get_token_str(char **start_ptr, const char delim){
    if((*start_ptr) == NULL){
        return NULL;
    }
    
    char *token = calloc(BUFFER_SIZE/2, sizeof(char));

    char *end_token = strchr((*start_ptr), delim);
    if(end_token != NULL){
        strncpy(token, (*start_ptr), end_token-(*start_ptr));
        token[BUFFER_SIZE/2-1] = '\0'; // Asserção para garantir que token termine com '\0'
        (*start_ptr) = end_token+1;
    }
    else{
        strncpy(token, (*start_ptr), BUFFER_SIZE/2);
        token[BUFFER_SIZE/2-1] = '\0'; // Asserção para garantir que token termine com '\0'
        (*start_ptr) = NULL;
    }

    return token;
}

/**
 * @brief Processa uma linha extraída de um arquivo .csv (string com tokens separadas por ',').
 * 
 * @param line Linha (string) a ser processada.
 * @return [JOGADOR] Retorna um objeto do tipo JOGADOR com os dados obtidos pela função
 */
static JOGADOR process_csv_jogador(char *line){
    JOGADOR j_out;
    char *line_ptr = line;
    int32_t len;

    // Ler o id do jogador
    char *id_str = get_token_str(&line_ptr, ',');
    sscanf(id_str, "%" PRId32, &j_out.id);
    free(id_str);
    
    // Ler a idade do jogador
    char *idade_str = get_token_str(&line_ptr, ',');
    if(strcmp(idade_str, "") != 0){
        sscanf(idade_str, "%" PRId32, &j_out.idade);
    }
    else{ // Idade não definida
        j_out.idade = -1;
    }
    free(idade_str);

    // Ler o nome do jogador
    char *nome_str = get_token_str(&line_ptr, ',');
    len = strlen(nome_str);
    j_out.nome = (char *)malloc(len+1);
    strncpy(j_out.nome, nome_str, len+1);
    free(nome_str);

    // Ler a nacionalidade do jogador
    char *nac_str = get_token_str(&line_ptr, ',');
    len = strlen(nac_str);
    j_out.nac = (char *)malloc(len+1);
    strncpy(j_out.nac, nac_str, len+1);
    free(nac_str);

    // Ler o clube do jogador
    char *clube_str = get_token_str(&line_ptr, ',');
    len = strlen(clube_str);
    j_out.clube = (char *)malloc(len+1);
    strncpy(j_out.clube, clube_str, len+1);
    free(clube_str);

    return j_out;
}

int create_data_file(FILE *csv_fptr, FILE *data_fptr){
    // Pular cabecalho
    fseek(data_fptr, HEADER_END_OFFSET, SEEK_SET);

    // Ler a primeira linha do arquivo .csv (colunas)
    char columns[BUFFER_SIZE];
    fgets(columns, BUFFER_SIZE, csv_fptr);
    TRIM_NEWLINE(columns);

    // Verifica se as colunas do .csv correspondem ao do projeto
    if(strcmp(columns, COLUMN_NAMES) != 0){
        errno = 95; // Operation not supported
        return -1;
    }

    // Contador da quantidade de registros
    int32_t reg_count = 0;

    while(1){
        // Ler linha no csv
        char line_buff[BUFFER_SIZE];
        fgets(line_buff, sizeof(char) * BUFFER_SIZE, csv_fptr);
        TRIM_NEWLINE(line_buff);

        if(feof(csv_fptr)){
            break;
        }

        // Processa a linha do csv e coloca o jogador no arquivo binario
        JOGADOR j = process_csv_jogador(line_buff);
        append_reg(j, data_fptr);
        free_jogador(&j);

        reg_count++;
    }

    // Lê o endereço do final do arquivo para ser escrito no campo proxByteOffset (cabeçalho)
    int64_t prox_byte_offset = ftell(data_fptr);
    
    // Atribuição/Inicialização do cabecalho
    fseek(data_fptr, 0, SEEK_SET);
    initialize_data_cabecalho('1', -1, prox_byte_offset, reg_count, 0, data_fptr);

    return 0;
}