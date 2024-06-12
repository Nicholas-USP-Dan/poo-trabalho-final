/**
 * @file data-utils.c
 * @brief Source file de algumas utilidades gerais do arquivo binário de dados
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
#include <inttypes.h>

#include "data-utils.h"

#include "campo-utils.h"
#include "funcoes_fornecidas.h"

const JOGADOR jNil = {
    .id = -1,
    .idade = -1,
    .nome = "",
    .nac = "",
    .clube = ""
}; // Definição externa

const char *CAMPO_LIST[5] = {
    "id",
    "idade",
    "nomeJogador",
    "nacionalidade",
    "nomeClube"
}; // Definição externa

void free_jogador(JOGADOR *j){
    free(j->nome);
    free(j->nac);
    free(j->clube);
}

void print_jogador(const JOGADOR j){
    printf("%" PRId32 ",", j.id);
    if(j.idade != -1){
        printf("%" PRId32, j.idade);
    }
    printf(",");

    printf("%s,", j.nome);
    printf("%s,", j.nac);
    printf("%s\n", j.clube);
}

JOGADOR read_jogador_data(FILE *fptr){
    JOGADOR j;

    j.id = get_campo32(fptr);
    j.idade = get_campo32(fptr);
    j.nome = get_campo_str(fptr);
    j.nac = get_campo_str(fptr);
    j.clube = get_campo_str(fptr);

    return j;
}

JOGADOR read_query(){
    JOGADOR j_query = jNil;

    // Quantidade de campos a serem lidos
    int m;
    scanf("%d", &m);

    // Alocação e atribuição inicial dos campos de texto de um jogador (nome, nac e clube)
    j_query.nome = calloc(sizeof(char), BUFFER_SIZE);
    j_query.nac = calloc(sizeof(char), BUFFER_SIZE);
    j_query.clube = calloc(sizeof(char), BUFFER_SIZE);

    // Lê o conjunto nomeCampo e valorCampo, atribuir o escolhido campo com o valor correspondente n vezes
    for(int i = 0; i < m; i++){
        // Lê nomeCampo
        char campo[BUFFER_SIZE];
        scanf("%s", campo);

        // "Switch com os diferentes nomes dos campos"
        if(strcmp(campo, CAMPO_LIST[0]) == 0){
            scanf("%" PRId32, &j_query.id);
        }
        else if(strcmp(campo, CAMPO_LIST[1]) == 0){
            scanf("%" PRId32, &j_query.idade);
        }
        else if(strcmp(campo, CAMPO_LIST[2]) == 0){
            scan_quote_string(j_query.nome);
        }
        else if(strcmp(campo, CAMPO_LIST[3]) == 0){
            scan_quote_string(j_query.nac);
        }
        else if(strcmp(campo, CAMPO_LIST[4]) == 0){
            scan_quote_string(j_query.clube);
        }
        else{
            break;
        }
    }

    return j_query;
}

JOGADOR read_new_jogador(){
    JOGADOR j_out;

    // Alocação e atribuição inicial dos campos de texto de um jogador (nome, nac e clube)
    j_out.nome = calloc(sizeof(char), BUFFER_SIZE);
    j_out.nac = calloc(sizeof(char), BUFFER_SIZE);
    j_out.clube = calloc(sizeof(char), BUFFER_SIZE);

    scanf("%" PRId32, &j_out.id);

    char buffer[BUFFER_SIZE];
    scanf("%s", buffer);
    if(strcmp(buffer, "NULO") == 0){
        j_out.idade = -1;
    }
    else{
        sscanf(buffer, "%" PRId32, &j_out.idade);
    }

    scan_quote_string(j_out.nome);
    if(strcmp(j_out.nome, "NULO") == 0){
        strcpy(j_out.nome, "");
    }
    scan_quote_string(j_out.nac);
    if(strcmp(j_out.nac, "NULO") == 0){
        strcpy(j_out.nac, "");
    }
    scan_quote_string(j_out.clube);
    if(strcmp(j_out.clube, "NULO") == 0){
        strcpy(j_out.clube, "");
    }

    return j_out;
}

int32_t get_reg_size(const JOGADOR j){
    // soma de bytes para a formação do registro
    int32_t lenNomeJog = strlen(j.nome);
    int32_t lenNac = strlen(j.nac);
    int32_t lenNomeClube = strlen(j.clube);

    // removido = 1 | tamReg = 4 | prox = 8 | id = 4 | idade = 4 | tamNomeJog = 4 | nomeJog = var | tamNac = 4 | nac = var | tamNomeClube = 4 | nomeClube = var
    return 1 + 4 + 8 + 4 + 4 + 4 + lenNomeJog + 4 + lenNac + 4 + lenNomeClube;
}

/**
 * @brief Adquire uma máscara de busca a partir de um critério de busca; Ela é usada para
 * ignorar os elementos vazios do critério
 * 
 * @param where 
 * @return [unsigned int] 
 */
static unsigned int get_mask(const JOGADOR where){
    unsigned int mask = 0;
    if(where.id != jNil.id){
        mask |= IDMASK;
    }
    if(where.idade != jNil.idade){
        mask |= IDADEMASK;
    }
    if(strcmp(where.nome, jNil.nome) != 0){
        mask |= NOMEMASK;
    }
    if(strcmp(where.nac, jNil.nac) != 0){
        mask |= NACMASK;
    }
    if(strcmp(where.clube, jNil.clube) != 0){
        mask |= CLUBEMASK;
    }

    return mask;
}

int pass_where(const JOGADOR j, const JOGADOR where){
    unsigned int mask = get_mask(where);
    return  (!(mask & IDMASK) || where.id == j.id) && 
            (!(mask & IDADEMASK) || where.idade == j.idade) && 
            (!(mask & NOMEMASK) || strcmp(where.nome, j.nome) == 0) && 
            (!(mask & NACMASK) || strcmp(where.nac, j.nac) == 0) && 
            (!(mask & CLUBEMASK) || strcmp(where.clube, j.clube) == 0);
}

/**
 * @brief Adiciona um registro em um arquivo binário de dados
 * 
 * @param j Dados do registro a ser inserido.
 * @param data_fptr Ponteiro para o arquivo binário de dados
 * 
 * @retval -1 Houve um erro durante a adição do registro no arquivo binário.
 * @retval 0 Registro adicionado no arquivo binário com sucesso.
 */
int append_reg(const JOGADOR j, FILE *data_fptr){
    set_campoc('0', data_fptr); // Atribuição do campo removido como '0'

    set_campo32(get_reg_size(j), data_fptr); // Atribuição do campo "tamReg"

    set_campo64(-1, data_fptr); // Atribuicao do campo Prox

    set_campo32(j.id, data_fptr); // Atribuicao do campo id
    set_campo32(j.idade, data_fptr); // Atribuicao do campo idade
    set_campo_str(j.nome, data_fptr); // Atribuicao do campo tamNomeJog e nomeJogador
    set_campo_str(j.nac, data_fptr); // Atribuicao do campo tamNacionalidade e nacionalidade
    set_campo_str(j.clube, data_fptr); // Atribuicao do campo tamNomeClube e nomeClube

    return 0;
}